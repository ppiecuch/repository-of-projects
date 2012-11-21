/*
2	
 * Copyright (C) 2008 The Android Open Source Project
3	
 *
4	
 * Licensed under the Apache License, Version 2.0 (the "License");
5	
 * you may not use this file except in compliance with the License.
6	
 * You may obtain a copy of the License at
7	
 *
8	
 *      http://www.apache.org/licenses/LICENSE-2.0
9	
 *
10	
 * Unless required by applicable law or agreed to in writing, software
11	
 * distributed under the License is distributed on an "AS IS" BASIS,
12	
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
13	
 * See the License for the specific language governing permissions and
14	
 * limitations under the License.
15	
 */
16	
17	
package android.opengl;
18	
19	
import java.io.Writer;
20	
import java.util.ArrayList;
21	
import java.util.concurrent.Semaphore;
22	
23	
import javax.microedition.khronos.egl.EGL10;
24	
import javax.microedition.khronos.egl.EGL11;
25	
import javax.microedition.khronos.egl.EGLConfig;
26	
import javax.microedition.khronos.egl.EGLContext;
27	
import javax.microedition.khronos.egl.EGLDisplay;
28	
import javax.microedition.khronos.egl.EGLSurface;
29	
import javax.microedition.khronos.opengles.GL;
30	
import javax.microedition.khronos.opengles.GL10;
31	
32	
import android.content.Context;
33	
import android.util.AttributeSet;
34	
import android.util.Log;
35	
import android.view.SurfaceHolder;
36	
import android.view.SurfaceView;
37	
38	
/**
39	
 * An implementation of SurfaceView that uses the dedicated surface for
40	
 * displaying OpenGL rendering.
41	
 * <p>
42	
 * A GLSurfaceView provides the following features:
43	
 * <p>
44	
 * <ul>
45	
 * <li>Manages a surface, which is a special piece of memory that can be
46	
 * composited into the Android view system.
47	
 * <li>Manages an EGL display, which enables OpenGL to render into a surface.
48	
 * <li>Accepts a user-provided Renderer object that does the actual rendering.
49	
 * <li>Renders on a dedicated thread to decouple rendering performance from the
50	
 * UI thread.
51	
 * <li>Supports both on-demand and continuous rendering.
52	
 * <li>Optionally wraps, traces, and/or error-checks the renderer's OpenGL calls.
53	
 * </ul>
54	
 *
55	
 * <h3>Using GLSurfaceView</h3>
56	
 * <p>
57	
 * Typically you use GLSurfaceView by subclassing it and overriding one or more of the
58	
 * View system input event methods. If your application does not need to override event
59	
 * methods then GLSurfaceView can be used as-is. For the most part
60	
 * GLSurfaceView behavior is customized by calling "set" methods rather than by subclassing.
61	
 * For example, unlike a regular View, drawing is delegated to a separate Renderer object which
62	
 * is registered with the GLSurfaceView
63	
 * using the {@link #setRenderer(Renderer)} call.
64	
 * <p>
65	
 * <h3>Initializing GLSurfaceView</h3>
66	
 * All you have to do to initialize a GLSurfaceView is call {@link #setRenderer(Renderer)}.
67	
 * However, if desired, you can modify the default behavior of GLSurfaceView by calling one or
68	
 * more of these methods before calling setRenderer:
69	
 * <ul>
70	
 * <li>{@link #setDebugFlags(int)}
71	
 * <li>{@link #setEGLConfigChooser(boolean)}
72	
 * <li>{@link #setEGLConfigChooser(EGLConfigChooser)}
73	
 * <li>{@link #setEGLConfigChooser(int, int, int, int, int, int)}
74	
 * <li>{@link #setGLWrapper(GLWrapper)}
75	
 * </ul>
76	
 * <p>
77	
 * <h4>Choosing an EGL Configuration</h4>
78	
 * A given Android device may support multiple possible types of drawing surfaces.
79	
 * The available surfaces may differ in how may channels of data are present, as
80	
 * well as how many bits are allocated to each channel. Therefore, the first thing
81	
 * GLSurfaceView has to do when starting to render is choose what type of surface to use.
82	
 * <p>
83	
 * By default GLSurfaceView chooses an available surface that's closest to a 16-bit R5G6B5 surface
84	
 * with a 16-bit depth buffer and no stencil. If you would prefer a different surface (for example,
85	
 * if you do not need a depth buffer) you can override the default behavior by calling one of the
86	
 * setEGLConfigChooser methods.
87	
 * <p>
88	
 * <h4>Debug Behavior</h4>
89	
 * You can optionally modify the behavior of GLSurfaceView by calling
90	
 * one or more of the debugging methods {@link #setDebugFlags(int)},
91	
 * and {@link #setGLWrapper}. These methods may be called before and/or after setRenderer, but
92	
 * typically they are called before setRenderer so that they take effect immediately.
93	
 * <p>
94	
 * <h4>Setting a Renderer</h4>
95	
 * Finally, you must call {@link #setRenderer} to register a {@link Renderer}.
96	
 * The renderer is
97	
 * responsible for doing the actual OpenGL rendering.
98	
 * <p>
99	
 * <h3>Rendering Mode</h3>
100	
 * Once the renderer is set, you can control whether the renderer draws
101	
 * continuously or on-demand by calling
102	
 * {@link #setRenderMode}. The default is continuous rendering.
103	
 * <p>
104	
 * <h3>Activity Life-cycle</h3>
105	
 * A GLSurfaceView must be notified when the activity is paused and resumed. GLSurfaceView clients
106	
 * are required to call {@link #onPause()} when the activity pauses and
107	
 * {@link #onResume()} when the activity resumes. These calls allow GLSurfaceView to
108	
 * pause and resume the rendering thread, and also allow GLSurfaceView to release and recreate
109	
 * the OpenGL display.
110	
 * <p>
111	
 * <h3>Handling events</h3>
112	
 * <p>
113	
 * To handle an event you will typically subclass GLSurfaceView and override the
114	
 * appropriate method, just as you would with any other View. However, when handling
115	
 * the event, you may need to communicate with the Renderer object
116	
 * that's running in the rendering thread. You can do this using any
117	
 * standard Java cross-thread communication mechanism. In addition,
118	
 * one relatively easy way to communicate with your renderer is
119	
 * to call
120	
 * {@link #queueEvent(Runnable)}. For example:
121	
 * <pre class="prettyprint">
122	
 * class MyGLSurfaceView extends GLSurfaceView {
123	
 *
124	
 *     private MyRenderer mMyRenderer;
125	
 *
126	
 *     public void start() {
127	
 *         mMyRenderer = ...;
128	
 *         setRenderer(mMyRenderer);
129	
 *     }
130	
 *
131	
 *     public boolean onKeyDown(int keyCode, KeyEvent event) {
132	
 *         if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
133	
 *             queueEvent(new Runnable() {
134	
 *                 // This method will be called on the rendering
135	
 *                 // thread:
136	
 *                 public void run() {
137	
 *                     mMyRenderer.handleDpadCenter();
138	
 *                 }});
139	
 *             return true;
140	
 *         }
141	
 *         return super.onKeyDown(keyCode, event);
142	
 *     }
143	
 * }
144	
 * </pre>
145	
 *
146	
 */
