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
  	String []contentType=Manager.getSupportedContentTypes(null);			//���֧�ֵ�ý�����͸�ʽ
    for(int i = 0;i<contentType.length;i++)
    {
    	 System.out.print (contentType[i]+"  ");					//����̨���ý�����͸�ʽ
 			 String[] protocol = Manager.getSupportedProtocols(contentType[i]);	//���ָ��ý�����͵�Э�鷽ʽ
 		for(int j= 0;j<protocol.length;j++)
 	  {
 	     System.out.print(protocol[j]+",");					//����̨�����ȡ�����͵�Э�鷽ʽ
	  }
		   System.out.println();
		}
		try{
      InputStream is = this.getClass().getResourceAsStream("/mmapi.wav");	//��ȡ��Դ
      Player player = Manager.createPlayer(is,"audio/x-wav");			//����Player
      player.start();											//����ý��
    }catch(Exception e){
      System.out.println("error");
    }

    try{
      Manager.playTone(100,1000,100);	//�������������鲥��ʱ�䳤һЩ����Ȼ����������
     }catch(Exception e) {
      System.out.println(e);			//�����쳣������豸���ܲ��������׳�MediaException�쳣
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
