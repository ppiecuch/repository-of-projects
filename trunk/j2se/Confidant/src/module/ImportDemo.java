package module;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;

public class ImportDemo {

	/**
	 * @param args
	 * @throws IOException 
	 * @throws ClassNotFoundException 
	 */
	public static void main(String[] args) throws IOException, ClassNotFoundException {
		File file =new File("D:/file.txt");
		FileInputStream fis=new FileInputStream(file);
		ObjectInputStream ois=new ObjectInputStream(fis);
		
		Operation operation=new Operation();
		operation.readExternal(ois);
		System.out.println(operation);
		operation.readExternal(ois);
		System.out.println(operation);
		operation.readExternal(ois);
		System.out.println(operation);
		operation.readExternal(ois);
		System.out.println(operation);
		
		ois.close();

	}

}
