package system;  

import javax.microedition.lcdui.Command;   
import javax.microedition.lcdui.CommandListener;   
import javax.microedition.lcdui.Display;   
import javax.microedition.lcdui.Displayable;   
import javax.microedition.lcdui.Form;   
import javax.microedition.midlet.MIDlet;   
 
/**
 * 原理：根据JSR特定的类来进行测试，以确定手机对JSR的支持情况 
 * 测试程序本身基于 CLDC1.0 MIDP1.0的最低要求
 * @author Administrator
 *
 */
public class JSRTest extends MIDlet implements Runnable,CommandListener{   
    Form form;   
    Thread thread;   
    Command c=new Command("Exit",Command.EXIT,0);   
    public JSRTest()   
    {   
        Display.getDisplay(this).setCurrent(form=new Form("JSR Test"));   
        form.addCommand(c);   
        form.setCommandListener(this);   
        (thread=new Thread(this)).start();   
    }   
    protected void destroyApp(boolean u){   
        super.notifyDestroyed();   
    }   
    protected void pauseApp() {   
    }   
    protected void startApp(){   
    }   
    public void run() {   
        checkJSR("MIDP2.0","javax.microedition.lcdui.game.GameCanvas");   
        checkJSR("CLDC1.1","java.lang.Float");   
        checkJSR("MMAPI","javax.microedition.media.Player");   
        checkJSR("WMAPI","javax.wireless.messaging.Message");   
        checkJSR("JSR75","javax.microedition.io.file.FileConnection");   
        checkJSR("JSR082","javax.bluetooth.UUID");   
        checkJSR("JSR179","javax.microedition.location.Location");   
        checkJSR("JSR180","javax.microedition.sip.SipConnection");   
        checkJSR("JSR184","javax.microedition.m3g.Mesh");   
        checkJSR("JSR211","javax.microedition.content.Registry");   
        checkJSR("JSR226","javax.microedition.m2g.SVGImage");   
        checkJSR("JSR229","javax.microedition.payment.TransactionRecord");   
        checkJSR("JSR234","javax.microedition.amms.Module");   
        checkJSR("JSR238","javax.microedition.global.Formatter");   
        checkJSR("JSR239","javax.microedition.khronos.egl.EGL");   
    }   
    private void checkJSR(String jsr,String className)   
    {   
        try {   
            Class.forName(className);   
            form.append(jsr+" Supproted\n");   
        } catch (ClassNotFoundException e) {   
            form.append(jsr+" Not Supproted\n");   
        }   
    }   
    public void commandAction(Command cmd, Displayable disp) {   
        this.destroyApp(false);   
    }   
}  
