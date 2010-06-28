package glapp;

import java.util.ArrayList;
import org.lwjgl.opengl.GL11;
import glmodel.*;

/**
 * GLLine creates a quad-strip line from a series of points.  To use:
 * <PRE>
 * draw() {
 *    GLLine L = new GLLine();
 *    L.setWidth(20);
 *    L.point(0,0);
 *    L.point(5,5);
 *    L.point(0,10);
 *    L.point(5,15);
 *    L.close();   // if you want a closed shape
 *    L.draw();
 * }
 * </PRE>
 * <P>
 * NOTE: gets ugly when points are very close, or reverse direction sharply.  Uses
 * my home-grown trigonometry... probably not the best way to do this, but it works
 * in most cases.
 * <P>
 * ALSO NOTE: the line will be drawn with the active texture.  To draw an untextured
 * line, call activateTexture(0) or disable textures (GL11.glDisable(GL11.GL_TEXTURE_2D)
 * before calling line.draw().
 */
public class GLLine {
	ArrayList points;          // holds each point in line (see point())
	float w2 = 1f;             // half of line width
	boolean changed = false;   // true if line was changed since last rebuild()
	float uvfactor = 100f;     // how to tile texture on line: if uvfactor=100 texture will repeat every 100 units

	float[][] verts;  // to hold vertex positions
	private int vc;   // vertex counter


	public GLLine() {
		clear();
	}

	public GLLine(float lineWidth) {
		clear();
		setWidth(lineWidth);
	}

	/**
	 * reset the line
	 */
	public void clear() {
		points = new ArrayList();
		verts = null;
	}

	/**
	 * Add an xy point to the line.
	 */
	public void point(float x, float y) {
		GL_Vector p = new GL_Vector(x,y,0);
		/*
		// eliminate segments shorter than 10 units
		// in hand drawn lines very short segments (around 1 unit long) give erratic results
		if (points.size() > 0) {
			if ( GL_Vector.sub(p,(GL_Vector)points.get(points.size()-1)).length() < 10) {
				return;
			}
		}
		*/
		points.add(p);
		changed = true;
	}

	/**
	 * Close the line by adding a line segment that connects the last point to the first.
	 */
	public void close() {
		int numPoints = points.size();
		if (numPoints >= 3) {
			GL_Vector firstP = (GL_Vector)points.get(0);       // first point in shape
			GL_Vector lastP = (GL_Vector)points.get(points.size()-1);  // last point in shape
			// close the shape if not already closed
			if (!lastP.equals(firstP)) {
				point(firstP.x, firstP.y);
			}
		}
	}

	/**
	 * set line width
	 */
	public void setWidth(float w) {
		w2 = w/2f;
		changed = true;
	}

	/**
	 * return line width
	 */
	public float getWidth() {
		return w2 * 2;
	}

	/**
	 * return number of points in this line
	 */
	public int getNumPoints() {
		return points.size();
	}

	/**
	 * return the given xy point
	 */
	public GL_Vector getPoint(int n) {
		return (n > 0 && n < points.size())? (GL_Vector)points.get(n) : null;
	}

	/**
	 * set texture tiling factor.  Texture will repeat every <uvfactor> units.
	 * If a line is 50 units long, and uvfactor is 10, then the texture will
	 * repeat five times along the length of the line.
	 */
	public void setUVfactor(float tilefactor) {
		uvfactor = tilefactor;
		changed = true;
	}

	/**
	 * regenerate the line geometry from the added points.
	 */
	public void rebuild() {
		makeLine();
	}

	/**
	 * create the line geometry from point positions
	 */
	public void makeLine() {
		// need at least two points
		if (points.size() < 2) {
			return;
		}

		// create array to hold vertex positions and reset vert counter
		verts = new float[(points.size()*4)-4][3];
		vc = 0;

		GL_Vector firstP = (GL_Vector) points.get(0);
		GL_Vector lastP = (GL_Vector) points.get(points.size()-1);

		for (int i=0; i < points.size()-1; i++) {
			GL_Vector p1 = (GL_Vector) points.get(i);
			GL_Vector p2 = (GL_Vector) points.get(i+1);
			// default prev and next line segment directions are the same as this segment direction
			GL_Vector v1 = new GL_Vector(p2, p1);
			GL_Vector v2 = new GL_Vector(p2, p1);
			// get end directions of line segment (will change the shape of the segment so it joins with next segment)
			// special case: first segment of closed shape
			if (i == 0 && firstP.equals(lastP))  {
				// if first point and last point are the same, first segment will join last
				v1 = new GL_Vector(lastP,  (GL_Vector)points.get(points.size()-2));
			}
			// previous line segment direction
			if (i > 0) {
				v1 = new GL_Vector(p1, (GL_Vector)points.get(i-1));
			}
			// next line segment direction
			if (i < points.size()-2) {
				v2 = new GL_Vector((GL_Vector)points.get(i+2), p2);
			}
			// draw the line segment
			makeSegment(p1,p2,v1,v2);
		}

		// if first point and last point are the same, join last segment to first
		if (firstP.equals(lastP)) {
			// move last vert to same position as first
			verts[verts.length-1][0] = verts[0][0];
			verts[verts.length-1][1] = verts[0][1];
			// move second to last vert to same position as second
			verts[verts.length-2][0] = verts[1][0];
			verts[verts.length-2][1] = verts[1][1];
		}

		changed = false;
	}

