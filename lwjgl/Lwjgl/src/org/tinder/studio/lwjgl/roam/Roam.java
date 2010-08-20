package org.tinder.studio.lwjgl.roam;

import org.lwjgl.opengl.GL11;
import org.tinder.studio.lwjgl.util.Util;


/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-23 下午05:47:56
 * 【原理】
 * ROAM(Real-Time Optionally Adapting Meshes)方法，即实时优化自适应地形网格，其基本思想是通过将网格分割为多个等腰直角三角形进行细分，
 * 从而提高细节等级(Mark Duchaineau等，1997)。在传统的ROAM算法中，当DEM数据量比较小时，采用三角形二叉树构建LOD模型的效率非常高。
 * 但当DEM数据量比较大时，三角形二叉树就会变得庞大复杂，难以处理规模较大的地形。
 * 
 * 分块思想
 * 采用地形分块的办法，可以有效地处理三角形二叉树过于庞大的问题，
 * 对规模较大的地形进行分块处理以后，每一个子块分别建立对应的二叉树三角形，
 * 这样整个地形的高深度二叉树就会分解成多个低深度的二叉树，在低深度的二叉树上建立LOD模型。
 * 在三维渲染时，首先根据视点判断各个子块是否需要更加细致的表现，如果需要，就建立三角形二叉树；
 * 否则就只是显示该块，这样就只需建立少数低深度二叉树，而不必对整个地形建立一个高深度二叉树，减少了存储空间，降低了复杂度。
 * 
 * 基于分块的ROAM算法的步骤可以分为建立分块结构、块分割、消除裂缝三个部分。
 * 
 * 注意：
 * 1、地图大小必须是2^n+1
 */
public class Roam {
	private static final int POOL_SIZE=25000;
	private short[][] heightMap;
	private float[] scales;	//放大系数     scale>0
	private float delicate;	//细致系数
	private int mapSize=65;//地图大小
	private int patchNumPerSide=16;//每边块数
	private int varianceLimit=50;//变差值界限
	private int patchSize=mapSize/patchNumPerSide;//块大小
	private Diamond[][] patchs;
	private float[] viewPosition={0,0,0};
	private TriTreeNode[] pool;
	public int nextTriNode=0;
	private static float gFovX = 90.0f;
	private float gClipAngle=120;
	private int gNumTrisRendered;
	// Desired number of Binary Triangle tessellations per frame.
	// This is not the desired number of triangles rendered!
	// There are usually twice as many Binary Triangle structures as there are rendered triangles.
	private int gDesiredTris = 10000;
	// Beginning frame varience (should be high, it will adjust automatically)
	private float gFrameVariance = 50;
	
	private static final float PI_DIV_180 = (float) (Math.PI / 180.0f);
	private static final float FOV_DIV_2 = gFovX/2;
	
	public Roam(short[][] heightMap,float[] scales,float delicate)
	{
		this.heightMap=heightMap;
		this.scales=scales;
		this.delicate=delicate;
	}
	
	/**
	 * 初始化所有地形块
	 */
	public void init(){
		pool=new TriTreeNode[POOL_SIZE];
		for(int i=0;i<POOL_SIZE;i++)
		{
			pool[i]=new TriTreeNode();
		}
		patchs=new Diamond[patchNumPerSide][];
		viewPosition=new float[3];
		for(int i=0;i<patchNumPerSide;i++)
		{
			patchs[i]=new Diamond[patchNumPerSide];
			for(int j=0;j<patchNumPerSide;j++)
			{
				patchs[i][j]=new Diamond();
				patchs[i][j].init(j*patchSize, i*patchSize);
				patchs[i][j].computeVariance();
			}
		}
	}
	
