/** 
 * Decodes and shows 8 bit color mapped,black and white and 16,24 and 32 bit
 * true color uncompressed and RLE Tga images. The compressed color mapped
 * formats 32 and 33 are not implemented in this verson because I have not
 * found any information on these two formats.
 */

/**
 * This software is provided "AS IS," without a warranty of any kind.
 * anyone can use it for free,emails are welcomed concerning bugs or
 * suggestions.
 */

/**
 * TGAReader.java.  
 *
 * @version 1.0 03/26/2007
 * @author Wen Yu, yuwen_66@yahoo.com
 */
package util.image;

import java.io.*;


public class TGAReader
{
	int width = 0;
	int height = 0;
	int pix[] = null;	
	int bitsPerPixel = 0;
    // IndexedColor related variables 
	boolean indexedColor =  false;
    int colorPalette[] = null;
    private TgaHeader tgaHeader;
    private int scanMode = 0;
	private int l = 0, m = 0, n = 0, o = 0;

   /** This method could have returned an Image object directly using
     * Toolkit.getDefaultToolkit().createImage(ImageProducer) method, 
     * but this method is somewhat slow when called in a non-gui class in
     * creating an image, a subclass of JFrame would do much faster!
	 */

   public void unpackImage(InputStream is) throws Exception
   {
	   tgaHeader = new TgaHeader();
       tgaHeader.readHeader(is);
    
	   bitsPerPixel = tgaHeader.bits_per_pixel;
 	   width = tgaHeader.width;
	   height = tgaHeader.height;
	   pix = new int[width*height];
	
	   if (tgaHeader.colourmap_type != 0 && tgaHeader.colourmap_type != 1) {
		   System.out.println("Can only handle colour map types of 0 and 1");    
		   System.exit(1);     
	   }

       scanMode = ((tgaHeader.image_descriptor&0x30)>>4);

       switch (scanMode)
       {
            case 0:
				l = height-1; m = -1; n = 0; o = 1;
				break;
            case 1:
                l = height-1; m = -1; n = width-1; o = -1;
				break;
			case 2:
                l = 0; m = 1; n = 0; o = 1;
				break;
            case 3:
				l = 0; m = 1; n = width-1; o = -1; 
				break;
			default:
       }

	   switch (tgaHeader.image_type)
	   {
	         case 0:
	               System.out.println("There are no data in the image file");
	               System.exit(1);
	          case 1:
	               unpack_CMP_TgaFile(is);
		           break;
	          case 2: 
                   unpackTrueColorTgaFile(is);
		           break;
	          case 3:
			       unpack_BW_TgaFile(is);
			       break;
		      case 9:
			       unpack_RLE_CMP_TgaFile(is);
			       break;
       		  case 10:
                   unpack_RLE_TrueColor_TgaFile(is);
			       break;
			  case 11:
			       unpack_RLE_BW_TgaFile(is);
			       break;
			  case 32: 
              case 33:
			       System.out.println("Not implemented for compressed color mapped images");
		           System.exit(1);
			  default:
				   System.out.println("I can't find a type matches this");
			       System.exit(1);			
	   }
	}	
   