147	
public class GLSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
148	
    /**
149	
     * The renderer only renders
150	
     * when the surface is created, or when {@link #requestRender} is called.
151	
     *
152	
     * @see #getRenderMode()
153	
     * @see #setRenderMode(int)
154	
     */
155	
    public final static int RENDERMODE_WHEN_DIRTY = 0;
156	
    /**
157	
     * The renderer is called
158	
     * continuously to re-render the scene.
159	
     *
160	
     * @see #getRenderMode()
161	
     * @see #setRenderMode(int)
162	
     * @see #requestRender()
163	
     */
164	
    public final static int RENDERMODE_CONTINUOUSLY = 1;
165	
166	
    /**
167	
     * Check glError() after every GL call and throw an exception if glError indicates
168	
     * that an error has occurred. This can be used to help track down which OpenGL ES call
169	
     * is causing an error.
170	
     *
171	
     * @see #getDebugFlags
172	
     * @see #setDebugFlags
173	
     */
174	
    public final static int DEBUG_CHECK_GL_ERROR = 1;
175	
176	
    /**
177	
     * Log GL calls to the system log at "verbose" level with tag "GLSurfaceView".
178	
     *
179	
     * @see #getDebugFlags
180	
     * @see #setDebugFlags
181	
     */
182	
    public final static int DEBUG_LOG_GL_CALLS = 2;
183	
184	
    /**
185	
     * Standard View constructor. In order to render something, you
186	
     * must call {@link #setRenderer} to register a renderer.
187	
     */
188	
    public GLSurfaceView(Context context) {
189	
        super(context);
190	
        init();
191	
    }
192	
193	
    /**
194	
     * Standard View constructor. In order to render something, you
195	
     * must call {@link #setRenderer} to register a renderer.
196	
     */
197	
    public GLSurfaceView(Context context, AttributeSet attrs) {
198	
        super(context, attrs);
199	
        init();
200	
    }
201	
202	
    private void init() {
203	
        // Install a SurfaceHolder.Callback so we get notified when the
204	
        // underlying surface is created and destroyed
205	
        SurfaceHolder holder = getHolder();
206	
        holder.addCallback(this);
207	
        holder.setType(SurfaceHolder.SURFACE_TYPE_GPU);
208	
    }
209	
210	
    /**
211	
     * Set the glWrapper. If the glWrapper is not null, its
212	
     * {@link GLWrapper#wrap(GL)} method is called
213	
     * whenever a surface is created. A GLWrapper can be used to wrap
214	
     * the GL object that's passed to the renderer. Wrapping a GL
215	
     * object enables examining and modifying the behavior of the
216	
     * GL calls made by the renderer.
217	
     * <p>
218	
     * Wrapping is typically used for debugging purposes.
219	
     * <p>
220	
     * The default value is null.
221	
     * @param glWrapper the new GLWrapper
222	
     */
223	
    public void setGLWrapper(GLWrapper glWrapper) {
224	
        mGLWrapper = glWrapper;
225	
    }
226	
227	
    /**
228	
     * Set the debug flags to a new value. The value is
229	
     * constructed by OR-together zero or more
230	
     * of the DEBUG_CHECK_* constants. The debug flags take effect
231	
     * whenever a surface is created. The default value is zero.
232	
     * @param debugFlags the new debug flags
233	
     * @see #DEBUG_CHECK_GL_ERROR
234	
     * @see #DEBUG_LOG_GL_CALLS
235	
     */
236	
    public void setDebugFlags(int debugFlags) {
237	
        mDebugFlags = debugFlags;
238	
    }
239	
240	
    /**
241	
     * Get the current value of the debug flags.
242	
     * @return the current value of the debug flags.
243	
     */
244	
    public int getDebugFlags() {
245	
        return mDebugFlags;
246	
    }
