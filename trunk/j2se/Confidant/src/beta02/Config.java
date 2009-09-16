package beta02;

import java.io.File;
import java.io.IOException;

import org.apache.xmlbeans.XmlException;
import org.dom4j.DocumentException;

import core.XMLFormater;

import script.CalculagraphDocument;

import module.Calculagraph;
import module.Operation;

public class Config {

	private static final String FILE_PATH = "script.xml";

	/**
	 * 导入脚本
	 * @param calculagraph
	 */
	public static void loadConfig(Calculagraph calculagraph) {
		File file = new File(FILE_PATH);
		try {
			CalculagraphDocument doc = CalculagraphDocument.Factory.parse(file);
			calculagraph.setTitle(doc.getCalculagraph().getTitle());
			int operationSize = doc.getCalculagraph().getOperations()
					.sizeOfOperationArray();
			for (int i = 0; i < operationSize; i++) {
				Operation operation = new Operation();
				operation.decode(doc.getCalculagraph().getOperations()
						.getOperationArray(i));
				calculagraph.addOperation(operation);
			}
		} catch (XmlException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 生成脚本
	 * @param calculagraph
	 */
	public static void saveConfig(Calculagraph calculagraph) {
		File file = new File(FILE_PATH);
		try {
			file.createNewFile();

			CalculagraphDocument doc = CalculagraphDocument.Factory
					.newInstance();
			if (calculagraph != null) {
				doc.addNewCalculagraph().setTitle(calculagraph.getTitle());
				if (calculagraph.getOperations() != null) {
					doc.getCalculagraph().addNewOperations();
					for (Operation op : calculagraph.getOperations()) {
						doc.getCalculagraph().getOperations().addOperation(
								op.encode());
					}
				}
			}
			doc.save(file);
			XMLFormater.format(file);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (DocumentException e) {
			e.printStackTrace();
		}
	}

}
