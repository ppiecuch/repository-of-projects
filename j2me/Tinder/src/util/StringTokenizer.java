package util;

import java.util.Enumeration;
import java.util.NoSuchElementException;

public class StringTokenizer implements Enumeration {

	private void setMaxDelimChar() {
		if (delimiters == null) {
			maxDelimChar = '\0';
			return;
		}
		char c = '\0';
		for (int i = 0; i < delimiters.length(); i++) {
			char c1 = delimiters.charAt(i);
			if (c < c1)
				c = c1;
		}

		maxDelimChar = c;
	}

	public StringTokenizer(String s, String s1, boolean flag) {
		currentPosition = 0;
		newPosition = -1;
		delimsChanged = false;
		str = s;
		maxPosition = s.length();
		delimiters = s1;
		retDelims = flag;
		setMaxDelimChar();
	}

	public StringTokenizer(String s, String s1) {
		this(s, s1, false);
	}

	public StringTokenizer(String s) {
		this(s, " \t\n\r\f", false);
	}

	private int skipDelimiters(int i) {
		if (delimiters == null)
			throw new NullPointerException();
		int j;
		for (j = i; !retDelims && j < maxPosition; j++) {
			char c = str.charAt(j);
			if (c > maxDelimChar || delimiters.indexOf(c) < 0)
				break;
		}

		return j;
	}

	private int scanToken(int i) {
		int j;
		for (j = i; j < maxPosition; j++) {
			char c = str.charAt(j);
			if (c <= maxDelimChar && delimiters.indexOf(c) >= 0)
				break;
		}

		if (retDelims && i == j) {
			char c1 = str.charAt(j);
			if (c1 <= maxDelimChar && delimiters.indexOf(c1) >= 0)
				j++;
		}
		return j;
	}

	public boolean hasMoreTokens() {
		newPosition = skipDelimiters(currentPosition);
		return newPosition < maxPosition;
	}

	public String nextToken() {
		currentPosition = newPosition < 0 || delimsChanged ? skipDelimiters(currentPosition)
				: newPosition;
		delimsChanged = false;
		newPosition = -1;
		if (currentPosition >= maxPosition) {
			throw new NoSuchElementException();
		} else {
			int i = currentPosition;
			currentPosition = scanToken(currentPosition);
			return str.substring(i, currentPosition);
		}
	}

	public String nextToken(String s) {
		delimiters = s;
		delimsChanged = true;
		setMaxDelimChar();
		return nextToken();
	}

	public boolean hasMoreElements() {
		return hasMoreTokens();
	}

	public Object nextElement() {
		return nextToken();
	}

	public int countTokens() {
		int i = 0;
		for (int j = currentPosition; j < maxPosition;) {
			j = skipDelimiters(j);
			if (j >= maxPosition)
				break;
			j = scanToken(j);
			i++;
		}

		return i;
	}

	private int currentPosition;

	private int newPosition;

	private int maxPosition;

	private String str;

	private String delimiters;

	private boolean retDelims;

	private boolean delimsChanged;

	private char maxDelimChar;
}