   private void unpackTrueColorTgaFile(InputStream is) throws Exception
	{
         int skipover = 0;
         int nindex = 0;

         skipover +=  tgaHeader.id_length;
		 skipover += tgaHeader.colourmap_type * tgaHeader.colourmap_length; 
		 is.skip(skipover);
	
         int bytes2read = tgaHeader.bits_per_pixel / 8;
			
	     byte brgb[] = new byte[bytes2read*width*height];
         is.read(brgb,0,bytes2read*width*height);
		 
         if (bytes2read == 3) // 24 bit image
	      {
			 System.out.println("24 bits Tga uncompressed image!");
            
			 for(int i = 0; i < height; i++) {
			     for(int j = 0; j < width; j++) {
                     pix[width*(l+m*i)+n+o*j] = (0xff<<24)|((brgb[nindex++]&0xff))|((brgb[nindex++]&0xff)<<8)|((brgb[nindex++]&0xff)<<16);
		         }
			 }
		  }
		 else if (bytes2read == 4) // 32 bit image 
		  {
			 System.out.println("32 bits Tga uncompressed image!");
            
			 for(int i = 0; i < height; i++) {
			      for(int j = 0; j < width; j++) {
                      pix[width*(l+m*i)+n+o*j] = ((brgb[nindex++]&0xff))|((brgb[nindex++]&0xff)<<8)|((brgb[nindex++]&0xff)<<16)|(255<<24);
			          nindex++;
				  } 
			 }
		  }
		 else if (bytes2read == 2) // 16 bit image
		  {
			 System.out.println("16 bits Tga uncompressed image!");
		  
		     int r = 0,g = 0,b = 0,a = 0;  
  
			 /** The two byte entry is broken down as follows:
			  *  ARRRRRGG GGGBBBBB, where each letter represents a bit.
			  *  but, because of the lo-hi storage order, the first byte
			  *  coming from the file will actually be GGGBBBBB, and the
			  *  second will be ARRRRRGG. "A" represents an attribute.
			  */
		     for(int i = 0; i < height; i++) {
			      for(int j = 0; j < width; j++) {
                       r = ((brgb[++nindex] & 0x7c) <<1);
		               g = (((brgb[nindex] & 0x03) << 6) | ((brgb[nindex-1] & 0xe0) >> 2));
	                   b = ((brgb[nindex-1] & 0x1f)<<3);
                   //  a = (brgb[nindex] & 0x80);
			           a = 0xff;
			  		   nindex++;
		         	   pix[width*(l+m*i)+n+o*j] = ((a<<24)|(r<<16)|(g<<8)|b);
			      }
		     }
	      }
		  is.close();
	}
   	
