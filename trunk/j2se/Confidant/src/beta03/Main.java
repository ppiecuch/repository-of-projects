package beta03;

public class Main {
	
	private Mode menuMode;
	private Mode imitateMode;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}
	
	private void init(){
		/*��ʼ���˵�ģʽ*/
		initMenu();
		/*��ʼ��ģ��ģʽ*/
		
		/*����XML*/
		/*��ʼ��Executive*/
		/*���ò˵�ģʽ*/
	}
	
	private void run(){
		/*Executive����*/
		/*CommanderThread����*/
	}
	
	private void exit(){
		/*CommanderThread����*/
		/*Executive����*/
	}
	
	private void initMenu()
	{
		menuMode=new Mode() {
			
			public void start() {
				System.out.println("��ѡ��ģʽ��");
				System.out.println("NUM1:��¼ģʽ");
				System.out.println("NUM2:ģ��ģʽ");
				System.out.println("F12:�˳�ϵͳ");
			}
			
			public void doKeyPress(int keyNum) {
				switch(keyNum)
				{
				case Mode.NUM_1:
//					currentMode=recordMode;
//					recordMode.start();
					break;
				case Mode.NUM_2:
//					currentMode=imitateMode;
//					imitateMode.start();
					break;
				case Mode.F_12:
					System.out.println("==============�˳�ϵͳ==============");
					System.exit(0);
					break;
				default:
				}
			}

			public void doKeyReleased(int keyNum) {}
			public void doLeftPressed(int x, int y) {}
			public void doLeftReleased(int x, int y) {}
			public void doMiddlePressed(int x, int y) {}
			public void doMiddleReleased(int x, int y) {}
			public void doRightPressed(int x, int y) {}
			public void doRightReleased(int x, int y) {}
		};
	}
	
	private void initImitate()
	{
		imitateMode=new Mode() {
			
			public void start() {
			}
			
			public void doKeyPress(int keyNum) {}
			public void doKeyReleased(int keyNum) {}
			public void doLeftPressed(int x, int y) {}
			public void doLeftReleased(int x, int y) {}
			public void doMiddlePressed(int x, int y) {}
			public void doMiddleReleased(int x, int y) {}
			public void doRightPressed(int x, int y) {}
			public void doRightReleased(int x, int y) {}
		};
	}

}