	/**
	 *  在每帧开始时重新设置每个地形块，并在需要的时候重新计算变差值
	 */
	public void reset(){
		//根据摄相机定义一个三角形，以patchSize为单位；如果patch在视截体中则此patch可视
		float FOV_DIV_2 = gFovX/2;
		int eyeX = (int)(viewPosition[0] - patchSize * Math.sin( gClipAngle * PI_DIV_180 ));
		int eyeY = (int)(viewPosition[2] + patchSize * Math.cos( gClipAngle * PI_DIV_180 ));

		int leftX  = (int)(eyeX + 100.0f * Math.sin( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));
		int leftY  = (int)(eyeY - 100.0f * Math.cos( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));

		int rightX = (int)(eyeX + 100.0f * Math.sin( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));
		int rightY = (int)(eyeY - 100.0f * Math.cos( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));
		
		nextTriNode=0;
		gNumTrisRendered=0;
		Diamond patch;
		for(int y=0;y<patchNumPerSide;y++)
		{
			for(int x=0;x<patchNumPerSide;x++)
			{
				patch=patchs[y][x];
				patch.reset();
				patch.setVisibility(eyeX, eyeY, leftX, leftY, rightX, rightY);
				
				//TODO 不明白
				//检测patch是否在上一帧中变形过，如果是重新切割
				if(patch.isDirty())
					patch.computeVariance();
				if(patch.isVisible())
				{
					//将左右上下的patch进行互联
					if(x>0)
						patch.baseLeft.leftNeighbor=patchs[y][x-1].baseRight;
					else
						patch.baseLeft.leftNeighbor=null;
					if(x<patchNumPerSide-1)
						patch.baseRight.leftNeighbor=patchs[y][x+1].baseLeft;
					else
						patch.baseRight.leftNeighbor=null;
					
					if(y>0)
						patch.baseLeft.rightNeighbor=patchs[y-1][x].baseRight;
					else
						patch.baseLeft.rightNeighbor=null;
					if(y<patchNumPerSide-1)
						patch.baseRight.rightNeighbor=patchs[y+1][x].baseLeft;
					else
						patch.baseRight.rightNeighbor=null;
				}
			}
		}
	}
	
	/**
	 * 画出视点跟视角
	 */
	public void drawFrustum(){
		
		GL11.glDisable(GL11.GL_LIGHTING);
		GL11.glDisable(GL11.GL_TEXTURE_2D);
		GL11.glDisable(GL11.GL_TEXTURE_GEN_S);
		GL11.glDisable(GL11.GL_TEXTURE_GEN_T);
		GL11.glBegin(GL11.GL_LINES);

		// Draw the View Vector starting at the eye (red)
		GL11.glColor3f(1, 0, 0);
		GL11.glVertex3f(viewPosition[0],viewPosition[1],viewPosition[2]);
		GL11.glVertex3f((float)(viewPosition[0]+50.0f* Math.sin( gClipAngle*Math.PI/180.0f)),viewPosition[1],(float)(viewPosition[2]-50.0f*Math.cos(gClipAngle*Math.PI/ 180.0f)));
		// Draw the view frustum (blue)
		GL11.glColor3f(0, 0, 1);
		GL11.glVertex3f(viewPosition[0],viewPosition[1],viewPosition[2] );
		GL11.glVertex3f((float)(viewPosition[0] + 1000.0f * Math.sin((gClipAngle-45.0f)*Math.PI/180.0f)),viewPosition[1],(float)(viewPosition[2]-1000.0f*Math.cos((gClipAngle-45.0f)*Math.PI/180.0f)));
		GL11.glVertex3f(viewPosition[0],viewPosition[1],viewPosition[2]);
		GL11.glVertex3f((float)(viewPosition[0]+1000.0f*Math.sin((gClipAngle+45.0f)*Math.PI/180.0f)),viewPosition[1],(float)(viewPosition[2]-1000.0f*Math.cos((gClipAngle+45.0f)*Math.PI/180.0f)));
		// Draw the clipping planes behind the eye (yellow)
		int ptEyeX = (int)(viewPosition[0] - patchSize * Math.sin( gClipAngle * PI_DIV_180 ));
		int ptEyeY = (int)(viewPosition[2] + patchSize * Math.cos( gClipAngle * PI_DIV_180 ));

		int ptLeftX = (int)(ptEyeX + 100.0f * Math.sin( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));
		int ptLeftY = (int)(ptEyeY - 100.0f * Math.cos( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));

		int ptRightX = (int)(ptEyeX + 100.0f * Math.sin( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));
		int ptRightY = (int)(ptEyeY - 100.0f * Math.cos( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));

		GL11.glColor3f(1, 1, 0);
		GL11.glVertex3f((float)ptEyeX,viewPosition[1],(float)ptEyeY);
		GL11.glVertex3f((float)ptLeftX,viewPosition[1],(float)ptLeftY);
		GL11.glVertex3f((float)ptEyeX,viewPosition[1],(float)ptEyeY);
		GL11.glVertex3f((float)ptRightX,viewPosition[1],(float)ptRightY);
		GL11.glEnd();
		GL11.glLineWidth(1.f);
		GL11.glColor3f(1, 1, 1);
		
	}
	
