package immediate;

/* * MyCanvas.java * * Created on 2008年2月16日, 下午1:15 * * To change this template, choose Tools | Template Manager * and open the template in the editor. */
import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;

/**
 * * *
 * 
 * @author Administrator
 */
public class MyCanvas extends Canvas {
	private Graphics3D iG3D;

	private Camera iCamera;

	private Light iLight;

	private float iAngle = 0.0f;

	private Transform iTransform = new Transform();

	private Background iBackground = new Background();

	private VertexBuffer iVb;

	private IndexBuffer iIb;

	private Appearance iAppearance;

	private Material iMaterial = new Material();

	private Image iImage;

	/**
	 * Creates a new instance of MyCanvas
	 */
	public MyCanvas() {
		setCommandListener(new CommandListener() {
			public void commandAction(Command c, Displayable d) {
				if (c.getCommandType() == Command.EXIT) {
					MIDletMain.quitApp();
				}
			}
		});
		try {
			init();
		} catch (Exception e) {
			e.printStackTrace();
			MIDletMain.quitApp();
		}
	}

	public void init() throws Exception {
		addCommand(new Command("Exit", Command.EXIT, 1));
		this.setFullScreenMode(true);
		iG3D = Graphics3D.getInstance();
		iCamera = new Camera();
		iCamera.setPerspective(60.0f, (float) getWidth() / (float) getHeight(),
				1.0f, 1000.0f);
		iLight = new Light();
		iLight.setColor(0xffffff);
		iLight.setIntensity(1.25f);
		short[] vert = { 10, 10, 10, -10, 10, 10, 10, -10, 10, -10, -10, 10, // 前面
				-10, 10, -10, 10, 10, -10, -10, -10, -10, 10, -10, -10, // 后面
				-10, 10, 10, -10, 10, -10, -10, -10, 10, -10, -10, -10, // 左面
				10, 10, -10, 10, 10, 10, 10, -10, -10, 10, -10, 10, // 右面
				10, 10, -10, -10, 10, -10, 10, 10, 10, -10, 10, 10, // 顶面
				10, -10, 10, -10, -10, 10, 10, -10, -10, -10, -10, -10 // 底面
		};
		VertexArray vertArray = new VertexArray(vert.length / 3, 3, 2);
		vertArray.set(0, vert.length / 3, vert);
		byte[] norm = { 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, -127,
				0, 0, -127, 0, 0, -127, 0, 0, -127, -127, 0, 0, -127, 0, 0,
				-127, 0, 0, -127, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, 127,
				0, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, -127, 0,
				0, -127, 0, 0, -127, 0, 0, -127, 0 };
		VertexArray normArray = new VertexArray(norm.length / 3, 3, 1);
		normArray.set(0, norm.length / 3, norm);
		short[] tex = { 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0,
				0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0,
				1, 1, 0, 0, 0, 1, 1, 0, 1, };
		VertexArray texArray = new VertexArray(tex.length / 2, 2, 2);
		texArray.set(0, tex.length / 2, tex);
		int[] stripLen = { 4, 4, 4, 4, 4, 4 };
		VertexBuffer vb = iVb = new VertexBuffer();
		vb.setPositions(vertArray, 1.0f, null);
		vb.setNormals(normArray);
		vb.setTexCoords(0, texArray, 1.0f, null);
		iIb = new TriangleStripArray(0, stripLen);
		iImage = Image.createImage("/wood.png");
		Image2D image2D = new Image2D(Image2D.RGB, iImage);
		Texture2D texture = new Texture2D(image2D);
		texture.setFiltering(Texture2D.FILTER_NEAREST,
						Texture2D.FILTER_NEAREST);
		texture.setWrapping(Texture2D.WRAP_CLAMP, Texture2D.WRAP_CLAMP);
		texture.setBlending(Texture2D.FUNC_MODULATE);
		iAppearance = new Appearance();
		iAppearance.setTexture(0, texture);
		iAppearance.setMaterial(iMaterial);
		iMaterial.setColor(Material.DIFFUSE, 0xFFFFFFFF);
		iMaterial.setColor(Material.SPECULAR, 0xFFFFFFFF);
		iMaterial.setShininess(100.0f);
		iBackground.setColor(0xf54588);
	}

	public void paint(Graphics g) {
		try {
			iG3D.bindTarget(g, true, Graphics3D.DITHER | Graphics3D.TRUE_COLOR);
			iG3D.clear(iBackground);
			Transform transform = new Transform();
			transform.postTranslate(0.0f, 0.0f, 30.0f);
			iG3D.setCamera(iCamera, transform);
			iG3D.resetLights();
			iG3D.addLight(iLight, transform);
			iAngle += 1.0f;
			iTransform.setIdentity();
			iTransform.postRotate(iAngle, 1.0f, 1.0f, 1.0f);
			iG3D.render(iVb, iIb, iAppearance, iTransform);
		} finally {
			iG3D.releaseTarget();
		}
	}
}