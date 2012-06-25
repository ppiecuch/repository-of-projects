package yon;

/**
 * A listener class for receiving text input.
 */
public interface TextInputListener {
    /**
     * Called from InputConnection to be committed with given text.
     * @param text The committed text.
     * @param newCursorPosition the new cursor position around the text.
     * @return Returns true on success, false if the input connection is no longer valid. 
     */
    boolean onCommitText(CharSequence text, int newCursorPosition);
}