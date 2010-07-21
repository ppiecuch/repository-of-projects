package org.tinder.studio.lwjgl.roam;
/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-21ÉÏÎç12:38:00
 *
 */
public class Landscape {
	
	private static final int POOL_SIZE=25000;	// How many TriTreeNodes should be allocated?
	private static TriTriangleNode[] pool=new TriTriangleNode[POOL_SIZE];	// Pool of TriTree nodes for splitting
	public static int nextTriNode;				// Index to next free TriTreeNode
	
	private Landscape(){}
	
	/**
	 * Initialize all patches
	 */
	public static void init()
	{
		/*// ---------------------------------------------------------------------
		// Initialize all patches
		//
		void Landscape::Init(unsigned char *hMap)
		{
			Patch *patch;
			int X, Y;
		
			// Store the Height Field array
			m_HeightMap = hMap;
		
			// Initialize all terrain patches
			for ( Y=0; Y < NUM_PATCHES_PER_SIDE; Y++)
				for ( X=0; X < NUM_PATCHES_PER_SIDE; X++ )
				{
					patch = &(m_Patches[Y][X]);
					patch->Init( X*PATCH_SIZE, Y*PATCH_SIZE, X*PATCH_SIZE, Y*PATCH_SIZE, hMap );
					patch->ComputeVariance();
				}
		}*/
		
	}
	
	/**
	 * Allocate a TriTreeNode from the pool.
	 * @return
	 */
	public static TriTriangleNode allocateTri(){
		if(nextTriNode>=POOL_SIZE)
			return null;
		TriTriangleNode node=pool[nextTriNode++];
		node.leftChild=node.rightChild=null;
		return node;
	}
	
	/**
	 * Reset all patches, recompute variance if needed
	 */
	public static void reset()
	{
		/*
		 * //
	// Perform simple visibility culling on entire patches.
	//   - Define a triangle set back from the camera by one patch size, following
	//     the angle of the frustum.
	//   - A patch is visible if it's center point is included in the angle: Left,Eye,Right
	//   - This visibility test is only accurate if the camera cannot look up or down significantly.
	//
	const float PI_DIV_180 = M_PI / 180.0f;
	const float FOV_DIV_2 = gFovX/2;

	int eyeX = (int)(gViewPosition[0] - PATCH_SIZE * sinf( gClipAngle * PI_DIV_180 ));
	int eyeY = (int)(gViewPosition[2] + PATCH_SIZE * cosf( gClipAngle * PI_DIV_180 ));

	int leftX  = (int)(eyeX + 100.0f * sinf( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));
	int leftY  = (int)(eyeY - 100.0f * cosf( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));

	int rightX = (int)(eyeX + 100.0f * sinf( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));
	int rightY = (int)(eyeY - 100.0f * cosf( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));

	int X, Y;
	Patch *patch;

	// Set the next free triangle pointer back to the beginning
	SetNextTriNode(0);

	// Reset rendered triangle count.
	gNumTrisRendered = 0;

	// Go through the patches performing resets, compute variances, and linking.
	for ( Y=0; Y < NUM_PATCHES_PER_SIDE; Y++ )
		for ( X=0; X < NUM_PATCHES_PER_SIDE; X++)
		{
			patch = &(m_Patches[Y][X]);
			
			// Reset the patch
			patch->Reset();
			patch->SetVisibility( eyeX, eyeY, leftX, leftY, rightX, rightY );
			
			// Check to see if this patch has been deformed since last frame.
			// If so, recompute the varience tree for it.
			if ( patch->isDirty() )
				patch->ComputeVariance();

			if ( patch->isVisibile() )
			{
				// Link all the patches together.
				if ( X > 0 )
					patch->GetBaseLeft()->LeftNeighbor = m_Patches[Y][X-1].GetBaseRight();
				else
					patch->GetBaseLeft()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

				if ( X < (NUM_PATCHES_PER_SIDE-1) )
					patch->GetBaseRight()->LeftNeighbor = m_Patches[Y][X+1].GetBaseLeft();
				else
					patch->GetBaseRight()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

				if ( Y > 0 )
					patch->GetBaseLeft()->RightNeighbor = m_Patches[Y-1][X].GetBaseRight();
				else
					patch->GetBaseLeft()->RightNeighbor = NULL;		// Link to bordering Landscape here..

				if ( Y < (NUM_PATCHES_PER_SIDE-1) )
					patch->GetBaseRight()->RightNeighbor = m_Patches[Y+1][X].GetBaseLeft();
				else
					patch->GetBaseRight()->RightNeighbor = NULL;	// Link to bordering Landscape here..
			}
		}
		 */
	}
	
	/**
	 * Create an approximate mesh of the landscape.
	 */
	public static void tessellate()
	{
		/*
		 * // Perform Tessellation
	int nCount;
	Patch *patch = &(m_Patches[0][0]);
	for (nCount=0; nCount < NUM_PATCHES_PER_SIDE*NUM_PATCHES_PER_SIDE; nCount++, patch++ )
	{
		if (patch->isVisibile())
			patch->Tessellate( );
	}
		 */
	}
	
	/**
	 * Render each patch of the landscape & adjust the frame variance.
	 */
	public static void render()
	{
		/*
		 *int nCount;
	Patch *patch = &(m_Patches[0][0]);

	// Scale the terrain by the terrain scale specified at compile time.
	glScalef( 1.0f, MULT_SCALE, 1.0f );

	for (nCount=0; nCount < NUM_PATCHES_PER_SIDE*NUM_PATCHES_PER_SIDE; nCount++, patch++ )
	{
		if (patch->isVisibile())
			patch->Render();
	}

	// Check to see if we got close to the desired number of triangles.
	// Adjust the frame variance to a better value.
	if ( GetNextTriNode() != gDesiredTris )
		gFrameVariance += ((float)GetNextTriNode() - (float)gDesiredTris) / (float)gDesiredTris;

	// Bounds checking.
	if ( gFrameVariance < 0 )
		gFrameVariance = 0; 
		 */
	}

}