	private TriTreeNode allocate(){
		if(nextTriNode>=POOL_SIZE)
			return null;
		TriTreeNode node=pool[nextTriNode++];
		node.leftChild=node.rightChild=null;
		return node;
	}
	
	/**
	 * 分割地形块以生成近似网格
	 * Create an approximate mesh of the landscape.
	 */
	public void tessellate()
	{
		Diamond patch;
		for(int i=0;i<patchNumPerSide;i++)
		{
			for(int j=0;j<patchNumPerSide;j++)
			{
				patch=patchs[i][j];
				if (patch.isVisible())
					patch.tessellate();
			}
		}
	}
	
	public void render(){
		Diamond patch = patchs[0][0];
//		glScalef( 1.0f, MULT_SCALE, 1.0f );
		for(int y=0;y<patchNumPerSide;y++)
		{
			for(int x=0;x<patchNumPerSide;x++)
			{
				patch=patchs[y][x];
				if(patch.isVisible())
					patch.render();
			}
		}
		//TODO  不明
		if(nextTriNode!=gDesiredTris)
			gFrameVariance += ((float)nextTriNode - (float)gDesiredTris) / (float)gDesiredTris;
		// Bounds checking.
		if ( gFrameVariance < 0 )
			gFrameVariance = 0;
	}
	
	/**
	 * 
	 * 钻石（块）由两个对角的等腰三角形构成，每个三角形形成一个独立的二元三角树，
	 * 每个钻石由两个二元三角树组成，当需要更高的细节时，再进一步划分。
	 *
	 */
	private class Diamond{
		private static final int VARIANCE_DEPTH=9;//变差树的深度，必须是近似 SQRT(PATCH_SIZE) + 1的值
		private TriTreeNode baseLeft;// 左二元三角树
		private TriTreeNode baseRight;// 右二元三角树
		private int[] varianceLeft=new int[1<<(VARIANCE_DEPTH)];// 左变差树
		private int[] varianceRight=new int[1<<(VARIANCE_DEPTH)];// 右变差树
		private int[] currentVariance;// 临时索引，用于tessellate和computeVariance方法
		private int x,y;
		private boolean visible=false;
		private boolean dirty=false;
//		public byte height;
		
		public Diamond() {
			this.baseLeft=new TriTreeNode();
			this.baseRight=new TriTreeNode();
		}
		
		public void init(int x,int y){
			//清空记录
			baseLeft.rightNeighbor=null;
			baseLeft.leftNeighbor=null;
			baseLeft.leftChild=null;
			baseLeft.rightChild=null;
			baseRight.rightNeighbor=null;
			baseRight.leftNeighbor=null;
			baseRight.leftChild=null;
			baseRight.rightChild=null;
			//关联两个三角形
			baseLeft.baseNeighbor=baseRight;
			baseRight.baseNeighbor=baseLeft;
			
			this.x=x;
			this.y=y;
			dirty=true;
		}
		