247	
248	
    /**
249	
     * Set the renderer associated with this view. Also starts the thread that
250	
     * will call the renderer, which in turn causes the rendering to start.
251	
     * <p>This method should be called once and only once in the life-cycle of
252	
     * a GLSurfaceView.
253	
     * <p>The following GLSurfaceView methods can only be called <em>before</em>
254	
     * setRenderer is called:
255	
     * <ul>
256	
     * <li>{@link #setEGLConfigChooser(boolean)}
257	
     * <li>{@link #setEGLConfigChooser(EGLConfigChooser)}
258	
     * <li>{@link #setEGLConfigChooser(int, int, int, int, int, int)}
259	
     * </ul>
260	
     * <p>
261	
     * The following GLSurfaceView methods can only be called <em>after</em>
262	
     * setRenderer is called:
263	
     * <ul>
264	
     * <li>{@link #getRenderMode()}
265	
     * <li>{@link #onPause()}
266	
     * <li>{@link #onResume()}
267	
     * <li>{@link #queueEvent(Runnable)}
268	
     * <li>{@link #requestRender()}
269	
     * <li>{@link #setRenderMode(int)}
270	
     * </ul>
271	
     *
272	
     * @param renderer the renderer to use to perform OpenGL drawing.
273	
     */
274	
    public void setRenderer(Renderer renderer) {
275	
        if (mGLThread != null) {
276	
            throw new IllegalStateException(
277	
                    "setRenderer has already been called for this instance.");
278	
        }
279	
        if (mEGLConfigChooser == null) {
280	
            mEGLConfigChooser = new SimpleEGLConfigChooser(true);
281	
        }
282	
        mGLThread = new GLThread(renderer);
283	
        mGLThread.start();
284	
    }
285	
286	
    /**
287	
     * Install a custom EGLConfigChooser.
288	
     * <p>If this method is
289	
     * called, it must be called before {@link #setRenderer(Renderer)}
290	
     * is called.
291	
     * <p>
292	
     * If no setEGLConfigChooser method is called, then by default the
293	
     * view will choose a config as close to 16-bit RGB as possible, with
294	
     * a depth buffer as close to 16 bits as possible.
295	
     * @param configChooser
296	
     */
297	
    public void setEGLConfigChooser(EGLConfigChooser configChooser) {
298	
        if (mGLThread != null) {
299	
            throw new IllegalStateException(
300	
                    "setRenderer has already been called for this instance.");
301	
        }
302	
        mEGLConfigChooser = configChooser;
303	
    }
304	
305	
    /**
306	
     * Install a config chooser which will choose a config
307	
     * as close to 16-bit RGB as possible, with or without an optional depth
308	
     * buffer as close to 16-bits as possible.
309	
     * <p>If this method is
310	
     * called, it must be called before {@link #setRenderer(Renderer)}
311	
     * is called.
312	
     * <p>
313	
      * If no setEGLConfigChooser method is called, then by default the
314	
     * view will choose a config as close to 16-bit RGB as possible, with
315	
     * a depth buffer as close to 16 bits as possible.
316	
     *
317	
     * @param needDepth
318	
     */
319	
    public void setEGLConfigChooser(boolean needDepth) {
320	
        setEGLConfigChooser(new SimpleEGLConfigChooser(needDepth));
321	
    }
322	
323	
    /**
324	
     * Install a config chooser which will choose a config
325	
     * with at least the specified component sizes, and as close
326	
     * to the specified component sizes as possible.
327	
     * <p>If this method is
328	
     * called, it must be called before {@link #setRenderer(Renderer)}
329	
     * is called.
330	
     * <p>
331	
     * If no setEGLConfigChooser method is called, then by default the
332	
     * view will choose a config as close to 16-bit RGB as possible, with
333	
     * a depth buffer as close to 16 bits as possible.
334	
     *
335	
     */
336	
    public void setEGLConfigChooser(int redSize, int greenSize, int blueSize,
337	
            int alphaSize, int depthSize, int stencilSize) {
338	
        setEGLConfigChooser(new ComponentSizeChooser(redSize, greenSize,
339	
                blueSize, alphaSize, depthSize, stencilSize));
340	
    }
341	
    /**
342	
     * Set the rendering mode. When renderMode is
343	
     * RENDERMODE_CONTINUOUSLY, the renderer is called
344	
     * repeatedly to re-render the scene. When renderMode
345	
     * is RENDERMODE_WHEN_DIRTY, the renderer only rendered when the surface
346	
     * is created, or when {@link #requestRender} is called. Defaults to RENDERMODE_CONTINUOUSLY.
347	
     * <p>
348	
     * Using RENDERMODE_WHEN_DIRTY can improve battery life and overall system performance
349	
     * by allowing the GPU and CPU to idle when the view does not need to be updated.
350	
     * <p>
351	
     * This method can only be called after {@link #setRenderer(Renderer)}
352	
     *
353	
     * @param renderMode one of the RENDERMODE_X constants
354	
     * @see #RENDERMODE_CONTINUOUSLY
355	
     * @see #RENDERMODE_WHEN_DIRTY
356	
     */
357	
    public void setRenderMode(int renderMode) {
358	
        mGLThread.setRenderMode(renderMode);
359	
    }
360	
361	
    /**
362	
     * Get the current rendering mode. May be called
363	
     * from any thread. Must not be called before a renderer has been set.
364	
     * @return the current rendering mode.
365	
     * @see #RENDERMODE_CONTINUOUSLY
366	
     * @see #RENDERMODE_WHEN_DIRTY
367	
     */
368	
    public int getRenderMode() {
369	
        return mGLThread.getRenderMode();
370	
    }
371	
372	
    /**
373	
     * Request that the renderer render a frame.
374	
     * This method is typically used when the render mode has been set to
375	
     * {@link #RENDERMODE_WHEN_DIRTY}, so that frames are only rendered on demand.
376	
     * May be called
377	
     * from any thread. Must not be called before a renderer has been set.
378	
     */
