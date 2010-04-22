package sms.abc;

import java.io.IOException;

import javax.microedition.io.Connector;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.TextMessage;

/**
 * File Name : SMSSender.java Created on : 2005-4-28 Summary : A standalone
 * thead, which is used to send SMS. Author : Jedi Chen
 * 简单的手机发短消息工具类
 * quote:http://gundumw100.javaeye.com/blog/623983
 */

public final class SMSSender implements Runnable {

	// the string stores the text message that will be sent.
	private String message;
	// the destination address for current SMS sender.
	private String address;

	// boolean variable indicates whether the SMS has been sent successfully.
	// initialized as false when get the singleton instance.
	private static boolean doSuccessfully;

	// boolean variable indicates whether error occured while sending SMS.
	// initialized as false when get the singleton instance.
	private static boolean errorOccured;

	// the singleton instance of SMSSender, since one instance is enough
	// for one MIDlet, we apply the Singleton pattern for this class.
	private static SMSSender instance;

	/**
	 * the Factory method to get the singleton instance.
	 */
	public static SMSSender getSMSSender() {
		if (instance == null) {
			instance = new SMSSender();
		} else {
			instance.reset();
		}

		return instance;
	}

	/**
	 * The private constructor for SMSSender, only could be called by
	 * getSMSSender.
	 * 
	 * call resetSenderStatus() to reset the members.
	 */
	private SMSSender() {
		reset();
	}

	/**
	 * Once the caller get the sender status, it must call this method to reset
	 * both status.
	 */
	private void reset() {
		doSuccessfully = false;
		errorOccured = false;
		address = null;
		message = null;
	}

	public synchronized void setMessageText(String address, String message) {

		// assert(address != null && !address.equals(""));
		this.address = "sms://" + address;

		if (message == null || message.equals(""))
			message = "[WARN] Error formatted message!";

		this.message = message;
		// System.out.println("[SMS] " + s);
		// m_fDoSuccessfully = false;
		// m_fErrorOccured = false;
	}

	/*
	 * Send the message in a standalone thread.
	 * 
	 * @see java.lang.Runnable#run()
	 */
	public void run() {
		MessageConnection smsconn = null;
		try {
			smsconn = (MessageConnection) Connector.open(address);

			TextMessage txtmsg = (TextMessage) smsconn
					.newMessage(MessageConnection.TEXT_MESSAGE);
			txtmsg.setPayloadText(message);

			smsconn.send(txtmsg);

			doSuccessfully = true;
			// System.out.println("[SMS] SMS sent successfully :)");
		} catch (Exception expt) {
			errorOccured = true;
			// System.out.println("[SMS] SMS sent error!");
		} finally {
			if (smsconn != null) {
				try {
					smsconn.close();
				} catch (IOException ioex) {
					// System.out.println("[SMS] Close SMS connection error caught!");
				}
			}
		}
	}

	/**
	 * @return Returns the doSuccessfully.
	 */
	public static synchronized boolean isDoSuccessfully() {
		return doSuccessfully;
	}

	/**
	 * @return Returns the errorOccured.
	 */
	public static synchronized boolean isErrorOccured() {
		return errorOccured;
	}

}