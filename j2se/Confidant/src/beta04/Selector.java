package beta04;

import java.awt.AWTException;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Toolkit;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;
import javax.swing.JFrame;

public abstract class Selector extends JFrame{
	private static final long serialVersionUID = 1L;
	int orgx, orgy, endx, endy;
	Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
	BufferedImage image;
	BufferedImage tempImage;
	BufferedImage saveImage;
	Graphics g;

	@Override
	public void paint(Graphics g) {
		RescaleOp ro = new RescaleOp(0.8f, 0, null);
		tempImage = ro.filter(image, null);
		g.drawImage(tempImage, 0, 0, this);
	}

	public Selector() {
		snapshot();
		setVisible(true);
		// setSize(d);//最大化窗口
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent e) {
				orgx = e.getX();
				orgy = e.getY();
			}
		});
		this.addMouseMotionListener(new MouseMotionAdapter() {
			public void mouseDragged(MouseEvent e) {
				endx = e.getX();
				endy = e.getY();
				g = getGraphics();
				g.drawImage(tempImage, 0, 0, Selector.this);
				int x = Math.min(orgx, endx);
				int y = Math.min(orgy, endy);
				int width = Math.abs(endx - orgx)+1;
				int height = Math.abs(endy - orgy)+1;
				// 加上1，防止width或height为0
				g.setColor(Color.BLUE);
				g.drawRect(x-1, y-1, width+1, height+1);
				//减1，加1都是为了防止图片将矩形框覆盖掉
				saveImage = image.getSubimage(x, y, width, height);
				g.drawImage(saveImage, x, y, Selector.this);
			}
		});
		this.addKeyListener(new KeyAdapter() {
			@Override
			public void keyReleased(KeyEvent e) {
				// 按Esc键退出
				if (e.getKeyCode() == 27 || e.getKeyCode() == 13 || e.getKeyCode() == 32) {
					Selector.this.dispose();
					Rect rect=new Rect();
					rect.topLeftX=Selector.this.orgx;
					rect.topLeftY=Selector.this.orgy;
					rect.bottomRightX=Selector.this.endx;
					rect.bottomRightY=Selector.this.endy;
					doSelected(rect);
				}
			}
		});
	}
	
	public abstract void doSelected(Rect rect);

	public void snapshot() {
		try {
			Robot robot = new Robot();
			Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
			image = robot.createScreenCapture(new Rectangle(0, 0, d.width, d.height));
		} catch (AWTException e) {
			e.printStackTrace();
		}
	}

}