379	
    public void requestRender() {
380	
        mGLThread.requestRender();
381	
    }
382	
383	
    /**
384	
     * This method is part of the SurfaceHolder.Callback interface, and is
385	
     * not normally called or subclassed by clients of GLSurfaceView.
386	
     */
387	
    public void surfaceCreated(SurfaceHolder holder) {
388	
        mGLThread.surfaceCreated();
389	
    }
390	
391	
    /**
392	
     * This method is part of the SurfaceHolder.Callback interface, and is
393	
     * not normally called or subclassed by clients of GLSurfaceView.
394	
     */
395	
    public void surfaceDestroyed(SurfaceHolder holder) {
396	
        // Surface will be destroyed when we return
397	
        mGLThread.surfaceDestroyed();
398	
    }
399	
400	
    /**
401	
     * This method is part of the SurfaceHolder.Callback interface, and is
402	
     * not normally called or subclassed by clients of GLSurfaceView.
403	
     */
404	
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
405	
        mGLThread.onWindowResize(w, h);
406	
    }
407	
408	
    /**
409	
     * Inform the view that the activity is paused. The owner of this view must
410	
     * call this method when the activity is paused. Calling this method will
411	
     * pause the rendering thread.
412	
     * Must not be called before a renderer has been set.
413	
     */
414	
    public void onPause() {
415	
        mGLThread.onPause();
416	
    }
417	
418	
    /**
419	
     * Inform the view that the activity is resumed. The owner of this view must
420	
     * call this method when the activity is resumed. Calling this method will
421	
     * recreate the OpenGL display and resume the rendering
422	
     * thread.
423	
     * Must not be called before a renderer has been set.
424	
     */
425	
    public void onResume() {
426	
        mGLThread.onResume();
427	
    }
428	
429	
    /**
430	
     * Queue a runnable to be run on the GL rendering thread. This can be used
431	
     * to communicate with the Renderer on the rendering thread.
432	
     * Must not be called before a renderer has been set.
433	
     * @param r the runnable to be run on the GL rendering thread.
434	
     */
435	
    public void queueEvent(Runnable r) {
436	
        mGLThread.queueEvent(r);
437	
    }
438	
439	
    /**
440	
     * This method is used as part of the View class and is not normally
441	
     * called or subclassed by clients of GLSurfaceView.
442	
     * Must not be called before a renderer has been set.
443	
     */
444	
    @Override
445	
    protected void onDetachedFromWindow() {
446	
        super.onDetachedFromWindow();
447	
        mGLThread.requestExitAndWait();
448	
    }
449	
450	
    // ----------------------------------------------------------------------
451	
452	
    /**
453	
     * An interface used to wrap a GL interface.
454	
     * <p>Typically
455	
     * used for implementing debugging and tracing on top of the default
456	
     * GL interface. You would typically use this by creating your own class
457	
     * that implemented all the GL methods by delegating to another GL instance.
458	
     * Then you could add your own behavior before or after calling the
459	
     * delegate. All the GLWrapper would do was instantiate and return the
460	
     * wrapper GL instance:
461	
     * <pre class="prettyprint">
462	
     * class MyGLWrapper implements GLWrapper {
463	
     *     GL wrap(GL gl) {
464	
     *         return new MyGLImplementation(gl);
465	
     *     }
466	
     *     static class MyGLImplementation implements GL,GL10,GL11,... {
467	
     *         ...
468	
     *     }
469	
     * }
470	
     * </pre>
471	
     * @see #setGLWrapper(GLWrapper)
472	
     */
473	
    public interface GLWrapper {
474	
        /**
475	
         * Wraps a gl interface in another gl interface.
476	
         * @param gl a GL interface that is to be wrapped.
477	
         * @return either the input argument or another GL object that wraps the input argument.
478	
         */
479	
        GL wrap(GL gl);
480	
    }
481	
482	
    /**
483	
     * A generic renderer interface.
484	
     * <p>
485	
     * The renderer is responsible for making OpenGL calls to render a frame.
486	
     * <p>
487	
     * GLSurfaceView clients typically create their own classes that implement
488	
     * this interface, and then call {@link GLSurfaceView#setRenderer} to
489	
     * register the renderer with the GLSurfaceView.
490	
     * <p>
491	
     * <h3>Threading</h3>
492	
     * The renderer will be called on a separate thread, so that rendering
493	
     * performance is decoupled from the UI thread. Clients typically need to
494	
     * communicate with the renderer from the UI thread, because that's where
495	
     * input events are received. Clients can communicate using any of the
496	
     * standard Java techniques for cross-thread communication, or they can
497	
     * use the {@link GLSurfaceView#queueEvent(Runnable)} convenience method.
498	
     * <p>
499	
     * <h3>EGL Context Lost</h3>
500	
     * There are situations where the EGL rendering context will be lost. This
501	
     * typically happens when device wakes up after going to sleep. When
502	
     * the EGL context is lost, all OpenGL resources (such as textures) that are
503	
     * associated with that context will be automatically deleted. In order to
504	
     * keep rendering correctly, a renderer must recreate any lost resources
505	
     * that it still needs. The {@link #onSurfaceCreated(GL10, EGLConfig)} method
506	
     * is a convenient place to do this.
507	
     *
508	
     *
509	
     * @see #setRenderer(Renderer)
510	
     */
