import java.awt.*;
import java.applet.Applet;

public class WaterWave2D extends Applet implements Runnable{
 int XMAX=112;
 float[] m_Height=new float[XMAX];
 float[] m_G=new float[XMAX];
 float b=1.01f;
 float fWeight=3;
 private Thread m_NAMI2D=null;
 public boolean mouseDown(Event event,int i,int j){
  m_Height[i/4-1]+=j/2;
  m_Height[i/4]+=j;
  m_Height[i/4+1]+=j/2;
  return false;
 }
 public void paint(Graphics g){
  g.setColor(Color.blue);
  float f1=3F*fWeight;
  float f2=f1-2.0F;
  m_Height[0]=m_Height[1];
  m_Height[XMAX-1]=m_Height[XMAX-2];
  float f=m_Height[0];
  for(int i=1;i<XMAX-1;i++){
   float f3=((f+f2*m_Height[i]+m_Height[i+1])/f1+m_G[i])/b;
   f=m_Height[i];
   m_G[i]=f3-f;
   m_Height[i]=f3;
   g.clearRect((i-1)*4,(int)f3-30,4,60);
   g.fillRect((i-1)*4,(int)f3+30,4,60);
  }
 }
 public void run(){
  do 
      try{
       repaint();
       Thread.sleep(50L);
          }
  catch(InterruptedException _ex){
   stop();
  }
     while(true);
 }
 public void start(){
  if(m_NAMI2D==null){
   m_NAMI2D=new Thread(this);
   m_NAMI2D.start();
   }
 }
 public void stop(){
  if(m_NAMI2D!=null){
   m_NAMI2D.stop();
   m_NAMI2D=null;
  }
 }
 public void update(Graphics g){
	  paint(g);
	 }
 
}