package sound.abc;




import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.media.*;
import java.io.*;
/**
 * MIDlet application that displays the M3G samples used in the 
 * article. Each sample implements <code>Sample</code> and provides 
 * the code to demonstrate a specific feature of the M3G API. The user
 * interface consists of a list of such samples that can be selected 
 * for execution. 
 *
 * @author Redhair 
 */
public class MMAPIMIDlet extends MIDlet 
{


   

  

 
  
  /**
   * Starts application and shows the list of samples. The user can 
   * select a sample for execution and return to the selection screen.
   */
  public void startApp()
  {
  	String []contentType=Manager.getSupportedContentTypes(null);			//获得支持的媒体类型格式
    for(int i = 0;i<contentType.length;i++)
    {
    	 System.out.print (contentType[i]+"  ");					//控制台输出媒体类型格式
 			 String[] protocol = Manager.getSupportedProtocols(contentType[i]);	//获得指定媒体类型的协议方式
 		for(int j= 0;j<protocol.length;j++)
 	  {
 	     System.out.print(protocol[j]+",");					//控制台输出获取该类型的协议方式
	  }
		   System.out.println();
		}
		try{
      InputStream is = this.getClass().getResourceAsStream("/mmapi.wav");	//读取资源
      Player player = Manager.createPlayer(is,"audio/x-wav");			//创建Player
      player.start();											//播放媒体
    }catch(Exception e){
      System.out.println("error");
    }

    try{
      Manager.playTone(100,1000,100);	//播放音调，建议播放时间长一些，不然不容易听到
     }catch(Exception e) {
      System.out.println(e);			//捕获异常，如果设备不能播放声音抛出MediaException异常
    }

//      destroyApp(true);
//      notifyDestroyed();
 
  }
  
  /**
   * Not used.
   */
  public void pauseApp()
  {
  }
  
  /**
   * Not used.
   */
  public void destroyApp(boolean unconditional)
  {
  }
  


}