511	
    public interface Renderer {
512	
        /**
513	
         * Called when the surface is created or recreated.
514	
         * <p>
515	
         * Called when the rendering thread
516	
         * starts and whenever the EGL context is lost. The context will typically
517	
         * be lost when the Android device awakes after going to sleep.
518	
         * <p>
519	
         * Since this method is called at the beginning of rendering, as well as
520	
         * every time the EGL context is lost, this method is a convenient place to put
521	
         * code to create resources that need to be created when the rendering
522	
         * starts, and that need to be recreated when the EGL context is lost.
523	
         * Textures are an example of a resource that you might want to create
524	
         * here.
525	
         * <p>
526	
         * Note that when the EGL context is lost, all OpenGL resources associated
527	
         * with that context will be automatically deleted. You do not need to call
528	
         * the corresponding "glDelete" methods such as glDeleteTextures to
529	
         * manually delete these lost resources.
530	
         * <p>
531	
         * @param gl the GL interface. Use <code>instanceof</code> to
532	
         * test if the interface supports GL11 or higher interfaces.
533	
         * @param config the EGLConfig of the created surface. Can be used
534	
         * to create matching pbuffers.
535	
         */
536	
        void onSurfaceCreated(GL10 gl, EGLConfig config);
537	
538	
        /**
539	
         * Called when the surface changed size.
540	
         * <p>
541	
         * Called after the surface is created and whenever
542	
         * the OpenGL ES surface size changes.
543	
         * <p>
544	
         * Typically you will set your viewport here. If your camera
545	
         * is fixed then you could also set your projection matrix here:
546	
         * <pre class="prettyprint">
547	
         * void onSurfaceChanged(GL10 gl, int width, int height) {
548	
         *     gl.glViewport(0, 0, width, height);
549	
         *     // for a fixed camera, set the projection too
550	
         *     float ratio = (float) width / height;
551	
         *     gl.glMatrixMode(GL10.GL_PROJECTION);
552	
         *     gl.glLoadIdentity();
553	
         *     gl.glFrustumf(-ratio, ratio, -1, 1, 1, 10);
554	
         * }
555	
         * </pre>
556	
         * @param gl the GL interface. Use <code>instanceof</code> to
557	
         * test if the interface supports GL11 or higher interfaces.
558	
         * @param width
559	
         * @param height
560	
         */
561	
        void onSurfaceChanged(GL10 gl, int width, int height);
562	
563	
        /**
564	
         * Called to draw the current frame.
565	
         * <p>
566	
         * This method is responsible for drawing the current frame.
567	
         * <p>
568	
         * The implementation of this method typically looks like this:
569	
         * <pre class="prettyprint">
570	
         * void onDrawFrame(GL10 gl) {
571	
         *     gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
572	
         *     //... other gl calls to render the scene ...
573	
         * }
574	
         * </pre>
575	
         * @param gl the GL interface. Use <code>instanceof</code> to
576	
         * test if the interface supports GL11 or higher interfaces.
577	
         */
578	
        void onDrawFrame(GL10 gl);
579	
    }
580	
581	
    /**
582	
     * An interface for choosing an EGLConfig configuration from a list of
583	
     * potential configurations.
584	
     * <p>
585	
     * This interface must be implemented by clients wishing to call
586	
     * {@link GLSurfaceView#setEGLConfigChooser(EGLConfigChooser)}
587	
     */
588	
    public interface EGLConfigChooser {
589	
        /**
590	
         * Choose a configuration from the list. Implementors typically
591	
         * implement this method by calling
592	
         * {@link EGL10#eglChooseConfig} and iterating through the results. Please consult the
593	
         * EGL specification available from The Khronos Group to learn how to call eglChooseConfig.
594	
         * @param egl the EGL10 for the current display.
595	
         * @param display the current display.
596	
         * @return the chosen configuration.
597	
         */
598	
        EGLConfig chooseConfig(EGL10 egl, EGLDisplay display);
599	
    }
600	
601	
    private static abstract class BaseConfigChooser
602	
            implements EGLConfigChooser {
603	
        public BaseConfigChooser(int[] configSpec) {
604	
            mConfigSpec = configSpec;
605	
        }
606	
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
607	
            int[] num_config = new int[1];
608	
            egl.eglChooseConfig(display, mConfigSpec, null, 0, num_config);
609	
610	
            int numConfigs = num_config[0];
611	
612	
            if (numConfigs <= 0) {
613	
                throw new IllegalArgumentException(
614	
                        "No configs match configSpec");
615	
            }
616	
617	
            EGLConfig[] configs = new EGLConfig[numConfigs];
618	
            egl.eglChooseConfig(display, mConfigSpec, configs, numConfigs,
619	
                    num_config);
620	
            EGLConfig config = chooseConfig(egl, display, configs);
621	
            if (config == null) {
622	
                throw new IllegalArgumentException("No config chosen");
623	
            }
624	
            return config;
625	
        }
626	
627	
        abstract EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
628	
                EGLConfig[] configs);
629	
630	
        protected int[] mConfigSpec;
631	
    }
