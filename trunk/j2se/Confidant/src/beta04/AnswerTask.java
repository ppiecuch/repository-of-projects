package beta04;

import java.awt.AWTException;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.event.InputEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;

import javax.imageio.ImageIO;

import core.Task;

public class AnswerTask implements Task {
	
	private static String questionsDir=null;
	private static String answersDir=null;
	private static HashMap<PixelImage, PixelImage> bank=new HashMap<PixelImage, PixelImage>();
	private static char OPTIONS[]={'A','B','C','D'};

	public static void setQuestionsDir(String questionsDir) {
		AnswerTask.questionsDir = questionsDir;
	}

	public static void setAnswersDir(String answersDir) {
		AnswerTask.answersDir = answersDir;
	}
	
	public static void init(){
		File queDir=new File(AnswerTask.questionsDir);
		File ansDir=new File(AnswerTask.answersDir);
		if(queDir.exists()==false)
			queDir.mkdirs();
		if(ansDir.exists()==false)
			ansDir.mkdirs();
		
		String[] quetions=queDir.list();
		String[] answers=ansDir.list();
		
		for(String q:quetions)
		{
			for(String a:answers)
			{
				if(q.equals(a) || (q+"_D").equals(a))
				{
					PixelImage qpi=new PixelImage(questionsDir,q);
					PixelImage api=new PixelImage(answersDir,a);
					bank.put(qpi, api);
					System.out.println("�����Ŀ��"+api);
				}
			}
		}
	}

	Rect questionRect;
	Rect optionRects[];
	Rect answerRects[];
	public AnswerTask(Rect questionRect,Rect optionRects[],Rect answerRects[]){
		this.questionRect=questionRect;
		this.optionRects=optionRects;
		this.answerRects=answerRects;
	}
	
	private void answer(Robot robot,PixelImage que) throws AWTException{
		PixelImage a=bank.get(que);
		
		PixelImage snaps[]=new PixelImage[4];
		Image imageA = robot.createScreenCapture(new Rectangle(optionRects[0].topLeftX, optionRects[0].topLeftY, optionRects[0].getWidth(),optionRects[0].getHeight()));
		snaps[0]=new PixelImage(imageA);
		
		Image imageB = robot.createScreenCapture(new Rectangle(optionRects[1].topLeftX, optionRects[1].topLeftY, optionRects[1].getWidth(),optionRects[1].getHeight()));
		snaps[1]=new PixelImage(imageB);
		
		Image imageC = robot.createScreenCapture(new Rectangle(optionRects[2].topLeftX, optionRects[2].topLeftY, optionRects[2].getWidth(),optionRects[2].getHeight()));
		snaps[2]=new PixelImage(imageC);
		
		Image imageD = robot.createScreenCapture(new Rectangle(optionRects[3].topLeftX, optionRects[3].topLeftY, optionRects[3].getWidth(),optionRects[3].getHeight()));
		snaps[3]=new PixelImage(imageD);
		
		for(int i=0;i<4; ++i)
		{
			if(a.equals(snaps[i]))
			{
				Point p=answerRects[i].getCenter();
				robot = new Robot();
				robot.mouseMove(p.x, p.y);
				robot.mousePress(InputEvent.BUTTON1_MASK);
				try {
					Thread.sleep(2);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				robot.mouseRelease(InputEvent.BUTTON1_MASK);
				System.out.println("�ҵ�ƥ����("+que+")������:"+OPTIONS[i]+"("+p.x+","+p.y+")");
				return;
			}
		}
		
		System.out.println("�Ҳ���ƥ����("+que+")�����𰸣�");
	}
	
	private String save(String dir,String name,Image image) throws IOException{
		// construct the buffered image
		BufferedImage bImage = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_INT_RGB);

		//obtain it's graphics
		Graphics2D bImageGraphics = bImage.createGraphics();

		//draw the Image (image) into the BufferedImage (bImage)
		bImageGraphics.drawImage(image, null, null);
		
		String result=name+".png";
		ImageIO.write(bImage, "png" , new File(dir,result));
		return result;
	}

	public void perform() {
		try {
			long start=System.currentTimeMillis();
			Robot robot = new Robot();
			//Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
			Image image = robot.createScreenCapture(new Rectangle(questionRect.topLeftX, questionRect.topLeftY, questionRect.getWidth(),questionRect.getHeight()));
			PixelImage snap=new PixelImage(image);
			for(PixelImage pi:bank.keySet())
			{
				if(pi.equals(snap))
				{
					answer(robot,pi);
					System.out.println("answer "+pi.getName()+", use time:"+(System.currentTimeMillis()-start));
					return;
				}
			}
			//�Ҳ�����Ŀ
			String qf=save(questionsDir,""+bank.size(),image);
			String af=null;
			
			for(int i=0;i<optionRects.length;++i)
			{
				image = robot.createScreenCapture(new Rectangle(optionRects[i].topLeftX, optionRects[i].topLeftY, optionRects[i].getWidth(),optionRects[i].getHeight()));
				af=save(answersDir,""+bank.size()+"_"+OPTIONS[i],image);
			}
			//Ĭ�����ΪD
			PixelImage qpi=new PixelImage(questionsDir,qf);
			PixelImage api=new PixelImage(answersDir,af);
			bank.put(qpi, api);
			System.out.println("�Ҳ���ƥ����Ŀ����������Ŀ("+qpi+"), use time:"+(System.currentTimeMillis()-start));
		} catch (AWTException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
	}

}
