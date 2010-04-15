import java.util.Date;






public class Demo {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		long st=new Date().getTime();
		QuadtreeNode root=new QuadtreeNode(6);
		System.out.println(new Date().getTime()-st);
	}
}