	/**
	 * Draw one segment from a line.  The segment is a quad created by
	 * finding normals to the line vector.  The line is drawn in the XY plane.
	 * In the diagram below, qv1 thru qv4 are the four quad vectors.  N1 and N2 are
	 * normals to the line vector and Z axis.  The normals are averaged with the
	 * previous and next line segments so that this segment will join gracefully
	 * with the previous and next.
	 * <PRE>
	 *      qv4   p2    qv3
	 *        +----o----+
	 *         -N2 |  N2
	 *             |
	 *             |          line segment p1-p2
	 *             |
	 *         -N1 |  N1
	 *        +----o----+
	 *      qv1   p1    qv2
	 * </PRE>
	 *
	 * @param p1      start point of line segment
	 * @param p2      end point of line segment
	 * @param prevV   direction of previous line segment
	 * @param postV   direction of next line segment
	 */
	public void makeSegment(GL_Vector p1, GL_Vector p2, GL_Vector prevV, GL_Vector postV) {
		// vectors of line segments
		GL_Vector v0 = prevV;                   // direction of prior line segment
		GL_Vector v1 = GL_Vector.sub(p2,p1);    // direction of segment we're drawing now
		GL_Vector v2 = postV;                   // direction of next line segment

		// normals to form the line joins
		GL_Vector N1 = getNormal(v0,v1,w2);     // average normal for previous and current segment
		GL_Vector N2 = getNormal(v1,v2,w2);     // average normal for current and next segment

		// make the four quad verts for the segment
		GL_Vector qv2 = GL_Vector.add(p1, N1);
		GL_Vector qv1 = GL_Vector.add(p1, N1.reverse());
		GL_Vector qv3 = GL_Vector.add(p2, N2);
		GL_Vector qv4 = GL_Vector.add(p2, N2.reverse());

		// add the verts to array
		verts[vc][0] = qv1.x;		verts[vc][1] = qv1.y;		verts[vc][2] = qv1.z;  vc++;
		verts[vc][0] = qv2.x;		verts[vc][1] = qv2.y;		verts[vc][2] = qv2.z;  vc++;
		verts[vc][0] = qv3.x;		verts[vc][1] = qv3.y;		verts[vc][2] = qv3.z;  vc++;
		verts[vc][0] = qv4.x;		verts[vc][1] = qv4.y;		verts[vc][2] = qv4.z;  vc++;
	}

	/**
	 * Make an average normal to the two vectors with the right length to
	 * make a join for a line <width> wide.
	 * <PRE>
	 *           + - - - - - -
	 *           |\ normal
	 *           | \
	 *           |  o-----------o line b
	 *              |
	 *              |
	 *              |
	 *              o line a
	 * </PRE>
	 * @param a
	 * @param b
	 * @return
	 */
	public GL_Vector getNormal(GL_Vector a, GL_Vector b, float width) {
		// Z axis
		GL_Vector Z = new GL_Vector(0,0,1);
		// get normal to line a and b (normals are in the XY plane)
		GL_Vector cross1 = GL_Vector.crossProduct(a,Z);
		GL_Vector cross2 = GL_Vector.crossProduct(b,Z);
		// average the two normals
		GL_Vector crossAvg = GL_Vector.add(cross1,cross2).div(2).normalize();
		// adjust the length of the normal
		float N1 = getNormalLen(cross1,crossAvg,width);
		crossAvg.mult(N1);
		return crossAvg;
	}

