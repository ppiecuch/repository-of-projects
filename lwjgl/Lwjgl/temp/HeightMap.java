package org.tinder.m3g.tutorial.heightmap;
import java.io.IOException;

import javax.microedition.lcdui.Image;
import javax.microedition.m3g.*;

public class HeightMap
{
    // The actual heightmap containing the Y-coords of our indexBuffer
    private short[] heightMap;
    private int[] data;

    
    // Map dimensions
    private int mapWidth;
    private int mapHeight;
    
    // Actual quads
    private Mesh[][] map;
    private static final String  TerrainImgFn ="/heightmap4.png" ;
    private float resolution = 0.25f;
    private int imgw, imgh;    
    // Local transform used for internal calculations
    private Transform localTransform = new Transform();
    
    /**
     * Allocates the internal heightmap and prepares it for rendering.
     * @param imageName The path to the actual heightmap BW image
     * @param resolution The resolution of the heightmap. A value of 1.0 means that each pixel is one quad.
     * @param waterLevel TODO
     * @throws IOException if any error occurs while loading the heightmap image or the texture images.
     */
    public HeightMap() throws IOException
    {
        // Check for invalid resolution values
        if(resolution <= 0.0001f || resolution > 1.0f)
            throw new IllegalArgumentException("Resolution too small or too large");
        
        // Load image and allocate the internal array
        loadMapImage();
        
        // Create quads
        createQuads();

    }
    public Mesh[][]getQuads(){
      return map;	
    	
    }
    public int getMapWidth(){
      return mapWidth;
    }

    public int getMapHeight(){
      return mapHeight;
    }
    /**
     * Create all quads (Triangle pairs) that our 
     */
    private void createQuads()
    {
        map = new Mesh[mapWidth][mapHeight];
        short[] heights = new short[4];
        
        for(int x = 0; x < (mapWidth - 1); x++)
        {
            for(int y = 0; y < (mapHeight - 1); y++)
            {
                // Set heights
               heights[0] = heightMap[x + y * mapWidth];
               heights[1] = heightMap[x + y * mapWidth + 1];
               heights[3] = heightMap[x + (y + 1) * mapWidth];
               heights[2] = heightMap[x + (y + 1) * mapWidth + 1];
                
                // Create mesh
                map[x][y] = createQuad(heights);
            }
        }
                
    }


    private void loadMapImage() throws IOException
    {
        // Load actual image
        Image img = Image.createImage(TerrainImgFn);
        
        // Allocate temporary memory to store pixels
        data = new int[img.getWidth() * img.getHeight()];
        
        // Get its rgb values
        img.getRGB(data, 0, img.getWidth(), 0, 0, img.getWidth(), img.getHeight());
        
        imgw = img.getWidth();
        imgh = img.getHeight();
        
        // Clear image
        img = null;
        System.gc();
        
        // Calculate new width and height
        mapWidth = (int)(resolution * imgw);
        mapHeight = (int)(resolution * imgh);
        
        // Allocate heightmap
        heightMap = new short[mapWidth * mapHeight];
        
        // Calculate height and width offset into image
        int xoff = imgw / mapWidth;
        int yoff = imgh / mapHeight;
        
        // Set height values
        for(int y = 0; y < mapHeight; y++)
        {
            for(int x = 0; x < mapWidth; x++)
            {
                heightMap[x + y * mapWidth] = (short)((data[x * xoff + y * yoff * imgw] & 0x000000ff) * 10);
            }
        }        

        // Clear data
        data = null;
        img = null;
        System.gc();
    }
    
    /**
     * Renders this heightmap using a specified graphics context and a transform.
     * @param g3d
     * @param t
     */
    public void setTransform(Transform t)
    {
        for(int x = 0; x < map.length - 1; x++)
        {
            for(int y = 0; y < map[x].length - 1; y++)
            {
                localTransform.setIdentity();
                
                localTransform.postTranslate(x * 5.1f, 0.0f, (mapHeight - y) * -5.1f);
                localTransform.postScale(0.01f, 0.01f, 0.01f);
                localTransform.postMultiply(t);
                map[x][y].setTransform(localTransform);
            }
        }



    }
        
    /**
     * Creates a colored quad consisting of two indexBuffer.
     * The heights parameter is an array that holds the heights of the new quad.
     * The array must have a length of 4 and goes counter-clockwise starting from the
     * northwestern position of the quad (-1, -1)
     * @param heights The height array, denoting the heights (y-coords) of the four corners.
     * @param cullFlags
     * @param texFilename
     * @return
     */
    public static Mesh createQuad(short[] heights)
    {
        // The POINTS of the quad
        short[] POINTS = {-255, heights[0], -255,
                255, heights[1], -255,
                255, heights[2], 255,
                -255, heights[3], 255}; 
        
        // The arrays
        VertexArray POSITION_ARRAY;
        IndexBuffer indexBuffer;

        // Create the model's POINTS
        POSITION_ARRAY = new VertexArray(POINTS.length/3, 3, 2);
        POSITION_ARRAY.set(0, POINTS.length/3, POINTS);
        
        // Allocate COLORS array
        byte[] COLORS = new byte[12];
        
        for(int i = 0; i < heights.length; i++)
        {
            int j = i * 3;
	        // Altitude check
            if(heights[i] >= 1000)
            {
                byte col = (byte)(57 + (heights[i] / 1550.0f) * 70);
                COLORS[j] = col;
                COLORS[j + 1] = col;
                COLORS[j + 2] = col;
            }
	        else
	        {
	            byte gCol = 110;
	            byte bCol = 25;
	            
	            COLORS[j] = 0;
	            COLORS[j + 1] = (byte)(gCol - (heights[i] / 1000.0f) * 85);
	            COLORS[j + 2] = (byte)(bCol - (heights[i] / 1000.0f) * 20);
	        }
        }
        
        // Create the model's vertex colors
        VertexArray COLOR_ARRAY = new VertexArray(COLORS.length/3, 3, 1);
        COLOR_ARRAY.set(0, COLORS.length / 3, COLORS);
        
        // Compose a VertexBuffer out of the previous POINTS and texture coordinates
        VertexBuffer vertexBuffer = new VertexBuffer();
        vertexBuffer.setPositions(POSITION_ARRAY, 1.0f, null);
        vertexBuffer.setColors(COLOR_ARRAY);
        
        // Create INDICES and face lengths
        int INDICES[] = new int[] {0, 1, 3, 2};
        int[] LENGTHS = new int[] {4};
        
        // Create the model's indexBuffer
        indexBuffer = new TriangleStripArray(INDICES, LENGTHS);

        // Create the appearance
        Appearance appearance = new Appearance();
        PolygonMode polygonmode = new PolygonMode();
        polygonmode.setCulling(PolygonMode.CULL_NONE);
        polygonmode.setPerspectiveCorrectionEnable(true);
        polygonmode.setShading(PolygonMode.SHADE_SMOOTH);
        appearance.setPolygonMode(polygonmode);
        
        // Finally create the Mesh
        Mesh mesh = new Mesh(vertexBuffer, indexBuffer, appearance);

        // All done
        return mesh;
    }
  
}
