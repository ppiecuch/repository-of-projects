package org.tinder.studio.lwjgl.roam;
/**
 *
 * Landscape管理大的地形块，协调块与块之间的沟通。它相当于地形渲染所有细节的封装体。
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-21上午12:38:00
 *
 */
public class Landscape {
	
	private static final int POOL_SIZE=25000;	// How many TriTreeNodes should be allocated?
	private static TriTreeNode[] pool=new TriTreeNode[POOL_SIZE];	// Pool of TriTree nodes for splitting
	public static int nextTriNode;				// Index to next free TriTreeNode
	private Patch[][] patchs;
	private int[] heightMap;
	
	private static int NUM_PATCHES_PER_SIDE;
	private static int MAP_SIZE;
	private static int PATCH_SIZE=(MAP_SIZE/NUM_PATCHES_PER_SIDE);
	
	// Desired number of Binary Triangle tessellations per frame.
	// This is not the desired number of triangles rendered!
	// There are usually twice as many Binary Triangle structures as there are rendered triangles.
	int gDesiredTris = 10000;
	// Beginning frame varience (should be high, it will adjust automatically)
	private float gFrameVariance = 50;
	
	private float gClipAngle;
	private float[] gViewPosition={ 0.f, 5.f, 0.f };
	int gNumTrisRendered;
	
	private Landscape(){}
	
	/**
	 * 初始化所有地形块
	 * Initialize all patches
	 */
	public void init(int[] heightMap)
	{
		Patch patch;
		int x,y;
		// Store the Height Field array
		this.heightMap=heightMap;
		// Initialize all terrain patches
		for ( y=0; y < NUM_PATCHES_PER_SIDE; y++)
		{
			for ( x=0; x < NUM_PATCHES_PER_SIDE; x++ )
			{
				patch = patchs[y][x];
				patch.init( x*PATCH_SIZE, y*PATCH_SIZE, x*PATCH_SIZE, y*PATCH_SIZE,heightMap );
				patch.computeVariance();
			}
		}
		
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
	public static TriTreeNode allocateTri(){
		if(nextTriNode>=POOL_SIZE)
			return null;
		TriTreeNode node=pool[nextTriNode++];
		node.leftChild=node.rightChild=null;
		return node;
	}
	
	/**
	 * 在每帧开始时重新设置每个地形块，并在需要的时候重新计算变差值
	 * Reset all patches, recompute variance if needed
	 */
	public void reset()
	{
		float gFovX = 90.0f;
		float PI_DIV_180 = (float) (Math.PI / 180.0f);
		float FOV_DIV_2 = gFovX/2;
		
		int eyeX = (int)(gViewPosition[0] - PATCH_SIZE * Math.sin( gClipAngle * PI_DIV_180 ));
		int eyeY = (int)(gViewPosition[2] + PATCH_SIZE * Math.cos( gClipAngle * PI_DIV_180 ));

		int leftX  = (int)(eyeX + 100.0f * Math.sin( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));
		int leftY  = (int)(eyeY - 100.0f * Math.cos( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));

		int rightX = (int)(eyeX + 100.0f * Math.sin( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));
		int rightY = (int)(eyeY - 100.0f * Math.cos( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));
		
		int x,y;
		Patch patch;
		
		// Set the next free triangle pointer back to the beginning
		nextTriNode=0;
		
		// Reset rendered triangle count.
		gNumTrisRendered = 0;
		
		// Go through the patches performing resets, compute variances, and linking.
		for ( y=0; y < NUM_PATCHES_PER_SIDE; y++ )
			for ( x=0; x < NUM_PATCHES_PER_SIDE; x++)
			{
				patch = patchs[y][x];
				
				// Reset the patch
				patch.reset();
				patch.setVisibility( eyeX, eyeY, leftX, leftY, rightX, rightY );
				
				// Check to see if this patch has been deformed since last frame.
				// If so, recompute the varience tree for it.
				if (patch.isDirty())
					patch.computeVariance();

				if (patch.isVisible())
				{
					// Link all the patches together.
					if (x>0)
						patch.baseLeft.leftNeighbor=patchs[y][x-1].baseRight;
//						patch->GetBaseLeft()->LeftNeighbor = m_Patches[Y][X-1].GetBaseRight();
					else
						patch.baseLeft.leftNeighbor=null;// Link to bordering Landscape here..
//						patch->GetBaseLeft()->LeftNeighbor = NULL;		

					if (x<(NUM_PATCHES_PER_SIDE-1) )
						patch.baseRight.leftNeighbor=patchs[y][x+1].baseLeft;
//						patch->GetBaseRight()->LeftNeighbor = m_Patches[Y][X+1].GetBaseLeft();
					else
						patch.baseRight.leftNeighbor=null;// Link to bordering Landscape here..
//						patch->GetBaseRight()->LeftNeighbor = NULL;		

					if (y>0)
						patch.baseLeft.rightNeighbor=patchs[y-1][x].baseRight;
//						patch->GetBaseLeft()->RightNeighbor = m_Patches[Y-1][X].GetBaseRight();
					else
						patch.baseLeft.rightNeighbor=null;// Link to bordering Landscape here..
//						patch->GetBaseLeft()->RightNeighbor = NULL;		

					if (y<(NUM_PATCHES_PER_SIDE-1))
						patch.baseRight.rightNeighbor=patchs[y+1][x].baseLeft;
//						patch->GetBaseRight()->RightNeighbor = m_Patches[Y+1][X].GetBaseLeft();
					else
						patch.baseRight.rightNeighbor=null;// Link to bordering Landscape here..
//						patch->GetBaseRight()->RightNeighbor = NULL;	
				}
			}
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
	 * 分割地形块以生成近似网格
	 * Create an approximate mesh of the landscape.
	 */
	public void tessellate()
	{
		// Perform Tessellation
		Patch patch;
		for(int i=0;i<NUM_PATCHES_PER_SIDE;i++)
		{
			for(int j=0;j<NUM_PATCHES_PER_SIDE;j++)
			{
				patch=patchs[i][j];
				if (patch.isVisible())
					patch.tesslate();
			}
		}
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
	 * 渲染每个地形块，并调整gFrameVariance的值
	 * Render each patch of the landscape & adjust the frame variance.
	 */
	public void render()
	{
		Patch patch;
		for(int i=0;i<NUM_PATCHES_PER_SIDE;i++)
		{
			for(int j=0;j<NUM_PATCHES_PER_SIDE;j++)
			{
				patch=patchs[i][j];
				if (patch.isVisible())
					patch.render();
			}
		}
		//TODO 不明白
		// Check to see if we got close to the desired number of triangles.
		// Adjust the frame variance to a better value.
		if(nextTriNode!=gDesiredTris)
			gFrameVariance+=(nextTriNode-gDesiredTris)/gDesiredTris;
		// Bounds checking.
		if(gFrameVariance<0)
			gFrameVariance=0;
			
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
