package org.tinder.m3g.tutorial.abc;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;
import javax.microedition.m3g.Graphics3D;
import javax.microedition.m3g.World;

public class M3GCanvas extends GameCanvas implements Runnable {
	
	public static final int FPS = 20;	//每秒绘制的帧数

	private Graphics3D g3d;
	private World world;
	private boolean runnable=true;
	private Thread thread;

	protected M3GCanvas() {
		// 不抑制按钮事件
		super(false);
		setFullScreenMode(true);
		g3d = Graphics3D.getInstance();
		world = new World();
	}

	public void run() {
		Graphics g = getGraphics();
		while (runnable) {
			long startTime = System.currentTimeMillis();
			
			// 处理游戏逻辑
			try {
				// Binds the given Graphics or mutable Image2D
				// as the rendering target of this Graphics3D
				g3d.bindTarget(g);
				g3d.render(world); // Render the world
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

}
