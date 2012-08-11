package yon.autoupdate.net;

import java.io.InputStream;
import java.io.OutputStream;

public interface StreamProcessor {
	public void process(InputStream is,OutputStream os);
}