	/**
	 * Return the length of the normal needed to make a join in a line
	 * of the given width.
	 * @param v1
	 * @param v2
	 * @param width
	 * @return
	 */
	public float getNormalLen(GL_Vector v1, GL_Vector v2, float width) {
		float A = 180 - GL_Vector.angleXY(v1,v2);
		float normlen = width/(float)Math.cos(Math.toRadians(A));
		return normlen;
	}

	/**
	 * for debugging
	 * @param v
	 * @param x
	 * @param y
	 */
	public void drawV(GL_Vector v, float x, float y) {
		GL11.glColor3f(0,1,1);
		GL11.glPushMatrix(); {
			GL11.glBegin(GL11.GL_LINES);
			GL11.glVertex3f(x,y,0);
			GL11.glVertex3f(x+v.x, y+v.y, 0);
			GL11.glEnd();
		}
		GL11.glPopMatrix();
	}

	/**
	 * draw the mesh geometry created by rebuild()
	 */
	public void draw() {
		// rebuild if points were added or width changed
		if (changed) {
			makeLine();
		}
		// line is not empty, draw it
		if (verts != null && verts.length >= 0) {
	        // normal is on Z axis
			GL11.glNormal3f(0,0,1);

			// draw the quads
			for (int i=0; i < verts.length; i+=4) {
				GL11.glBegin(GL11.GL_QUADS);
				{
					GL11.glTexCoord2f(verts[i+0][0]/uvfactor,verts[i+0][1]/uvfactor);
					GL11.glVertex3f(verts[i+0][0],verts[i+0][1],verts[i+0][2]);

					GL11.glTexCoord2f(verts[i+1][0]/uvfactor,verts[i+1][1]/uvfactor);
					GL11.glVertex3f(verts[i+1][0],verts[i+1][1],verts[i+1][2]);

					GL11.glTexCoord2f(verts[i+2][0]/uvfactor,verts[i+2][1]/uvfactor);
					GL11.glVertex3f(verts[i+2][0],verts[i+2][1],verts[i+2][2]);

					GL11.glTexCoord2f(verts[i+3][0]/uvfactor,verts[i+3][1]/uvfactor);
					GL11.glVertex3f(verts[i+3][0],verts[i+3][1],verts[i+3][2]);
				}
				GL11.glEnd();
			}
		}
	}

