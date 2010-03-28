package org.tinder.m3g.tutorial.collsion.aabb;
import java.lang.Math;
import javax.microedition.m3g.Transform;

class AABB {
	float[] min = new float[] { 0.0f, 0.0f, 0.0f };

	float[] max = new float[] { 0.0f, 0.0f, 0.0f };

	float[] center = new float[] { 0.0f, 0.0f, 0.0f };

	public AABB() {

	}

	float xSize() {
		return (max[0] - min[0]);
	}

	float ySize() {
		return (max[1] - min[1]);
	}

	float zSize() {
		return (max[2] - min[2]);
	}

	float[] getMin() {
		return min;
	}

	float[] getMax() {
		return max;
	}

	void setMin(float x, float y, float z) {
		min[0] = x;
		min[1] = y;
		min[2] = z;
	}

	void setMax(float x, float y, float z) {
		max[0] = x;
		max[1] = y;
		max[2] = z;
	}

	float[] center() {
		center[0] = (min[0] + max[0]) * 0.5f;
		center[1] = (min[1] + max[1]) * 0.5f;
		center[2] = (min[2] + max[2]) * 0.5f;
		return center;
	}

	float size() {
		return (max[0] - min[0]) * (max[1] - min[1]) * (max[2] - min[2]);
	}

	void reset() {
		for (int i = 0; i < 3; i++) {
			min[i] = 0;
			max[i] = 0;
		}
	}

	void add(float[] p) {
		// Expand the box as necessary to contain the point.
		if (p[0] < min[0])
			min[0] = p[0];
		if (p[0] > max[0])
			max[0] = p[0];
		if (p[1] < min[1])
			min[1] = p[1];
		if (p[1] > max[1])
			max[1] = p[1];
		if (p[2] < min[2])
			min[2] = p[2];
		if (p[2] > max[2])
			max[2] = p[2];
	}

	boolean intersectAABBs(AABB box2, AABB boxIntersect) {
		float[] box2_min = box2.getMin();
		float[] box2_max = box2.getMax();
		// Check for no overlap
		if (min[0] > box2_max[0])
			return false;
		if (max[0] < box2_min[0])
			return false;
		if (min[1] > box2_max[1])
			return false;
		if (max[1] < box2_min[1])
			return false;
		if (min[2] > box2_max[2])
			return false;
		if (max[2] < box2_min[2])
			return false;
		// We have overlap. Compute AABB of intersection, if they want it
		if (boxIntersect != null) {
			float[] box_intersect_min = new float[3];
			float[] box_intersect_max = new float[3];
			box_intersect_min[0] = Math.max(min[0], box2_min[0]);
			box_intersect_max[0] = Math.min(max[0], box2_max[0]);
			box_intersect_min[1] = Math.max(min[1], box2_min[1]);
			box_intersect_max[1] = Math.min(max[1], box2_max[1]);
			box_intersect_min[2] = Math.max(min[2], box2_min[2]);
			box_intersect_max[2] = Math.min(max[2], box2_max[2]);
		}
		// They intersected
		return true;
	}

	boolean isEmpty() {
		// Check if we're inverted on any axis
		return (min[0] > max[0]) || (min[1] > max[1]) || (min[2] > max[2]);
	}

	boolean contains(float[] p) {
		// Check for overlap on each axis
		return (p[0] >= min[0]) && (p[0] <= max[0]) && (p[1] >= min[1])
				&& (p[1] <= max[1]) && (p[2] >= min[2]) && (p[2] <= max[2]);
	}

	void setToTransformedBox(Transform t) {
		// If we're empty, then bail
		if (isEmpty()) {
			return;
		}
		float[] m = new float[16];
		t.get(m);

		// Examine each of the nine matrix elements
		// and compute the new AABB

		// 0 1 2 3
		// 4 5 6 7
		// 8 9 10 11
		// 12 13 14 15
		float minx = 0, miny = 0, minz = 0;
		float maxx = 0, maxy = 0, maxz = 0;
		minx += m[3];
		maxx += m[3];
		miny += m[7];
		maxy += m[7];
		minz += m[11];
		maxz += m[11];
		if (m[0] > 0.0f) {
			minx += m[0] * min[0];
			maxx += m[0] * max[0];
		} else {
			minx += m[0] * max[0];
			maxx += m[0] * min[0];
		}
		if (m[1] > 0.0f) {
			minx += m[1] * min[1];
			maxx += m[1] * max[1];
		} else {
			minx += m[1] * max[1];
			maxx += m[1] * min[1];
		}
		if (m[2] > 0.0f) {
			minx += m[2] * min[2];
			maxx += m[2] * max[2];
		} else {
			minx += m[2] * max[2];
			maxx += m[2] * min[2];
		}
		if (m[4] > 0.0f) {
			miny += m[4] * min[0];
			maxy += m[4] * max[0];
		} else {
			miny += m[4] * max[0];
			maxy += m[4] * min[0];
		}
		if (m[5] > 0.0f) {
			miny += m[5] * min[1];
			maxy += m[5] * max[1];
		} else {
			miny += m[5] * max[1];
			maxy += m[5] * min[1];
		}
		if (m[6] > 0.0f) {
			miny += m[6] * min[2];
			maxy += m[6] * max[2];
		} else {
			miny += m[6] * max[2];
			maxy += m[6] * min[2];
		}
		if (m[8] > 0.0f) {
			minz += m[8] * min[0];
			maxz += m[8] * max[0];
		} else {
			minz += m[8] * max[0];
			maxz += m[8] * min[0];
		}
		if (m[9] > 0.0f) {
			minz += m[9] * min[1];
			maxz += m[9] * max[1];
		} else {
			minz += m[9] * max[1];
			maxz += m[9] * min[1];
		}
		if (m[10] > 0.0f) {
			minz += m[10] * min[2];
			maxz += m[10] * max[2];
		} else {
			minz += m[10] * max[2];
			maxz += m[10] * min[2];
		}

		min[0] = minx;
		min[1] = miny;
		min[2] = minz;
		max[0] = maxx;
		max[1] = maxy;
		max[2] = maxz;
	}

}