package rms.abc;




import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
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
public class RMSMIDlet extends MIDlet 
{


   private RecordStore recordStore; 

  

 
  
  /**
   * Starts application and shows the list of samples. The user can 
   * select a sample for execution and return to the selection screen.
   */
  public void startApp()
  {   
   try{
      recordStore = RecordStore.openRecordStore("test",true,RecordStore.AUTHMODE_PRIVATE,true);
    }	//以参数createIfNecessary=true调用的时候不抛出RecordStoreNotFoundException异常。
   catch(RecordStoreFullException e) {					//存储系统已满异常
     System.out.println(e);
   }catch(RecordStoreException e) {						//存储系统异常
     System.out.println(e);
   }
  catch(IllegalArgumentException e)	{					//参数错误异常
     System.out.println(e);
   }	//可以只处理一个异常catch(Exception e){}。这样打印异常堆栈也可以知道具体的异常。
  try{
     recordStore.closeRecordStore();		//关闭打开的纪录
     RecordStore.deleteRecordStore("test");	//删除前面创建的名为test的记录
   }catch(RecordStoreNotOpenException e) { 	//记录没有打开的情况下关闭
     System.out.println(e);
   }catch(RecordStoreException e) {			//记录没有打开的情况下删除及其他存储系统异常
    System.out.println(e);
   }

  System.out.println("execute over");
      destroyApp(true);
      notifyDestroyed();
 
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
