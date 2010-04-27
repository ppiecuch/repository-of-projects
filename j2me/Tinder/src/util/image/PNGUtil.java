package util.image;

import javax.microedition.lcdui.Image;

public class PNGUtil {
	
	public static void getImageBufferForImageARGB8888(Image img, byte[] rawByte,
            int w, int h, int off) {
        int n = off;
        int[] raw = new int[w];
        for (int j = 0; j < h; j++) {
            img.getRGB(raw, 0, w, 0, j, w, 1);
            for (int i = 0; i < raw.length; i++) {
                int ARGB = raw[i];
                int a = (ARGB & 0xff000000) >> 24;
                int r = (ARGB & 0xff0000) >> 16;
                int g = (ARGB & 0xff00) >> 8;
                int b = ARGB & 0xff;
                if (i % w == 0) {
                    n += 1;
                }
                rawByte[n] = (byte) r;
                rawByte[n + 1] = (byte) g;
                rawByte[n + 2] = (byte) b;
                rawByte[n + 3] = (byte) a;
                n += 4;
            }
        }
        raw = null;
        System.gc();
    }

}
