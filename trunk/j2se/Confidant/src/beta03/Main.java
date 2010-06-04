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
		/*初始化菜单模式*/
		initMenu();
		/*初始化模拟模式*/
		
		/*载入XML*/
		/*初始化Executive*/
		/*启用菜单模式*/
	}
	
	private void run(){
		/*Executive启动*/
		/*CommanderThread启动*/
	}
	
	private void exit(){
		/*CommanderThread销毁*/
		/*Executive销毁*/
	}
	
	private void initMenu()
	{
		menuMode=new Mode() {
			
			public void start() {
				System.out.println("请选择模式：");
				System.out.println("NUM1:记录模式");
				System.out.println("NUM2:模拟模式");
				System.out.println("F12:退出系统");
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
					System.out.println("==============退出系统==============");
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
