package core;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;

public class XMLFormater {

//	public static String format(String source) throws JDOMException,
//			IOException {
//		SAXBuilder sb = new SAXBuilder();
//		Reader reader = new StringReader(source);
//		Document doc = sb.build(reader);
//		XMLOutputter outputter = new XMLOutputter(Format.getPrettyFormat());
//		String result = outputter.outputString(doc);
//		return result;
//	}

	public static void format(File file) throws DocumentException, IOException {
		
		SAXReader reader = new SAXReader();
	   	Document doc= (Document)reader.read(file);
		OutputFormat of = OutputFormat.createPrettyPrint();
        of.setIndent(true);
        of.setNewlines(true);
        Writer fileWriter=new FileWriter(file);   
        XMLWriter xmlWriter=new XMLWriter(fileWriter,of);
        xmlWriter.write(doc);   
        xmlWriter.close();                      
	}

}