    private void  unpack_RLE_TrueColor_TgaFile(InputStream is) throws Exception
	{
	    
         int skipover = 0;
		 int nindex = 0;
         int p = 0;
		 int k = 0;
		 int i = 0;
		 int j = 0;

		 skipover += tgaHeader.id_length; 
		 skipover += tgaHeader.colourmap_type * tgaHeader.colourmap_length; 
		 is.skip(skipover);
	          
         int bytes2read = tgaHeader.bits_per_pixel / 8;
                 
		 int available = is.available();
 
		 byte brgb[] = new byte[available];
         is.read(brgb,0,available);
		 
		 if (bytes2read == 3) // 24 bit image
	      {
			 System.out.println("24 bits Tga RLE image!");
			 
			 while(p<width*height) { 
			       
			     k = (brgb[nindex++] & 0x7f)+1; 
						 					       
				   if ((brgb[nindex-1] & 0x80) != 0){
					   for(int q = 0; q < k; q++){
						   pix[width*(l+m*i)+n+o*j] = (0xff<<24)|((brgb[nindex]&0xff))|((brgb[nindex+1]&0xff)<<8)|((brgb[nindex+2]&0xff)<<16);
						   j++;
						   if(j%width == 0)
						   {
							   i++;
							   j = 0;
						   }
						   p++;
						   if (p >= width*height)
					       {
						      break;
					       }
					   }
				   nindex += 3;
				   }
				   else {
					   for (int q = 0; q < k; q++)
					   {
						   pix[width*(l+m*i)+n+o*j] = (0xff<<24)|((brgb[nindex++]&0xff))|((brgb[nindex++]&0xff)<<8)|((brgb[nindex++]&0xff)<<16);  
						   j++;
						   if(j%width == 0)
						   {
							   i++;
							   j = 0;
						   }
						   p++;
						   if (p >= width*height)
					       {
							   break;
					       }
					   }
				   }
			  } 
	      }
		 else if (bytes2read == 4) // 32 bit image 
		  {
		     System.out.println("32 bits Tga RLE image!");
            
			
			 while(p<width*height) { 
			        
				   k = (brgb[nindex++] & 0x7f)+1; 
						 					       
					   if ((brgb[nindex-1] & 0x80) != 0){
						   for(int q = 0; q < k; q++){
							   pix[width*(l+m*i)+n+o*j] = ((brgb[nindex]&0xff))|((brgb[nindex+1]&0xff)<<8)|((brgb[nindex+2]&0xff)<<16)|(255<<24);
			     	           j++;
						       if(j%width == 0)
						       {
							     i++;
							     j = 0;
						       }
						       p++;
							   if (p >= width*height)
					     	   {
							      break;
					     	   }
							}
						   nindex += 4;
					    }
					   else {
						   for (int q = 0; q < k; q++)
						   {
							   pix[width*(l+m*i)+n+o*j] = ((brgb[nindex++]&0xff))|((brgb[nindex++]&0xff)<<8)|((brgb[nindex++]&0xff)<<16)|(255<<24);  
						       nindex++;
							   j++;
						       if(j%width == 0)
						       {
							     i++;
							     j = 0;
						       }
						       p++;
							   if (p >= width*height)
						       {
								   break;
						       }
						   }
					   }
			  } 
		  }
		 else if (bytes2read == 2) 
		  {
		     System.out.println("16 bits Tga RLE image!");
			 
			 int r = 0,g = 0,b = 0,a = 0;  
  
			 /** The two byte entry is broken down as follows:
			  *  ARRRRRGG GGGBBBBB, where each letter represents a bit.
			  *  but, because of the lo-hi storage order, the first byte
			  *  coming from the file will actually be GGGBBBBB, and the
			  *  second will be ARRRRRGG. "A" represents an attribute.
			  */
			 
	
			 while(p<width*height) { 
			            
				   k = (brgb[nindex++] & 0x7f)+1; 
						 					       
					   if ((brgb[nindex-1] & 0x80) != 0){
						   r = ((brgb[++nindex] & 0x7c) <<1);
		                   g = (((brgb[nindex] & 0x03) << 6) | ((brgb[nindex-1] & 0xe0) >> 2));
	                       b = ((brgb[nindex-1] & 0x1f)<<3);
			    		   a = 0xff;
						   nindex++;
						   for(int q = 0; q < k; q++){
							   pix[width*(l+m*i)+n+o*j] = ((a<<24)|(r<<16)|(g<<8)|b);
				     	       j++;
						       if(j%width == 0)
						       {
							     i++;
							     j = 0;
						       }
						       p++;
							   if (p >= width*height)
				     	       {
							      break;
				     	       }
						   }
					   }
					   else {
						   for (int q = 0;q<k;q++)
						   {
							   r = ((brgb[++nindex] & 0x7c) <<1);
		                       g = (((brgb[nindex] & 0x03) << 6) | ((brgb[nindex-1] & 0xe0) >> 2));
	                           b = ((brgb[nindex-1] & 0x1f)<<3);
							   a = 0xff;
							   nindex++;
							   pix[width*(l+m*i)+n+o*j] = ((a<<24)|(r<<16)|(g<<8)|b);
							   j++;
						       if(j%width == 0)
						       {
							     i++;
							     j = 0;
						       }
						       p++;
							   if (p >= width*height)
							   {
							     break;
							   }
						   }
					   }
			  } 
		  }
		  is.close();
	}

    private void readPalette(InputStream is) throws Exception
	   {
		  int index = 0, r = 0, g = 0, b = 0, a = 0;
		  int byte_per_pixel = (tgaHeader.colourmap_entry_size+1)/8;
		  int readbytes = byte_per_pixel*(tgaHeader.colourmap_length-tgaHeader.first_entry_index);
		  byte brgb[] = new byte[readbytes];
		  
		  int colorsUsed = tgaHeader.colourmap_length-tgaHeader.first_entry_index; // actual colors used
		  colorPalette = new int[colorsUsed];
		  indexedColor = true;

		  is.skip(tgaHeader.id_length);
          is.skip(tgaHeader.first_entry_index);
		  is.read(brgb,0,readbytes);

		  switch (tgaHeader.colourmap_entry_size)
		  {
		    
			 case 15:
			 case 16:
                   for (int i = 0; i < tgaHeader.colourmap_length - tgaHeader.first_entry_index; i++)
                     {
                         r = ((brgb[++index] & 0x7c) <<1);
		                 g = (((brgb[index] & 0x03) << 6) | ((brgb[index-1] & 0xe0) >> 2));
	                     b = ((brgb[index-1] & 0x1f)<<3); 
                         a = 0xff;
						 colorPalette[i] = ((a<<24)|(r<<16)|(g<<8)|b);
					     index++;
                     } 
					 break;
			 case 24:
                   for (int i = 0; i < tgaHeader.colourmap_length - tgaHeader.first_entry_index; i++)         
			             colorPalette[i] = (0xff<<24)|((brgb[index++]&0xff))|((brgb[index++]&0xff)<<8)|((brgb[index++]&0xff)<<16);
			       break;
			 case 32:
                   for (int i = 0; i < tgaHeader.colourmap_length - tgaHeader.first_entry_index; i++)         
			             colorPalette[i] = ((brgb[index++]&0xff))|((brgb[index++]&0xff)<<8)|((brgb[index++]&0xff)<<16)|((brgb[index++]&0xff)<<24);
			       break;
		     default:
		  }
	   }

