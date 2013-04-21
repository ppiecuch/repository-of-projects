package beta04;

import java.awt.Image;
import java.awt.image.PixelGrabber;
import java.io.File;

import javax.swing.ImageIcon;

import sun.security.jca.GetInstance.Instance;

public class PixelImage {
	
	private int width,height;
	private String name;
	private int[] pixels;
	
	private void init(Image image){
		//System.out.println(this+"w:"+width+",height:"+height);
		this.pixels=new int[width*height];
		PixelGrabber pg = new PixelGrabber(image, 0, 0, width, height, pixels,0,width);
		try {
			pg.grabPixels();
		} catch (InterruptedException e) {
			e.printStackTrace();
			this.width=-1;
			this.height=-1;
			this.pixels=null;
		}
	}
	
	public PixelImage(String dir,String file) {
		this.name=file;
		String path=new File(dir,file).getAbsolutePath();
		ImageIcon ii=new ImageIcon(path);
		this.width=ii.getIconWidth();
		this.height=ii.getIconHeight();
		init(ii.getImage());
		//System.out.println("PixelImage(String dir,String file):"+path);
	}
	
	public PixelImage(Image image) {
		this.name=image.toString();
		this.width=image.getWidth(null);
		this.height=image.getHeight(null);
		//System.out.println("PixelImage(Image image)");
		init(image);
	}
	
	public PixelImage(String name,int width,int height){
		this.width=width;
		this.height=height;
		this.name=name;
		this.pixels=new int[width*height];
	}

	public int getWidth() {
		return width;
	}

	public int getHeight() {
		return height;
	}

	public String getName() {
		return name;
	}

	public int[] getPixels() {
		return pixels;
	}
	
	@Override
	public boolean equals(Object o) {
		if(o instanceof PixelImage ==false)
		{
			System.out.println("not instanceof");
			return false;
		}
		PixelImage pi=(PixelImage)o;
		if(this.width!=pi.width || this.height!=pi.height)
		{
			System.out.println("not same dimension");
			return false;
		}
		
		for(int r=0;r<height;++r)
		{
			int offset=r*width;
			for(int c=0;c<width;++c)
			{
				if(pixels[offset+c]!=pi.pixels[offset+c])
				{
					//System.out.println("not same color["+(offset+c)+"]:"+Integer.toHexString(pixels[offset+c])+","+Integer.toHexString(pi.pixels[offset+c]));
					return false;
				}
			}
		}
		
		return true;
	}
	
	@Override
	public String toString() {
		return this.name;
	}
	
}
