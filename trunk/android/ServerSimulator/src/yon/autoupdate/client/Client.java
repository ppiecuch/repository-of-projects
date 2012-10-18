package yon.autoupdate.client;

import javax.swing.SwingUtilities;
import java.awt.BorderLayout;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import yon.autoupdate.net.Callback;

public class Client extends JFrame {

	private static final long serialVersionUID = 1L;
	private JPanel jContentPane = null;
	private JTextField ipTextField = null;
	private JToggleButton setupToggleButton = null;
	private JScrollPane logScrollPane = null;
	private JTextArea logTextArea = null;

	/**
	 * This method initializes ipTextField	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getIpTextField() {
		if (ipTextField == null) {
			ipTextField = new JTextField();
			ipTextField.setText("192.168.23.100");
		}
		return ipTextField;
	}

	/**
	 * This method initializes setupToggleButton	
	 * 	
	 * @return javax.swing.JToggleButton	
	 */
	private JToggleButton getSetupToggleButton() {
		if (setupToggleButton == null) {
			setupToggleButton = new JToggleButton();
			setupToggleButton.setText("open");
			setupToggleButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					if(setupToggleButton.isSelected())
					{
						String ip=ipTextField.getText();
						Delegate.getInstance().connect(ip, 8081,new Callback() {
							
							@Override
							public void doFinish() {
								setupToggleButton.setSelected(false);
							}
						});
						setupToggleButton.setText("open");
					}
					else
					{
						Delegate.getInstance().close();
						setupToggleButton.setText("close");
					}
				}
			});
		}
		return setupToggleButton;
	}

	/**
	 * This method initializes logScrollPane	
	 * 	
	 * @return javax.swing.JScrollPane	
	 */
	private JScrollPane getLogScrollPane() {
		if (logScrollPane == null) {
			logScrollPane = new JScrollPane();
			logScrollPane.setViewportView(getLogTextArea());
		}
		return logScrollPane;
	}

	/**
	 * This method initializes logTextArea	
	 * 	
	 * @return javax.swing.JTextArea	
	 */
	private JTextArea getLogTextArea() {
		if (logTextArea == null) {
			logTextArea = new JTextArea();
		}
		return logTextArea;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				Client thisClass = new Client();
				thisClass.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				thisClass.setVisible(true);
			}
		});
	}

	/**
	 * This is the default constructor
	 */
	public Client() {
		super();
		initialize();
		Delegate.getInstance().setOutput(logTextArea);
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(300, 200);
		this.setContentPane(getJContentPane());
		this.setTitle("Autoupdate Client");
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new JPanel();
			jContentPane.setLayout(new BorderLayout());
			jContentPane.add(getIpTextField(), BorderLayout.NORTH);
			jContentPane.add(getSetupToggleButton(), BorderLayout.SOUTH);
			jContentPane.add(getLogScrollPane(), BorderLayout.CENTER);
		}
		return jContentPane;
	}

}
