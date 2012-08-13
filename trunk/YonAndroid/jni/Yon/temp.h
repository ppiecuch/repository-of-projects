class Patch
{
	//TODO ¸ÄÎª·Çstatic
	// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
	const static s32 VARIANCE_DEPTH;
protected:
	//u8 *m_HeightMap;								// Pointer to height map to use
	//s32 m_WorldX, m_WorldY;							// World coordinate offset of this patch.
	u16 m_uIndex;

	u8 m_VarianceLeft[1<<(VARIANCE_DEPTH)];			// Left variance tree
	u8 m_VarianceRight[1<<(VARIANCE_DEPTH)];		// Right variance tree

	u8 *m_CurrentVariance;							// Which varience we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
	u8 m_VarianceDirty;								// Does the Varience Tree need to be recalculated for this Patch?
	bool m_isVisible;								// Is this patch visible in the current frame?

	TriTreeNode m_BaseLeft;							// Left base triangle tree node
	TriTreeNode m_BaseRight;						// Right base triangle tree node

public:
	// Some encapsulation functions & extras
	TriTreeNode *getBaseLeft(){ return &m_BaseLeft; }
	TriTreeNode *getBaseRight(){ return &m_BaseRight; }
	u8 isDirty(){ return m_VarianceDirty; }
	bool isVisibile(){ return m_isVisible; }
	void setVisibility(s32 eyeX, s32 eyeY, s32 leftX, s32 leftY, s32 rightX, s32 rightY);

	// The static half of the Patch Class
	//virtual void init(s32 heightX, s32 heightY, s32 worldX, s32 worldY, u8 *hMap);
	void init(u16 index);
	void reset();
	void tessellate();
	void render();
	void computeVariance();

	// The recursive half of the Patch Class
	void split(TriTreeNode *tri);
	void recursTessellate( TriTreeNode *tri, s32 leftX, s32 leftY, s32 rightX, s32 rightY, s32 apexX, s32 apexY, s32 node);
	void recursRender(TriTreeNode *tri, s32 leftX, s32 leftY, s32 rightX, s32 rightY, s32 apexX, s32 apexY);
	u8 recursComputeVariance(s32 leftX,s32 leftY,u8 leftZ, s32 rightX, s32 rightY, u8 rightZ, s32 apexX,  s32 apexY, u8 apexZ, s32 node);
};


class Landscape {
public:
	// Initialize the whole process
	void init(u8 *hMap);
	// Reset for a new frame
	void reset();
	// Create mesh approximation
	void tessellate();
	// Render current mesh static
	void render();
	// Allocate a new node for the mesh
	TriTreeNode *allocateTri();

protected:
	// Index to the next free TriTreeNode
	static int m_NextTriNode;
	// Pool of nodes for tessellation
	static TriTreeNode m_TriPool[];
	// Array of patches to be rendered
	Patch m_aPatches[][];
	// Pointer to Height Field data
	u8 *m_HeightMap;

};