	private void  unpack_CMP_TgaFile(InputStream is) throws Exception
	   {
          
		  System.out.println("color mapped Tga uncompressed image!");

		  int index = 0;

          readPalette(is);
         
		  if (tgaHeader.bits_per_pixel != 8)
		  {
			  System.out.println("Can only handle 8 bit color mapped tga file");
			  System.exit(1);
		  }
		
	      byte brgb[] = new byte[width*height];
          is.read(brgb,0,width*height);

          for(int i = 0; i < height; i++) {
		       for(int j = 0; j < width; j++) {
                    pix[width*(l+m*i)+n+o*j] = colorPalette[brgb[index++]&0xff];
		       }
		  }
		  is.close();
	   }


    private void  unpack_RLE_CMP_TgaFile(InputStream is) throws Exception
	   {
          System.out.println("color mapped Tga RLE image!");

		  int nindex = 0;
          int p = 0;
		  int k = 0;
		  int i = 0;
		  int j = 0;

          if (tgaHeader.bits_per_pixel != 8)
		  {
			  System.out.println("Can only handle 8 bit color mapped tga file");
			  System.exit(1);
		  }

		  readPalette(is);

		  int available = is.available();
 
		  byte brgb[] = new byte[available];
          is.read(brgb,0,available);

          
		  while(p<width*height) { 
			            
			   k = (brgb[nindex++] & 0x7f)+1; 
						 					       
				   if ((brgb[nindex-1] & 0x80) != 0){
					   for(int q = 0; q < k; q++){
						   pix[width*(l+m*i)+n+o*j] = colorPalette[brgb[nindex]&0xff];
                           j++;
						   if(j%width == 0)
						   {
							 i++;
							 j = 0;
						   }
						   p++;
			     	       if (p >= width*height)
			     	       {
						      break;
			     	       }
					   }
					   nindex += 1;
				   }
				   else {
					   for (int q = 0; q < k; q++)
					   {
						   pix[width*(l+m*i)+n+o*j] = colorPalette[brgb[nindex++]&0xff];
						   j++;
						   if(j%width == 0)
						   {
							 i++;
							 j = 0;
						   }
						   p++;
					       if (p >= width*height)
					       {
							   break;
					       }
					   }
				   }
			} 
			is.close();
	   }

    private void unpack_BW_TgaFile(InputStream is) throws Exception
	{
          bitsPerPixel = 1;
		  System.out.println("Uncompressed Black and White Tga image!");

		  int index = 0;
		  
		  is.skip(tgaHeader.id_length);
		  is.skip(tgaHeader.colourmap_type * tgaHeader.colourmap_length);

          byte brgb[] = new byte[width*height];
          is.read(brgb,0,width*height);

          for(int i = 0; i < height; i++) {
		      for(int j = 0; j < width; j++) {
                  pix[width*(l+m*i)+n+o*j] = (0xff<<24)|((brgb[index]&0xff))|((brgb[index]&0xff)<<8)|((brgb[index++]&0xff)<<16);
		      }
		  }
		  is.close();
	}

