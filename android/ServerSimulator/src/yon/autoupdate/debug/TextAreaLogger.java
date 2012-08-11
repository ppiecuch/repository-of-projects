package yon.autoupdate.debug;

import javax.swing.JTextArea;

public class TextAreaLogger implements Logger {
	
	private JTextArea textArea=null;
	
	public void setOutput(JTextArea textArea) {
		this.textArea = textArea;
	}

	@Override
	public void debug(String str) {
		textArea.append("[DEBG]"+str);
		textArea.setCaretPosition(textArea.getText().length());
	}

	@Override
	public void info(String str) {
		textArea.append("[INFO]"+str);
		textArea.setCaretPosition(textArea.getText().length());
	}

	@Override
	public void warn(String str) {
		textArea.append("[WARN]"+str);
		textArea.setCaretPosition(textArea.getText().length());
	}

	@Override
	public void error(String str) {
		textArea.append("[EROR]"+str);
		textArea.setCaretPosition(textArea.getText().length());
	}

}
