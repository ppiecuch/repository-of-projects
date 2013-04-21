package core;

import java.awt.AWTException;
import java.awt.Image;
import java.awt.SystemTray;
import java.awt.Toolkit;
import java.awt.TrayIcon;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;

public class Tray {

	private static Tray instance = null;
	private TrayIcon ti;
	
	private Tray(){
		SystemTray tray = SystemTray.getSystemTray();
		InputStream is=this.getClass().getResourceAsStream("/yon.gif");
		try {
			Image image = ImageIO.read(is);
			//Image image  = Toolkit.getDefaultToolkit().getImage(this.getClass().getResource("/icon48.png"));
			ti= new TrayIcon(image);
			tray.add(ti);
		} catch (AWTException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static Tray getInstance() {
		if(instance==null)
			instance=new Tray();
		return instance;
	}
	
	public void tip(String title,String content){
		ti.displayMessage(title,content,TrayIcon.MessageType.INFO);
	}
	
	public void tip(String content){
		tip(null,content);
	}
}