	/**
	 * draw the mesh geometry created by rebuild() as a 3D shape (extrude the line shape
	 * along the Z axis).
	 */
	public void draw3D(float height) {
		// rebuild if points were added or width changed
		if (changed) {
			makeLine();
		}
		// if line is not empty, draw it
		if (verts != null && verts.length > 0) {
			float z = height;   // how high to extrude line
			GL_Vector n;        // normal of quad

			// draw the quads
			for (int i=0; i < verts.length; i+=4) {
				// quad facing forward
				GL11.glNormal3f(0,0,1);
				GL11.glBegin(GL11.GL_QUADS);
				{
					GL11.glTexCoord2f(verts[i+0][0]/uvfactor,verts[i+0][1]/uvfactor);
					GL11.glVertex3f(verts[i+0][0],verts[i+0][1],verts[i+0][2]);

					GL11.glTexCoord2f(verts[i+1][0]/uvfactor,verts[i+1][1]/uvfactor);
					GL11.glVertex3f(verts[i+1][0],verts[i+1][1],verts[i+1][2]);

					GL11.glTexCoord2f(verts[i+2][0]/uvfactor,verts[i+2][1]/uvfactor);
					GL11.glVertex3f(verts[i+2][0],verts[i+2][1],verts[i+2][2]);

					GL11.glTexCoord2f(verts[i+3][0]/uvfactor,verts[i+3][1]/uvfactor);
					GL11.glVertex3f(verts[i+3][0],verts[i+3][1],verts[i+3][2]);
				}
				GL11.glEnd();
				
				// quad facing backward
				GL11.glNormal3f(0,0,-1);
				GL11.glBegin(GL11.GL_QUADS);
				{
					GL11.glTexCoord2f(verts[i+1][0]/uvfactor,verts[i+1][1]/uvfactor);
					GL11.glVertex3f(verts[i+1][0],verts[i+1][1],verts[i+1][2]-z);

					GL11.glTexCoord2f(verts[i+0][0]/uvfactor,verts[i+0][1]/uvfactor);
					GL11.glVertex3f(verts[i+0][0],verts[i+0][1],verts[i+0][2]-z);

					GL11.glTexCoord2f(verts[i+3][0]/uvfactor,verts[i+3][1]/uvfactor);
					GL11.glVertex3f(verts[i+3][0],verts[i+3][1],verts[i+3][2]-z);

					GL11.glTexCoord2f(verts[i+2][0]/uvfactor,verts[i+2][1]/uvfactor);
					GL11.glVertex3f(verts[i+2][0],verts[i+2][1],verts[i+2][2]-z);
				}
				GL11.glEnd();
				
				// quad facing side 1  (calc normal from three points on side plane)
				n = GL_Vector.getNormal(new GL_Vector(verts[i+3][0],verts[i+3][1],verts[i+3][2]-z),
										new GL_Vector(verts[i+0]),
										new GL_Vector(verts[i+3]));
				GL11.glNormal3f(n.x,n.y,n.z);
				GL11.glBegin(GL11.GL_QUADS);
				{
					GL11.glTexCoord2f(0/uvfactor,verts[i+0][1]/uvfactor);
					GL11.glVertex3f(verts[i+0][0],verts[i+0][1],verts[i+0][2]-z);

					GL11.glTexCoord2f(z/uvfactor,verts[i+0][1]/uvfactor);
					GL11.glVertex3f(verts[i+0][0],verts[i+0][1],verts[i+0][2]);

					GL11.glTexCoord2f(z/uvfactor,verts[i+3][1]/uvfactor);
					GL11.glVertex3f(verts[i+3][0],verts[i+3][1],verts[i+3][2]);

					GL11.glTexCoord2f(0/uvfactor,verts[i+3][1]/uvfactor);
					GL11.glVertex3f(verts[i+3][0],verts[i+3][1],verts[i+3][2]-z);
				}
				GL11.glEnd();
				
				// quad facing side 2 (calc normal from three points on side plane)
				n = GL_Vector.getNormal(new GL_Vector(verts[i+1][0],verts[i+1][1],verts[i+1][2]-z),
										new GL_Vector(verts[i+1]),
										new GL_Vector(verts[i+2]));
				GL11.glNormal3f(n.x,n.y,n.z);
				GL11.glBegin(GL11.GL_QUADS);
				{
					GL11.glTexCoord2f(0/uvfactor,verts[i+1][1]/uvfactor);
					GL11.glVertex3f(verts[i+1][0],verts[i+1][1],verts[i+1][2]);

					GL11.glTexCoord2f(z/uvfactor,verts[i+1][1]/uvfactor);
					GL11.glVertex3f(verts[i+1][0],verts[i+1][1],verts[i+1][2]-z);

					GL11.glTexCoord2f(z/uvfactor,verts[i+2][1]/uvfactor);
					GL11.glVertex3f(verts[i+2][0],verts[i+2][1],verts[i+2][2]-z);

					GL11.glTexCoord2f(0/uvfactor,verts[i+2][1]/uvfactor);
					GL11.glVertex3f(verts[i+2][0],verts[i+2][1],verts[i+2][2]);

				}
				GL11.glEnd();
			}
		}
	}

	/**
	 * render the line into a display list and return the list ID.
	 */
	public int makeDisplayList() {
		int DLID = GLApp.beginDisplayList();
		draw();
		GLApp.endDisplayList();
		return DLID;
	}

	/**
	 * return a GLLine shaped into a rectangle.
	 */
	public static GLLine makeRectangle(float x, float y, float w, float h, float linewidth) {
		GLLine rect = new GLLine(linewidth);
		rect.point(x, y);
		rect.point(x+w, y);
		rect.point(x+w, y+h);
		rect.point(x, y+h);
		rect.close();
		return rect;
	}

    /**
     * return a GLLine shaped into a circle.  Can also make triangle, square, pentagon, etc. by
     * giving a low number for numSegments.
     */
    public static GLLine makeCircle(float x, float y, float radius, int numSegments, float linewidth) {
        int s = 0;     // start
        int e = 360;   // end
        int stepSize = 360/numSegments;   // degrees per segment
        GLLine L = new GLLine(linewidth);
        // add first point
        float ts = (float) Math.sin(Math.toRadians(s));
        float tc = (float) Math.cos(Math.toRadians(s));
        L.point(x+tc*radius, y+ts*radius);
        // add intermediate points, snap to {step} degrees
        while ( (s = ((s+stepSize)/stepSize)*stepSize) < e) {
        	ts = (float) Math.sin(Math.toRadians(s));
        	tc = (float) Math.cos(Math.toRadians(s));
        	L.point(x+tc*radius, y+ts*radius);
        }
        // add last point
        L.close();
        return L;
    }

}