		public void reset(){
			visible=false;
			
			baseLeft.leftChild=null;
			baseLeft.rightChild=null;
			baseRight.leftChild=null;
			baseRight.rightChild=null;
			
			baseLeft.baseNeighbor=baseRight;
			baseRight.baseNeighbor=baseLeft;
			
			baseLeft.rightNeighbor=null;
			baseLeft.leftNeighbor=null;
			baseRight.rightNeighbor=null;
			baseRight.leftNeighbor=null;
		}
		
		/**
		 * 在实际中，并不需要分割所有的三角形，也不可能无限分割，因此就需要对要分割的三角形进行选择，
		 * 也就是说需要知道一个结点何时应该继续进行分割。这与三角形与视点的距离、地形本身的粗糙程度有关，
		 * 简言之，对远处的地形和平坦的地形不需要高细节的渲染，对近处的地形和复杂的地形需要更加清晰的细节才能看清物体。
		 * 对于分割选择，首要的是得到三角形与视点的距离和地形粗糙程度。
		 * 在实际运算中，计算视点到三角形的距离并不复杂，但是对于大规模地形而言，无疑会加重CPU负担，
		 * 所以，采用直角三角形的斜边中点的位置和视点的距离近似视点与三角形的距离，这种方法虽然存在微小误差，
		 * 但在整体上不会影响地形的LOD显示，有效降低了计算复杂度，提高了渲染帧速度。 
		 * 规定每个三角形的粗糙度是斜边中点的实际高度与两个定点连线的中点高度差值，把该值记为T。
		 * 每个节点的粗糙度，就是它所有子结点中最大的T值。
		 * 
		 * 调用recursComputeVariance方法完成各个节点的变差计算
		 */
		public void computeVariance(){
			//分别对两个二元三角树进行递归运算
			currentVariance=varianceLeft;
			recursComputeVariance(0,patchSize,heightMap[y+patchSize][x],patchSize,0,heightMap[y][x+patchSize],0,0,heightMap[y][x],1);
			currentVariance=varianceRight;
			recursComputeVariance(patchSize,0,heightMap[y][x+patchSize],0,patchSize,heightMap[y+patchSize][x],patchSize,patchSize,heightMap[y+patchSize][x+patchSize],1);
			dirty=false;
		}
		
		/**
		 * 计算整棵二元三角树所有节点的变差
		 * @param leftX
		 * @param leftY
		 * @param leftZ
		 * @param rightX
		 * @param rightY
		 * @param rightZ
		 * @param apexX
		 * @param apexY
		 * @param apexZ
		 * @param index
		 * @return
		 */
		private int recursComputeVariance(int leftX,int leftY,int leftZ,int rightX,int rightY,int rightZ,int apexX,int apexY,int apexZ,int index){
			//计算斜边中点坐标
			int centerX = (leftX + rightX) >>1;
			int centerY = (leftY + rightY) >>1;
			int myVariance;
			
			// 获取实际高度值
			int centerZ=heightMap[centerY][centerX];
			
			//计算变差值
			myVariance = Math.abs((int)centerZ - (((int)leftZ + (int)rightZ)>>1));
			
			//基于效率的考虑，我们只以8*8大小的块为单位进行计算
			if ((Math.abs(leftX - rightX)>=8)||(Math.abs(leftY - rightY)>=8))
			{
				// 将自身的变差值与子孙的变差值进行比较，取其最大者
				myVariance = Math.max( myVariance, recursComputeVariance( apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    index<<1 ) );
				myVariance = Math.max( myVariance, recursComputeVariance( rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(index<<1)) );
			}
			
			//保存结果，注意变差值不为0
			if (index < currentVariance.length)
				currentVariance[index] = 1 + myVariance;
			return myVariance;
		}
		
