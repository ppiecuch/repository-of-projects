package org.tinder.m3g.tutorial.abc01;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;
import javax.microedition.m3g.Appearance;
import javax.microedition.m3g.Camera;
import javax.microedition.m3g.Graphics3D;
import javax.microedition.m3g.IndexBuffer;
import javax.microedition.m3g.Mesh;
import javax.microedition.m3g.PolygonMode;
import javax.microedition.m3g.TriangleStripArray;
import javax.microedition.m3g.VertexArray;
import javax.microedition.m3g.VertexBuffer;
import javax.microedition.m3g.World;

public class M3GCanvas extends GameCanvas implements Runnable {
	
	public static final int FPS = 20;	//ÿ����Ƶ�֡��

	private Graphics3D g3d;
	private World world;
	private boolean runnable=true;
	private Thread thread;
	private Camera camera; // the camera in the scene
    private Mesh pyramidMesh; // the pyramid in the scene

	protected M3GCanvas() {
		super(false);
		setFullScreenMode(true);
		g3d = Graphics3D.getInstance();
		world = new World();
		
		camera = new Camera();
        world.addChild(camera); // add the camera to the world.

        float w = getWidth();
        float h = getHeight();

        // Constructs a perspective projection matrix and sets that as the current projection matrix.
        //setPerspective (float fovy, float aspectRatio, float near, float far)
        camera.setPerspective(60.0f, w / h, 0.1f, 50f);
        
        pyramidMesh = createpyramid(); // create our pyramid.
        
        //��������Z���ƶ�-4����λ
        pyramidMesh.setTranslation(0.0f, 0.0f, -4.0f);
        world.addChild(pyramidMesh); // add the pyramid to the world

        //Sets the Camera to use when rendering this World. 
        world.setActiveCamera(camera);
	}

	public void run() {
		Graphics g = getGraphics();
		while (runnable) {
			long startTime = System.currentTimeMillis();
			
			// rotate the pyramid 3 degree around the Y-axis.
			//postRotate(float angle,float ax,float ay,float az)
            pyramidMesh.postRotate(3.0f, 0.0f, 1.0f, 0.0f);
            
			try {
				g3d.bindTarget(g);
				g3d.render(world);
			} finally {
				g3d.releaseTarget();
			}
			flushGraphics();
			
			long endTime = System.currentTimeMillis();
            long costTime = endTime - startTime;
            if(costTime<1000/FPS)
            {
                try{
                  Thread.sleep(1000/FPS-costTime);
                }
                catch(Exception e){
                   e.printStackTrace();
                }
            }
		}
		System.out.println("Canvas stopped");

	}
	
	public void start()
	{
		thread=new Thread(this);
		thread.start();
	}
	
	public void stop()
	{
		this.runnable=false;
		try {
			thread.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	
	/**
	 * ����������
	 * @return
	 */
    private Mesh createpyramid(){
        
        /**
         * ��ɽ��������������
         */
        short[] points = new short[]{
        		-1, -1,  1,		//��ǰ
                 1, -1,  1,		//��ǰ
                 1, -1, -1,		//�Һ�
                -1, -1, -1,		//���
                 0,  1,  0		//��
        };
                                                            
                                                            
        /**
         * ������,ʹ����ʱ��ָ�������,������һ���ı���,�ֽ������������
         */
        int[] indices = new int[]{
        		0, 1, 4,		//ǰ
        		1, 2, 4,		//��
        		2, 3, 4,		//��
        		3, 0, 4,		//��
        		2, 1, 0,		//��1
        		2, 0, 3			//��2
        };
        /**
         * ����������ɫ
         */
        byte[] colors = new byte[]{
        		-1,  0,  0,   	//��ɫ
                 0, -1,  0,   	//��ɫ
                 0,  0, -1,   	//��ɫ
                -1,  0, -1,   	//��ɫ
                -1, -1,  0    	//��ɫ
        };
                                                        

        // The length of each sequence in the indices array.
        int[] length = new int[]{3, 3, 3, 3, 3, 3}; // the pyramid is built by six triangles
        
        VertexArray vertexArray, colorArray;
        IndexBuffer indexBuffer;
        
        //׼�������������ڴ������㻺��,short��Ϊ2���ֽ�
        //VertexArray(int numVertices, int numComponents, int componentSize)
        vertexArray = new VertexArray(points.length/3, 3, 2);
        //��������
        //set(int firstVertex, int numVertices, short[] values) 
        vertexArray.set(0, points.length/3, points);
        
        //ͬ��������ɫ����,byteΪ1���ֽ�
        colorArray = new VertexArray(colors.length/3, 3, 1);
        colorArray.set(0, colors.length / 3, colors);
        indexBuffer = new TriangleStripArray(indices, length);
        
        // VertexBuffer holds references to VertexArrays that contain the positions, colors, normals, 
        // and texture coordinates for a set of vertices
        VertexBuffer vertexBuffer = new VertexBuffer();
        vertexBuffer.setPositions(vertexArray, 1.0f, null);
        vertexBuffer.setColors(colorArray);
        
        // Create the 3D object defined as a polygonal surface
        //Constructs a new Mesh consisting of only one submesh. 
        //Mesh(VertexBuffer vertices, IndexBuffer submesh, Appearance appearance) 
        Mesh mesh = new Mesh(vertexBuffer, indexBuffer, null);
        
        //Appearance���ڶ���Mesh����Sprite3D����Ⱦ����,�ɻ�������������
        //ÿ�������������һϵ���໥�������������
        Appearance appearance = new Appearance(); // A set of component objects that define the rendering attributes of a Mesh
        PolygonMode polygonMode = new PolygonMode(); // An Appearance component encapsulating polygon-level attributes
        polygonMode.setPerspectiveCorrectionEnable(true);
        polygonMode.setWinding(PolygonMode.WINDING_CCW);//������ʱ��˳��Ϊ����
        polygonMode.setCulling(PolygonMode.CULL_BACK); // ��������,���ΪCULL_NONEֻ���涼��
        polygonMode.setShading(PolygonMode.SHADE_SMOOTH); //����ͶӰģʽΪ�⻬ģʽ,Ҳ��������Ϊƽ��ģʽSHADE_FLAT
        appearance.setPolygonMode(polygonMode);
        
        //setAppearance(int index, Appearance appearance)  Sets the Appearance for the specified submesh.
        mesh.setAppearance(0, appearance); // Set the appearance to the 3D object
        return mesh;
    }

}