632	
633	
    private static class ComponentSizeChooser extends BaseConfigChooser {
634	
        public ComponentSizeChooser(int redSize, int greenSize, int blueSize,
635	
                int alphaSize, int depthSize, int stencilSize) {
636	
            super(new int[] {
637	
                    EGL10.EGL_RED_SIZE, redSize,
638	
                    EGL10.EGL_GREEN_SIZE, greenSize,
639	
                    EGL10.EGL_BLUE_SIZE, blueSize,
640	
                    EGL10.EGL_ALPHA_SIZE, alphaSize,
641	
                    EGL10.EGL_DEPTH_SIZE, depthSize,
642	
                    EGL10.EGL_STENCIL_SIZE, stencilSize,
643	
                    EGL10.EGL_NONE});
644	
            mValue = new int[1];
645	
            mRedSize = redSize;
646	
            mGreenSize = greenSize;
647	
            mBlueSize = blueSize;
648	
            mAlphaSize = alphaSize;
649	
            mDepthSize = depthSize;
650	
            mStencilSize = stencilSize;
651	
       }
652	
653	
        @Override
654	
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
655	
                EGLConfig[] configs) {
656	
            EGLConfig closestConfig = null;
657	
            int closestDistance = 1000;
658	
            for(EGLConfig config : configs) {
659	
                int r = findConfigAttrib(egl, display, config,
660	
                        EGL10.EGL_RED_SIZE, 0);
661	
                int g = findConfigAttrib(egl, display, config,
662	
                         EGL10.EGL_GREEN_SIZE, 0);
663	
                int b = findConfigAttrib(egl, display, config,
664	
                          EGL10.EGL_BLUE_SIZE, 0);
665	
                int a = findConfigAttrib(egl, display, config,
666	
                        EGL10.EGL_ALPHA_SIZE, 0);
667	
                int d = findConfigAttrib(egl, display, config,
668	
                        EGL10.EGL_DEPTH_SIZE, 0);
669	
                int s = findConfigAttrib(egl, display, config,
670	
                        EGL10.EGL_STENCIL_SIZE, 0);
671	
                int distance = Math.abs(r - mRedSize)
672	
                    + Math.abs(g - mGreenSize)
673	
                    + Math.abs(b - mBlueSize) + Math.abs(a - mAlphaSize)
674	
                    + Math.abs(d - mDepthSize) + Math.abs(s - mStencilSize);
675	
                if (distance < closestDistance) {
676	
                    closestDistance = distance;
677	
                    closestConfig = config;
678	
                }
679	
            }
680	
            return closestConfig;
681	
        }
682	
683	
        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
684	
                EGLConfig config, int attribute, int defaultValue) {
685	
686	
            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
687	
                return mValue[0];
688	
            }
689	
            return defaultValue;
690	
        }
691	
692	
        private int[] mValue;
693	
        // Subclasses can adjust these values:
694	
        protected int mRedSize;
695	
        protected int mGreenSize;
696	
        protected int mBlueSize;
697	
        protected int mAlphaSize;
698	
        protected int mDepthSize;
699	
        protected int mStencilSize;
700	
        }
701	
702	
    /**
703	
     * This class will choose a supported surface as close to
704	
     * RGB565 as possible, with or without a depth buffer.
705	
     *
706	
     */
707	
    private static class SimpleEGLConfigChooser extends ComponentSizeChooser {
708	
        public SimpleEGLConfigChooser(boolean withDepthBuffer) {
709	
            super(4, 4, 4, 0, withDepthBuffer ? 16 : 0, 0);
710	
            // Adjust target values. This way we'll accept a 4444 or
711	
            // 555 buffer if there's no 565 buffer available.
712	
            mRedSize = 5;
713	
            mGreenSize = 6;
714	
            mBlueSize = 5;
715	
        }
716	
    }
717	
718	
    /**
719	
     * An EGL helper class.
720	
     */
721	
722	
    private class EglHelper {
723	
        public EglHelper() {
724	
725	
        }
726	
727	
        /**
728	
         * Initialize EGL for a given configuration spec.
729	
         * @param configSpec
730	
         */
731	
        public void start(){
732	
            /*
733	
             * Get an EGL instance
734	
             */
735	
            mEgl = (EGL10) EGLContext.getEGL();
736	
737	
            /*
738	
             * Get to the default display.
739	
             */
740	
            mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
741	
742	
            /*
743	
             * We can now initialize EGL for that display
744	
             */
745	
            int[] version = new int[2];
746	
            mEgl.eglInitialize(mEglDisplay, version);
747	
            mEglConfig = mEGLConfigChooser.chooseConfig(mEgl, mEglDisplay);
748	
749	
            /*
750	
            * Create an OpenGL ES context. This must be done only once, an
751	
            * OpenGL context is a somewhat heavy object.
752	
            */
753	
            mEglContext = mEgl.eglCreateContext(mEglDisplay, mEglConfig,
754	
                    EGL10.EGL_NO_CONTEXT, null);
755	
756	
            mEglSurface = null;
757	
        }
758	
759	
        /*
760	
         * React to the creation of a new surface by creating and returning an
761	
         * OpenGL interface that renders to that surface.
762	
         */
763	
        public GL createSurface(SurfaceHolder holder) {
764	
            /*
765	
             *  The window size has changed, so we need to create a new
766	
             *  surface.
767	
             */
768	
            if (mEglSurface != null) {
769	
770	
                /*
771	
                 * Unbind and destroy the old EGL surface, if
772	
                 * there is one.
773	
                 */
774	
                mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
775	
                        EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
776	
                mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
777	
            }
778	
779	
            /*
780	
             * Create an EGL surface we can render into.
781	
             */
782	
            mEglSurface = mEgl.eglCreateWindowSurface(mEglDisplay,
783	
                    mEglConfig, holder, null);
784	
785	
            /*
786	
             * Before we can issue GL commands, we need to make sure
787	
             * the context is current and bound to a surface.
788	
             */
789	
            mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
790	
                    mEglContext);
791	
792	
793	
            GL gl = mEglContext.getGL();
794	
            if (mGLWrapper != null) {
795	
                gl = mGLWrapper.wrap(gl);
796	
            }
797	
798	
            if ((mDebugFlags & (DEBUG_CHECK_GL_ERROR | DEBUG_LOG_GL_CALLS))!= 0) {
799	
                int configFlags = 0;
800	
                Writer log = null;
801	
                if ((mDebugFlags & DEBUG_CHECK_GL_ERROR) != 0) {
802	
                    configFlags |= GLDebugHelper.CONFIG_CHECK_GL_ERROR;
803	
                }
804	
                if ((mDebugFlags & DEBUG_LOG_GL_CALLS) != 0) {
805	
                    log = new LogWriter();
806	
                }
807	
                gl = GLDebugHelper.wrap(gl, configFlags, log);
808	
            }
809	
            return gl;
810	
        }