	private void unpack_RLE_BW_TgaFile(InputStream is) throws Exception
	{
          bitsPerPixel = 1;
          System.out.println("Black and White Tga RLE image!");

		  int nindex = 0;
          int p = 0;
		  int k = 0;
		  int i = 0;
		  int j = 0;
   
          is.skip(tgaHeader.id_length);
		  is.skip(tgaHeader.colourmap_type * tgaHeader.colourmap_length);

		  int available = is.available();
 		
		  byte brgb[] = new byte[available];
          is.read(brgb,0,available);

          while(p<width*height) { 
			            
			   k = (brgb[nindex++] & 0x7f)+1; 
						 					       
			   if ((brgb[nindex-1] & 0x80) != 0){
				   for(int q = 0; q < k; q++){
					   pix[width*(l+m*i)+n+o*j] = (0xff<<24)|((brgb[nindex]&0xff))|((brgb[nindex]&0xff)<<8)|((brgb[nindex]&0xff)<<16);
					   j++;
					   if(j%width == 0)
					   {
						 i++;
						 j = 0;
					   }
					   p++;
		     	       if (p >= width*height)
		     	       {
					      break;
		     	       }
				   }
				   nindex += 1;
			   }
			   else {
				   for (int q = 0; q < k; q++)
				   {
					   pix[width*(l+m*i)+n+o*j] = (0xff<<24)|((brgb[nindex]&0xff))|((brgb[nindex]&0xff)<<8)|((brgb[nindex++]&0xff)<<16);
					   j++;
					   if(j%width == 0)
					   {
						 i++;
						 j = 0;
					   }
					   p++;
				       if (p >= width*height)
				       {
						   break;
				       }
				   }
			   }
		  }
		  is.close();
	}

/**
	public static void main(String args[]) throws Exception
	{

		FileInputStream fs = new FileInputStream(args[0]);
        TGAReader reader = new TGAReader();

		long t1 = System.currentTimeMillis();
		reader.unpackImage(fs);
		long t2 = System.currentTimeMillis();
		
		System.out.println("Decoding time: "+(t2-t1)+"ms");
		
		fs.close();
		
		int pix[] = reader.getImageData();
		Dimension dm = reader.getImageSize();
		int width = dm.width;
		int height = dm.height;

		final JFrame jframe = new JFrame("TGAReader");

		jframe.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent evt)
			{
				jframe.dispose();
				System.exit(0);
			}
		});
		Image img = jframe.createImage(new MemoryImageSource(width, height, pix, 0, width));
        JLabel theLabel = new JLabel(new ImageIcon(img));
        jframe.getContentPane().add(new JScrollPane(theLabel));
        jframe.setSize(400,400);
        jframe.setVisible(true);
	}
*/
    private class TgaHeader
    {
	   byte  id_length;
	   byte  colourmap_type;
	   byte  image_type; 
	   short first_entry_index;
	   short colourmap_length;
	   byte  colourmap_entry_size; 
	   short x_origin;
	   short y_origin;
	   short width;
	   short height;
	   byte  bits_per_pixel; 
	   byte  image_descriptor;

	   void readHeader(InputStream is) throws Exception
	   {
	     int nindex = 0;
	     byte header[] = new byte[18]; // for the 18 bit header trunk

         is.read(header,0,18);
         
		 id_length = header[nindex++];
	     colourmap_type = header[nindex++];
	     image_type = header[nindex++];
	     first_entry_index = (short)((header[nindex++]&0xff)|((header[nindex++]&0xff)<<8));
	     colourmap_length = (short)((header[nindex++]&0xff)|((header[nindex++]&0xff)<<8));
         colourmap_entry_size = header[nindex++];
	     x_origin = (short)((header[nindex++]&0xff)|((header[nindex++]&0xff)<<8));
	     y_origin = (short)((header[nindex++]&0xff)|((header[nindex++]&0xff)<<8));
	     width = (short)((header[nindex++]&0xff)|((header[nindex++]&0xff)<<8));
	     height = (short)((header[nindex++]&0xff)|((header[nindex++]&0xff)<<8));
         bits_per_pixel = header[nindex++];
	     image_descriptor = header[nindex++];
	   } 
    }
//  Get the color depth of the image to determine whether a colorPalette exists
	public int getColorDepth()
	{
	   return bitsPerPixel;
	}
    //Returns a colorPalette for an indexed-color image
	public int[] getColorPalette()
	{
       return colorPalette; // call this method if getImageColorDepth() <= 8
	}
	// Is it an indexedColor image
	public boolean isIndexedColor()
	{
		return indexedColor;
	}
    //Returns an integer array representation of the image 
  	//in alpha_RGB sequence
	public int[] getImageData()
    {
	   return pix;// check about null pointer after method call ... 
    }

	public int getHeight() {
		return height;
	}

	public int getWidth() {
		return width;
	}
	
	
}