		/**
		 * 细分网格
		 * 将每一个网格分割成两个三角小片，每个小片都是一个单独的等腰直角三角形，
		 * 从每个等腰直角三角形的顶点到对面斜边的中点分割三角形为两个新的等腰直角三角形，如此递归分割，直到达到希望的细节等级。
		 * 分割以后，ROAM方法采用二叉树的数据结构来存储分割的三角形
		 */
		public void tessellate(){
			// 分割两个二元三角树
			currentVariance=varianceLeft;
			recursTessellate(baseLeft,x,y+patchSize,x+patchSize,y,x,y,1);
			currentVariance=varianceRight;
			recursTessellate(baseRight,x+patchSize,y,x,y+patchSize,x+patchSize,y+patchSize,1);
		}
		
		/**
		 * 分割一个小块，直到满足一定细节层次
		 */
		private void recursTessellate(TriTreeNode tri,int leftX,int leftY,int rightX,int rightY,int apexX,int apexY,int index)
		{
			float triVariance=0;
			//计算斜边中点
			int centerX = (leftX + rightX)>>1;
			int centerY = (leftY + rightY)>>1;
			
			if (index<(1<<VARIANCE_DEPTH))
			{
				// Extremely slow distance metric (sqrt is used).
				// Replace this with a faster one!
				float distance = (float) (1.0f + Math.sqrt(Util.square((float)centerX - viewPosition[0])+Util.square((float)centerY - viewPosition[2]) ));
				// Egads!  A division too?  What's this world coming to!
				// This should also be replaced with a faster operation.
				triVariance = ((float)currentVariance[index]*mapSize*2)/distance;	// Take both distance and variance into consideration
//				System.out.println("distance:"+distance+",triVariance:"+triVariance);
			}
			
			//如果索引溢出，表示之前已经分割过此节点（不明白），因此继续分割操作，又或者变差超限，也要进行分割操作
			if((index>=(1<<VARIANCE_DEPTH))||(triVariance > varianceLimit))
			{
				split(tri);
//				System.out.println("tri:"+tri);
				//如果儿子不空，则继续分割儿子
				if(tri.leftChild!=null&&((Math.abs(leftX - rightX)>=3)||(Math.abs(leftY-rightY)>= 3)))
				{
					recursTessellate(tri.leftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    index<<1);
					recursTessellate(tri.rightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(index<<1));
				}
			}
		}
		
		/**
		 * 对当前节点进行递归分解
		 * 当分割一个节点时存在三种可能：
		 * 1、节点是钻石的一部分---分割它和它的下邻节点。
		 * 2、节点是网格的边---只分割这个节点。
		 * 3、节点不是钻石的一部分---强制分割下邻节点。
		 * @param node
		 */
		public void split(TriTreeNode node){
			//如果已经切割，无需再处理
			if(node.leftChild!=null)
				return;
			//如果节点不是钻石的一部分---强制分割下邻节点
			if(node.baseNeighbor!=null&&node.baseNeighbor.baseNeighbor!=node)
				split(node.baseNeighbor);
			//分配
			node.leftChild=allocate();
			node.rightChild=allocate();
			//如果分配库存已经用空
			if(node.leftChild==null)
				return;
			//关联节点
			node.leftChild.baseNeighbor=node.leftNeighbor;
			node.leftChild.leftNeighbor=node.rightChild;
			node.rightChild.baseNeighbor=node.rightNeighbor;
			node.rightChild.rightNeighbor=node.leftChild;
			if(node.leftNeighbor!=null)
			{
				if(node.leftNeighbor.baseNeighbor==node)
					node.leftNeighbor.baseNeighbor=node.leftChild;
				else if(node.leftNeighbor.leftNeighbor==node)
					node.leftNeighbor.leftNeighbor=node.leftChild;
				else if(node.leftNeighbor.rightNeighbor==node)
					node.leftNeighbor.rightNeighbor=node.leftChild;
				else
					;// Illegal Left Neighbor!
			}
			if(node.rightNeighbor!=null)
			{
				if(node.rightNeighbor.baseNeighbor==node)
					node.rightNeighbor.baseNeighbor=node.rightChild;
				else if(node.rightNeighbor.rightNeighbor==node)
					node.rightNeighbor.rightNeighbor=node.rightChild;
				else if(node.rightNeighbor.leftNeighbor==node)
					node.rightNeighbor.leftNeighbor=node.rightChild;
				else
					;// Illegal Right Neighbor!
			}
			//如果节点是钻石的一部分
			if(node.baseNeighbor!=null)
			{
				//如果已经切割，关联节点
				if(node.baseNeighbor.leftChild!=null)
				{
					node.baseNeighbor.leftChild.rightNeighbor=node.rightChild;
					node.baseNeighbor.rightChild.leftNeighbor=node.leftChild;
					node.leftChild.rightNeighbor=node.baseNeighbor.rightChild;
					node.rightChild.leftNeighbor=node.baseNeighbor.leftChild;
				}
				//否则下邻节点。
				else
					split(node.baseNeighbor);
			}
			else
			{
				//是一个边缘节点，清空
				node.leftChild.rightNeighbor=null;
				node.rightChild.leftNeighbor=null;
			}
//			System.out.println("split "+node);
		}
		
