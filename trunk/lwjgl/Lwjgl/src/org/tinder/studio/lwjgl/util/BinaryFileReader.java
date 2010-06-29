/*
 * BinaryFileReader.java
 *
 * Created on 24. Juni 2006, 23:40
 */

package org.tinder.studio.lwjgl.util;



import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;


/**
 * @author xindon
 */
public class BinaryFileReader {

	private byte[] data;
	private int index = 0;




	/***************************************************************************************************************************************************************************************************
	 * BinaryFileReader() - Constructor
	 * 
	 * @param is
	 *            The InputStream of the file to load
	 **************************************************************************************************************************************************************************************************/
	public BinaryFileReader(InputStream is) {

		try {

			DataInputStream dis = new DataInputStream(is); // Open file as DataInputStream
			this.data = new byte[dis.available()]; // Allocate memory for file
			dis.readFully(this.data); // Read the file completely into memory
			dis.close(); // Close the inut stream

		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}




	/***************************************************************************************************************************************************************************************************
	 * readByteAsInt() - Read one byte from the data array and return it as Integer. Increase file index by one.
	 * 
	 * @return The read byte as an int
	 **************************************************************************************************************************************************************************************************/
	public int readByteAsInt() {

		int b = (data[index] & 0xFF); // Get byte
		index++; // Increase index

		return b;
	}




	/***************************************************************************************************************************************************************************************************
	 * readByte() - Read one byte from the data array and return it
	 * 
	 * @return
	 **************************************************************************************************************************************************************************************************/
	public byte readByte() {

		index++;
		return data[index - 1];
	}




	/***************************************************************************************************************************************************************************************************
	 * readShort() - Read two bytes from the data array and generate a short. Return it as Integer and increase file index by one.
	 * 
	 * @return The read short as an int
	 **************************************************************************************************************************************************************************************************/
	public int readShort() {

		int s1 = (data[index] & 0xFF); // Get byte one
		int s2 = (data[index + 1] & 0xFF) << 8; // Get byte two
		index += 2; // Increase index

		return (s1 | s2);
	}




	/***************************************************************************************************************************************************************************************************
	 * readInt() - Read 4 bytes from the data array and generate an Integer. Increase file index by four.
	 * 
	 * @return The read int
	 **************************************************************************************************************************************************************************************************/
	public int readInt() {

		int i1 = (data[index] & 0xFF); // Get byte one
		int i2 = (data[index + 1] & 0xFF) << 8; // Get byte two
		int i3 = (data[index + 2] & 0xFF) << 16; // Get byte three
		int i4 = (data[index + 3] & 0xFF) << 24; // Get byte four
		index += 4; // Increase index

		return (i1 | i2 | i3 | i4);
	}




	/***************************************************************************************************************************************************************************************************
	 * readFloat() - Read 4 bytes from the data array and generate a float. Increase file index by four.
	 * 
	 * @return The read float
	 **************************************************************************************************************************************************************************************************/
	public float readFloat() {

		return Float.intBitsToFloat(this.readInt()); // Get float from int
	}




	/***************************************************************************************************************************************************************************************************
	 * readString() - Read some number of bytes from the data array and return a String. Increase file index by number of characters read.
	 * 
	 * @return The read string
	 **************************************************************************************************************************************************************************************************/
	public String readString(int length) {

		// Look for zero terminated string from byte array
		for (int i = index; i < index + length; i++) {
			if (data[i] == (byte) 0) {
				String s = new String(data, index, i - index);
				index += length;
				return s;
			}
		}

		String s = new String(data, index, length);
		index += length;
		return s;
	}




	/***************************************************************************************************************************************************************************************************
	 * setOffset() - Sets the file index to the new offset-
	 * 
	 * @param newOffset
	 *            The new byte offset in the file
	 **************************************************************************************************************************************************************************************************/
	public void setOffset(int newOffset) {

		if (newOffset < 0 || newOffset > data.length)
			return;
		else
			this.index = newOffset;
	}


}