811	
812	
        /**
813	
         * Display the current render surface.
814	
         * @return false if the context has been lost.
815	
         */
816	
        public boolean swap() {
817	
            mEgl.eglSwapBuffers(mEglDisplay, mEglSurface);
818	
819	
            /*
820	
             * Always check for EGL_CONTEXT_LOST, which means the context
821	
             * and all associated data were lost (For instance because
822	
             * the device went to sleep). We need to sleep until we
823	
             * get a new surface.
824	
             */
825	
            return mEgl.eglGetError() != EGL11.EGL_CONTEXT_LOST;
826	
        }
827	
828	
        public void finish() {
829	
            if (mEglSurface != null) {
830	
                mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
831	
                        EGL10.EGL_NO_SURFACE,
832	
                        EGL10.EGL_NO_CONTEXT);
833	
                mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
834	
                mEglSurface = null;
835	
            }
836	
            if (mEglContext != null) {
837	
                mEgl.eglDestroyContext(mEglDisplay, mEglContext);
838	
                mEglContext = null;
839	
            }
840	
            if (mEglDisplay != null) {
841	
                mEgl.eglTerminate(mEglDisplay);
842	
                mEglDisplay = null;
843	
            }
844	
        }
845	
846	
        EGL10 mEgl;
847	
        EGLDisplay mEglDisplay;
848	
        EGLSurface mEglSurface;
849	
        EGLConfig mEglConfig;
850	
        EGLContext mEglContext;
851	
    }
852	
853	
    /**
854	
     * A generic GL Thread. Takes care of initializing EGL and GL. Delegates
855	
     * to a Renderer instance to do the actual drawing. Can be configured to
856	
     * render continuously or on request.
857	
     *
858	
     */
