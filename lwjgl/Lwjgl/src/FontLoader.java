import java.awt.Font;
import java.awt.FontFormatException;
import java.io.BufferedInputStream;
import java.io.IOException;
import org.newdawn.slick.*;
import org.newdawn.slick.font.effects.*;

//It does nothing but to load a True Type Font ... we can use it later 
//to load unicode fonts as well. 
@SuppressWarnings("unchecked")
public class FontLoader {

	public static UnicodeFont loadFontByFile(String fontLocation, int fontSize, java.awt.Color fontColor)
			throws SlickException, FontFormatException, IOException {
		// Get font from fontLocation
		BufferedInputStream location = new BufferedInputStream(FontLoader.class
				.getClassLoader().getResourceAsStream(fontLocation));
		// Now a font based on the given file is created
		Font startFont = Font.createFont(Font.TRUETYPE_FONT, location);
		// As the TrueTypeFont constructor needs already scaled fonts, we need
		// to do that little WA
		Font baseFont = startFont.deriveFont(Font.TRUETYPE_FONT, fontSize);
		UnicodeFont tFont = new UnicodeFont(baseFont);
		tFont.getEffects().add(new ColorEffect(fontColor));
		/*
         * Tells the instance that you want to use the ASCII characters subset, 
         * and the glyphs at Unicode point 4e00 through to 9fa5 inclusive. 
         * UnicodeFont arranges the glyphs to be rendered to the backing texture as efficiently as possible. 
         * So it is best to add all the glyphs you know you will be using ahead of time. 
         */
		tFont.addAsciiGlyphs();
		tFont.addGlyphs(0x4e00, 0x9fa5);
		/*
		 * Loads the glyphs. UnicodeFont renders the glyphs to the backing texture. The font is now ready to be used. 
		 */
		tFont.loadGlyphs();
		return tFont;
	}

	public static UnicodeFont loadFontByName(String fontName, int fontSize, java.awt.Color fontColor)
			throws SlickException, FontFormatException, IOException {
		Font font = new Font(fontName, Font.PLAIN, fontSize); 
		UnicodeFont tFont = new UnicodeFont(font); 
		tFont.getEffects().add(new ColorEffect(fontColor));
		tFont.addAsciiGlyphs();
		tFont.addGlyphs(0x4e00, 0x9fa5);
		tFont.loadGlyphs();
		return tFont;
	}

}