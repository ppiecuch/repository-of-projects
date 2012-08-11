package yon.autoupdate.server;

import javax.swing.SwingUtilities;
import java.awt.BorderLayout;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JToggleButton;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import yon.autoupdate.net.Callback;

public class Server extends JFrame {

	private static final long serialVersionUID = 1L;
	private JPanel jContentPane = null;
	private JToggleButton setupToggleButton = null;
	private JScrollPane logScrollPane = null;
	private JTextArea logTextArea = null;

	/**
	 * This method initializes setupToggleButton	
	 * 	
	 * @return javax.swing.JToggleButton	
	 */
	private JToggleButton getSetupToggleButton() {
		if (setupToggleButton == null) {
			setupToggleButton = new JToggleButton();
			setupToggleButton.setText("close");
			setupToggleButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					if(setupToggleButton.isSelected())
					{
						Delegate.getInstance().setup(new Callback() {
							
							@Override
							public void doFinish() {
								Delegate.getInstance().close();
								setupToggleButton.setText("close");
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
			logTextArea.setEditable(false);
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
				Server thisClass = new Server();
				thisClass.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				thisClass.setVisible(true);
			}
		});
	}

	/**
	 * This is the default constructor
	 */
	public Server() {
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
		this.setSize(600, 400);
		this.setContentPane(getJContentPane());
		this.setTitle("Autoupdate Server");
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
			jContentPane.add(getSetupToggleButton(), BorderLayout.NORTH);
			jContentPane.add(getLogScrollPane(), BorderLayout.CENTER);
		}
		return jContentPane;
	}

}