859	
    class GLThread extends Thread {
860	
        GLThread(Renderer renderer) {
861	
            super();
862	
            mDone = false;
863	
            mWidth = 0;
864	
            mHeight = 0;
865	
            mRequestRender = true;
866	
            mRenderMode = RENDERMODE_CONTINUOUSLY;
867	
            mRenderer = renderer;
868	
            setName("GLThread");
869	
        }
870	
871	
        @Override
872	
        public void run() {
873	
            /*
874	
             * When the android framework launches a second instance of
875	
             * an activity, the new instance's onCreate() method may be
876	
             * called before the first instance returns from onDestroy().
877	
             *
878	
             * This semaphore ensures that only one instance at a time
879	
             * accesses EGL.
880	
             */
881	
            try {
882	
                try {
883	
                sEglSemaphore.acquire();
884	
                } catch (InterruptedException e) {
885	
                    return;
886	
                }
887	
                guardedRun();
888	
            } catch (InterruptedException e) {
889	
                // fall thru and exit normally
890	
            } finally {
891	
                sEglSemaphore.release();
892	
            }
893	
        }
894	
895	
        private void guardedRun() throws InterruptedException {
896	
            mEglHelper = new EglHelper();
897	
            mEglHelper.start();
898	
899	
            GL10 gl = null;
900	
            boolean tellRendererSurfaceCreated = true;
901	
            boolean tellRendererSurfaceChanged = true;
902	
903	
            /*
904	
             * This is our main activity thread's loop, we go until
905	
             * asked to quit.
906	
             */
907	
            while (!mDone) {
908	
909	
                /*
910	
                 *  Update the asynchronous state (window size)
911	
                 */
912	
                int w, h;
913	
                boolean changed;
914	
                boolean needStart = false;
915	
                synchronized (this) {
916	
                    Runnable r;
917	
                    while ((r = getEvent()) != null) {
918	
                        r.run();
919	
                    }
920	
                    if (mPaused) {
921	
                        mEglHelper.finish();
922	
                        needStart = true;
923	
                    }
924	
                    while (needToWait()) {
925	
                        wait();
926	
                    }
927	
                    if (mDone) {
928	
                        break;
929	
                    }
930	
                    changed = mSizeChanged;
931	
                    w = mWidth;
932	
                    h = mHeight;
933	
                    mSizeChanged = false;
934	
                    mRequestRender = false;
935	
                }
936	
                if (needStart) {
937	
                    mEglHelper.start();
938	
                    tellRendererSurfaceCreated = true;
939	
                    changed = true;
940	
                }
941	
                if (changed) {
942	
                    gl = (GL10) mEglHelper.createSurface(getHolder());
943	
                    tellRendererSurfaceChanged = true;
944	
                }
945	
                if (tellRendererSurfaceCreated) {
946	
                    mRenderer.onSurfaceCreated(gl, mEglHelper.mEglConfig);
947	
                    tellRendererSurfaceCreated = false;
948	
                }
949	
                if (tellRendererSurfaceChanged) {
950	
                    mRenderer.onSurfaceChanged(gl, w, h);
951	
                    tellRendererSurfaceChanged = false;
952	
                }
953	
                if ((w > 0) && (h > 0)) {
954	
                    /* draw a frame here */
955	
                    mRenderer.onDrawFrame(gl);
956	
957	
                    /*
958	
                     * Once we're done with GL, we need to call swapBuffers()
959	
                     * to instruct the system to display the rendered frame
960	
                     */
961	
                    mEglHelper.swap();
962	
                }
963	
             }
964	
965	
            /*
966	
             * clean-up everything...
967	
             */
968	
            mEglHelper.finish();
969	
        }
970	
971	
        private boolean needToWait() {
972	
            if (mDone) {
973	
                return false;
974	
            }
975	
976	
            if (mPaused || (! mHasSurface)) {
977	
                return true;
978	
            }
979	
980	
            if ((mWidth > 0) && (mHeight > 0) && (mRequestRender || (mRenderMode == RENDERMODE_CONTINUOUSLY))) {
981	
                return false;
982	
            }
983	
984	
            return true;
985	
        }
986	
987	
        public void setRenderMode(int renderMode) {
988	
            if ( !((RENDERMODE_WHEN_DIRTY <= renderMode) && (renderMode <= RENDERMODE_CONTINUOUSLY)) ) {
989	
                throw new IllegalArgumentException("renderMode");
990	
            }
991	
            synchronized(this) {
992	
                mRenderMode = renderMode;
993	
                if (renderMode == RENDERMODE_CONTINUOUSLY) {
994	
                    notify();
995	
                }
996	
            }
997	
        }
998	
999	
        public int getRenderMode() {
1000	
            synchronized(this) {
1001	
                return mRenderMode;
1002	
            }
1003	
        }
1004	
1005	
        public void requestRender() {
1006	
            synchronized(this) {
1007	
                mRequestRender = true;
1008	
                notify();
1009	
            }
1010	
        }
1011	
1012	
        public void surfaceCreated() {
1013	
            synchronized(this) {
1014	
                mHasSurface = true;
1015	
                notify();
1016	
            }
1017	
        }
1018	
1019	
        public void surfaceDestroyed() {
1020	
            synchronized(this) {
1021	
                mHasSurface = false;
1022	
                notify();
1023	
            }
1024	
        }
1025	
1026	
        public void onPause() {
1027	
            synchronized (this) {
1028	
                mPaused = true;
1029	
            }
1030	
        }
1031	
1032	
        public void onResume() {
1033	
            synchronized (this) {
1034	
                mPaused = false;
1035	
                notify();
1036	
            }
1037	
        }
1038	
1039	
        public void onWindowResize(int w, int h) {
1040	
            synchronized (this) {
1041	
                mWidth = w;
1042	
                mHeight = h;
1043	
                mSizeChanged = true;
1044	
                notify();
1045	
            }
1046	
        }
1047	
1048	
        public void requestExitAndWait() {
1049	
            // don't call this from GLThread thread or it is a guaranteed
1050	
            // deadlock!
1051	
            synchronized(this) {
1052	
                mDone = true;
1053	
                notify();
1054	
            }
1055	
            try {
1056	
                join();
1057	
            } catch (InterruptedException ex) {
1058	
                Thread.currentThread().interrupt();
1059	
            }
1060	
        }
1061	
1062	
        /**
1063	
         * Queue an "event" to be run on the GL rendering thread.
1064	
         * @param r the runnable to be run on the GL rendering thread.
1065	
         */
1066	
        public void queueEvent(Runnable r) {
1067	
            synchronized(this) {
1068	
                mEventQueue.add(r);
1069	
            }
1070	
        }
1071	
1072	
        private Runnable getEvent() {
1073	
            synchronized(this) {
1074	
                if (mEventQueue.size() > 0) {
1075	
                    return mEventQueue.remove(0);
1076	
                }
1077	
1078	
            }
1079	
            return null;
1080	
        }
1081	
1082	
        private boolean mDone;
1083	
        private boolean mPaused;
1084	
        private boolean mHasSurface;
1085	
        private int mWidth;
1086	
        private int mHeight;
1087	
        private int mRenderMode;
1088	
        private boolean mRequestRender;
1089	
        private Renderer mRenderer;
1090	
        private ArrayList<Runnable> mEventQueue = new ArrayList<Runnable>();
1091	
        private EglHelper mEglHelper;
1092	
    }
1093	
1094	
    static class LogWriter extends Writer {
1095	
1096	
        @Override public void close() {
1097	
            flushBuilder();
1098	
        }
1099	
1100	
        @Override public void flush() {
1101	
            flushBuilder();
1102	
        }
1103	
1104	
        @Override public void write(char[] buf, int offset, int count) {
1105	
            for(int i = 0; i < count; i++) {
1106	
                char c = buf[offset + i];
1107	
                if ( c == '\n') {
1108	
                    flushBuilder();
1109	
                }
1110	
                else {
1111	
                    mBuilder.append(c);
1112	
                }
1113	
            }
1114	
        }
1115	
1116	
        private void flushBuilder() {
1117	
            if (mBuilder.length() > 0) {
1118	
                Log.v("GLSurfaceView", mBuilder.toString());
1119	
                mBuilder.delete(0, mBuilder.length());
1120	
            }
1121	
        }
1122	
1123	
        private StringBuilder mBuilder = new StringBuilder();
1124	
    }
1125	
1126	
    private static final Semaphore sEglSemaphore = new Semaphore(1);
1127	
    private boolean mSizeChanged = true;
1128	
1129	
    private GLThread mGLThread;
1130	
    private EGLConfigChooser mEGLConfigChooser;
1131	
    private GLWrapper mGLWrapper;
1132	
    private int mDebugFlags;
1133	
}
http://gitorious.org/0xdroid/frameworks_base/blobs/93f411386a570082f25996cb373bc338c6beee7a/opengl/java/android/opengl/GLSurfaceView.java