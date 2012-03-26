
public class LeftMatrix {

	public static String multiply(String a,String b){
		if(a.equals("0")||b.equals("0"))
			return "0";
		if(a.equals("1"))
			return b;
		if(b.equals("1"))
			return a;
		return a+"*"+b;
	}
	
	public static String add(String a,String b){
		if(a.equals("0"))
			return b;
		if(b.equals("0"))
			return a;
		return a+"+"+b;
	}
	
	public static String[][] multiply(String[][] a,String[][] b){
		String[][] m=new String[3][3];
		for(int i=0;i<3;++i)
			for(int j=0;j<3;++j){
				//m[j][i]=a[0][i]+"*"+b[j][0]+"+"+a[1][i]+"*"+b[j][1]+"+"+a[2][i]+"*"+b[j][2];
//				m[j][i]=multiply(a[0][i], b[j][0]);
//				m[j][i]=add(m[j][i],multiply(a[1][i], b[j][1]));
//				m[j][i]=add(m[j][i],multiply(a[2][i], b[j][2]));
				m[i][j]=multiply(a[i][0], b[0][j]);
				m[i][j]=add(m[i][j],multiply(a[i][1], b[1][j]));
				m[i][j]=add(m[i][j],multiply(a[i][2], b[2][j]));
			}
		return m;
	}
	
	public static void print(String[][] m){
		for(int i=0;i<3;++i){
			System.out.println(m[0][i]+","+m[1][i]+","+m[2][i]);
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		//ÐÐÓÅÏÈ
		String[][] mx={
				{"1","0","0"},
				{"0","ca","sa"},
				{"0","-sa","ca"}
				};
		String[][] my={
				{"cb","0","-sb"},
				{"0","1","0"},
				{"sb","0","cb"}
				};
		String[][] mz={
				{"cc","sc","0"},
				{"-sc","cc","0"},
				{"0","0","1"}
				};
		
		String[][] m;
		//xyz
		m=multiply(mx, my);
		m=multiply(m, mz);
		print(m);
		System.out.println();
		
		//xzy
		m=multiply(mx, mz);
		m=multiply(m, my);
		print(m);
		System.out.println();
		
		//yxz
		m=multiply(my, mx);
		m=multiply(m, mz);
		print(m);
		System.out.println();
		
		//yzx
		m=multiply(my, mz);
		m=multiply(m, mx);
		print(m);
		System.out.println();
		
		//zxy
		m=multiply(mz, mx);
		m=multiply(m, my);
		print(m);
		System.out.println();
		
		//zyx
		System.out.println("zyx");
		m=multiply(mz, my);
		m=multiply(m, mx);
		print(m);
		System.out.println();
		
		//xyx
		m=multiply(mx, my);
		m=multiply(m, mx);
		print(m);
		System.out.println();
		//xzx
		m=multiply(mx, mz);
		m=multiply(m, mx);
		print(m);
		System.out.println();
		
		//yxy
		m=multiply(my, mx);
		m=multiply(m, my);
		print(m);
		System.out.println();
		//yzy
		m=multiply(my, mz);
		m=multiply(m, my);
		print(m);
		System.out.println();
		
		//zxz
		m=multiply(mz, mx);
		m=multiply(m, mz);
		print(m);
		System.out.println();
		//zyz
		m=multiply(mz, my);
		m=multiply(m, mz);
		print(m);
		System.out.println();
		

	}

}
