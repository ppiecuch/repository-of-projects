package org.tinder.m3g.tutorial.heightmap;

import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;


public class TiledWater
{
  private Mesh waterMesh;
  private VertexBuffer vertexBuffer;
  private IndexBuffer indexBuffer;
  private Appearance appearance;
  public TiledWater(Image2D waterIm, int sz) 
  { 
    /* Make sure the size value is divisible by 2, since
       we'll use size/2 later. */
    int size = (sz/2)*2;
    if (size != sz)
      System.out.println("Size set to multiple of 2: " + size);

    int numTiles = size*size;  
      /* Each tile is a 1-by-1 quadrilateral (quad), so numTiles
         of them will cover a size-by-size water. */
    vertexBuffer = new VertexBuffer(); 
    // build the water mesh
   makeGeometry(size, numTiles);



   makeAppearance(waterIm);

    waterMesh = new Mesh(vertexBuffer, indexBuffer, appearance);
 
    waterMesh.setScale(25, 25, 25) ;

  //  waterMesh.setTranslation(-2000.0f, 1.0f, -2000.0f);
  }  // end of TiledWater()

  public Mesh getWaterMesh()
  {  return waterMesh;  }
  private void makeGeometry(int size, int numTiles)
  /*  numTiles squares centered at the origin on the XZ plane.
      Normals aren't required since the water doesn't reflect
      light. */
  {
    // create vertices
    short[] POINTS = new short[12*numTiles];   // 3 * 4 points for each tile
    int i=0;
    for(int z = (-size/2)+1; z <= size/2; z++)
      for(int x = -size/2; x <= (size/2)-1; x++) {
        POINTS[i] = (short) x; POINTS[i+1]=0; POINTS[i+2] = (short) z;
        POINTS[i+3] = (short)(x+1); POINTS[i+4]=0; POINTS[i+5] = (short) z;
        POINTS[i+6] = (short)(x+1); POINTS[i+7]=0; POINTS[i+8] = (short)(z-1);
        POINTS[i+9] = (short) x; POINTS[i+10]=0; POINTS[i+11] = (short)(z-1);
        i += 12;
      }
    VertexArray POSITION_ARRAY = new VertexArray(POINTS.length/3, 3, 2);
    POSITION_ARRAY.set(0, POINTS.length/3, POINTS);
  
    // create texture coordinates
    short[] TEXCOORDS = new short[8*numTiles];   // 2 * 4 points for each tile
    for(i = 0; i < 8*numTiles; i += 8) {  // {0,1,  1,1,  1,0,  0,0}
      TEXCOORDS[i] = 0; TEXCOORDS[i+1] = 1;               // for each tile
      TEXCOORDS[i+2] = 1; TEXCOORDS[i+3] = 1;
      TEXCOORDS[i+4] = 1; TEXCOORDS[i+5] = 0;
      TEXCOORDS[i+6] = 0; TEXCOORDS[i+7] = 0;
    }
    VertexArray TEXCOORD_ARRAY = new VertexArray(TEXCOORDS.length/2, 2, 2);
    TEXCOORD_ARRAY.set(0, TEXCOORDS.length/2, TEXCOORDS);
    vertexBuffer.setPositions(POSITION_ARRAY, 1.0f, null); // no scale, bias
    vertexBuffer.setTexCoords(0, TEXCOORD_ARRAY, 1.0f, null);
    int pos1 = 1; int pos2 = 2;
    int pos3 = 0; int pos4 = 3;

    int[] INDICES = new int[4*numTiles];   // 4 points for each tile
    for(i = 0; i < 4*numTiles; i += 4) {
      INDICES[i] = pos1; pos1 += 4;   // increment the positions by 4
      INDICES[i+1] = pos2; pos2 += 4;
      INDICES[i+2] = pos3; pos3 +=4;
      INDICES[i+3] = pos4; pos4 += 4;
    }
    int[] LENGTHS = new int[numTiles]; 
    for(i = 0; i < numTiles; i++)
      LENGTHS[i] = 4;
    indexBuffer = new TriangleStripArray(INDICES,LENGTHS);
  }  // end of makeGeometry()

  private void makeAppearance(Image2D waterIm)
  // No Material, with the texture clamped to each tile
  {
    appearance = new Appearance();

    if (waterIm != null) {
      Texture2D texture = new Texture2D(waterIm);
      texture.setFiltering(Texture2D.FILTER_NEAREST, Texture2D.FILTER_NEAREST);
      texture.setWrapping(Texture2D.WRAP_CLAMP, Texture2D.WRAP_CLAMP);
      appearance.setTexture(0, texture);
    }

    // add perspective correction, and switch off all culling
    PolygonMode polygonmode = new PolygonMode();
    polygonmode.setPerspectiveCorrectionEnable(true);
    polygonmode.setCulling(PolygonMode.CULL_NONE);

    appearance.setPolygonMode(polygonmode);

  }  // end of makeAppearance()


  // -------------------- access method ----------------------



} // end of TiledWater Class
