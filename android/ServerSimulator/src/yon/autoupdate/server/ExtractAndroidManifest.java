package yon.autoupdate.server;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import org.xmlpull.v1.XmlPullParser;

import android.content.res.AXmlResourceParser;

/**
 * @author hongzhibiao
 * @email babala_234@163.com
 * AXMLPrinter(下载地址：http://code.google.com/p/android4me/downloads/list)
 */
public class ExtractAndroidManifest {

	/**
	 * apk包实际上就是一个zip文件，用java中的gzip就可以解压出AndroidManifest.xml
	 * @return
	 */
	public static File zip2file() {
		File file=null;
		try {
			ZipInputStream in = new ZipInputStream(new FileInputStream(System.getProperty("user.dir")+ "\\apk\\AndroidClientSimulator.apk"));
			ZipEntry z = null;
			while ((z = in.getNextEntry()) != null) {
				if (!z.isDirectory()&& z.getName().equals("AndroidManifest.xml")) {
					File f = new File(System.getProperty("user.dir")+ "\\temp");
					f.mkdirs();
					file = new File(System.getProperty("user.dir")+ "\\temp\\" + z.getName());
					if(file.exists())
						file.delete();
					OutputStream out = new FileOutputStream(file);
					int b;
					while ((b = in.read()) != -1) {
						out.write(b);
					}
					out.close();
					break;
				}
			}
			in.close();

		} catch (Exception e) {
			e.printStackTrace();
		}
		return file;
	}

	/**
	 * apk中提取的AndroidManifest.xml是经过压缩的，无法直接用XmlPullParser解析。
	 * 幸亏AXMLPrinter已经帮我们做好了解压缩和解析xml的算法，从googlecode下载下来的源码稍微改动下就可以应用到服务器端的项目了。
	 * @return
	 */
	public static int getVersionCode() {
		try {
			File androidManifest = ExtractAndroidManifest.zip2file();
			AXmlResourceParser parser = new AXmlResourceParser();
			parser.open(new FileInputStream(androidManifest));
			while (true) {
				int type = parser.next();
				if (type == XmlPullParser.END_DOCUMENT) {
					break;
				}
				switch (type) {
				case XmlPullParser.START_TAG:
					for (int i = 0; i != parser.getAttributeCount(); ++i) {
						//System.out.println(parser.getAttributeName(i)+":"+parser.getAttributeValue(i));
						if (parser.getAttributeName(i).equals("versionCode")) {
							return Integer.parseInt(parser.getAttributeValue(i));
						}
					}
					break;
				}
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
		return -1;
	}
	
	public static void main(String[] args) {
		
		System.out.println(getVersionCode());
	}

}