		/**
		 * 根据嵌套包围盒算法来判断地形的可见性，从而实现裁剪
		 * @param eyeX
		 * @param eyeY
		 * @param leftX
		 * @param leftY
		 * @param rightX
		 * @param rightY
		 */
		public void setVisibility(int eyeX,int eyeY,int leftX,int leftY,int rightX,int rightY )
		{
			int patchCenterX=x+patchSize/2;
			int patchCenterY=y+patchSize/2;
			
			// Set visibility flag (orientation of both triangles must be counter clockwise)
			visible=Util.orientation(eyeX,eyeY,rightX,rightY,patchCenterX, patchCenterY)<0&&Util.orientation(leftX,leftY,eyeX,eyeY,patchCenterX,patchCenterY )<0;
			
//			System.out.println("visible:"+visible);
		}

		public boolean isVisible() {
			return visible;
		}

		public boolean isDirty() {
			return dirty;
		}
		
		private void recursRender(TriTreeNode tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY ){
//			System.out.println("recursRender");
			if(tri.leftChild!=null)
			{
				int centerX = (leftX + rightX)>>1;
				int centerY = (leftY + rightY)>>1;
				recursRender( tri.leftChild,  apexX,   apexY, leftX, leftY, centerX, centerY );
				recursRender( tri.rightChild, rightX, rightY, apexX, apexY, centerX, centerY );
			}
			else
			{
				gNumTrisRendered++;
				float leftZ=heightMap[leftY][leftX];
				float rightZ=heightMap[rightY][rightX];
				float apexZ=heightMap[apexY][apexX];
				
				float fColor = (60.0f + leftZ) / 256.0f;
				if ( fColor > 1.0f )  fColor = 1.0f;
				GL11.glColor3f( fColor, fColor, fColor );
				GL11.glVertex3f(leftX,leftZ,leftY );
//				System.out.println(leftX+","+leftZ+","+leftY);
				
				fColor = (60.0f + rightZ) / 256.0f;
				if ( fColor > 1.0f )  fColor = 1.0f;
				GL11.glColor3f( fColor, fColor, fColor );
				GL11.glVertex3f(rightX,rightZ,rightY );
				
				fColor = (60.0f + apexZ) / 256.0f;
				if ( fColor > 1.0f )  fColor = 1.0f;
				GL11.glColor3f( fColor, fColor, fColor );
				GL11.glVertex3f(apexX,apexZ,apexY );
			}
		}
		
		private void render(){
			GL11.glPushMatrix();
			GL11.glTranslatef( x, 0, y );
			GL11.glBegin(GL11.GL_LINE_LOOP);
			recursRender(baseLeft,0,patchSize,patchSize,0,0,0);
			recursRender(baseRight,patchSize,0,0,patchSize,patchSize,patchSize);
			GL11.glEnd();
			GL11.glPopMatrix();
		}
		
	}

	public void setgClipAngle(float gClipAngle) {
		this.gClipAngle = gClipAngle;
	}
	
	

}
