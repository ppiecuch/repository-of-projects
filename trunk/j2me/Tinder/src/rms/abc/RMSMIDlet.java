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
    }	//�Բ���createIfNecessary=true���õ�ʱ���׳�RecordStoreNotFoundException�쳣��
   catch(RecordStoreFullException e) {					//�洢ϵͳ�����쳣
     System.out.println(e);
   }catch(RecordStoreException e) {						//�洢ϵͳ�쳣
     System.out.println(e);
   }
  catch(IllegalArgumentException e)	{					//���������쳣
     System.out.println(e);
   }	//����ֻ����һ���쳣catch(Exception e){}��������ӡ�쳣��ջҲ����֪��������쳣��
  try{
     recordStore.closeRecordStore();		//�رմ򿪵ļ�¼
     RecordStore.deleteRecordStore("test");	//ɾ��ǰ�洴������Ϊtest�ļ�¼
   }catch(RecordStoreNotOpenException e) { 	//��¼û�д򿪵�����¹ر�
     System.out.println(e);
   }catch(RecordStoreException e) {			//��¼û�д򿪵������ɾ���������洢ϵͳ�쳣
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
