package org.tinder.studio.lwjgl.roam;

import org.lwjgl.opengl.GL11;
import org.tinder.studio.lwjgl.util.Util;


/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-23 ����05:47:56
 * ��ԭ��
 * ROAM(Real-Time Optionally Adapting Meshes)��������ʵʱ�Ż�����Ӧ�������������˼����ͨ��������ָ�Ϊ�������ֱ�������ν���ϸ�֣�
 * �Ӷ����ϸ�ڵȼ�(Mark Duchaineau�ȣ�1997)���ڴ�ͳ��ROAM�㷨�У���DEM�������Ƚ�Сʱ�����������ζ���������LODģ�͵�Ч�ʷǳ��ߡ�
 * ����DEM�������Ƚϴ�ʱ�������ζ������ͻ����Ӵ��ӣ����Դ����ģ�ϴ�ĵ��Ρ�
 * 
 * �ֿ�˼��
 * ���õ��ηֿ�İ취��������Ч�ش��������ζ����������Ӵ�����⣬
 * �Թ�ģ�ϴ�ĵ��ν��зֿ鴦���Ժ�ÿһ���ӿ�ֱ�����Ӧ�Ķ����������Σ�
 * �����������εĸ���ȶ������ͻ�ֽ�ɶ������ȵĶ��������ڵ���ȵĶ������Ͻ���LODģ�͡�
 * ����ά��Ⱦʱ�����ȸ����ӵ��жϸ����ӿ��Ƿ���Ҫ����ϸ�µı��֣������Ҫ���ͽ��������ζ�������
 * �����ֻ����ʾ�ÿ飬������ֻ�轨����������ȶ������������ض��������ν���һ������ȶ������������˴洢�ռ䣬�����˸��Ӷȡ�
 * 
 * ���ڷֿ��ROAM�㷨�Ĳ�����Է�Ϊ�����ֿ�ṹ����ָ�����ѷ��������֡�
 * 
 * ע�⣺
 * 1����ͼ��С������2^n+1
 */
public class Roam {
	private static final int POOL_SIZE=25000;
	private short[][] heightMap;
	private float[] scales;	//�Ŵ�ϵ��     scale>0
	private float delicate;	//ϸ��ϵ��
	private int mapSize=65;//��ͼ��С
	private int patchNumPerSide=16;//ÿ�߿���
	private int varianceLimit=50;//���ֵ����
	private int patchSize=mapSize/patchNumPerSide;//���С
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
	 * ��ʼ�����е��ο�
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
	 *  ��ÿ֡��ʼʱ��������ÿ�����ο飬������Ҫ��ʱ�����¼�����ֵ
	 */
	public void reset(){
		//�������������һ�������Σ���patchSizeΪ��λ�����patch���ӽ��������patch����
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
				
				//TODO ������
				//���patch�Ƿ�����һ֡�б��ι�������������и�
				if(patch.isDirty())
					patch.computeVariance();
				if(patch.isVisible())
				{
					//���������µ�patch���л���
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
	 * �����ӵ���ӽ�
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
	 * �ָ���ο������ɽ�������
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
		//TODO  ����
		if(nextTriNode!=gDesiredTris)
			gFrameVariance += ((float)nextTriNode - (float)gDesiredTris) / (float)gDesiredTris;
		// Bounds checking.
		if ( gFrameVariance < 0 )
			gFrameVariance = 0;
	}
	
	/**
	 * 
	 * ��ʯ���飩�������Խǵĵ��������ι��ɣ�ÿ���������γ�һ�������Ķ�Ԫ��������
	 * ÿ����ʯ��������Ԫ��������ɣ�����Ҫ���ߵ�ϸ��ʱ���ٽ�һ�����֡�
	 *
	 */
	private class Diamond{
		private static final int VARIANCE_DEPTH=9;//���������ȣ������ǽ��� SQRT(PATCH_SIZE) + 1��ֵ
		private TriTreeNode baseLeft;// ���Ԫ������
		private TriTreeNode baseRight;// �Ҷ�Ԫ������
		private int[] varianceLeft=new int[1<<(VARIANCE_DEPTH)];// ������
		private int[] varianceRight=new int[1<<(VARIANCE_DEPTH)];// �ұ����
		private int[] currentVariance;// ��ʱ����������tessellate��computeVariance����
		private int x,y;
		private boolean visible=false;
		private boolean dirty=false;
//		public byte height;
		
		public Diamond() {
			this.baseLeft=new TriTreeNode();
			this.baseRight=new TriTreeNode();
		}
		
		public void init(int x,int y){
			//��ռ�¼
			baseLeft.rightNeighbor=null;
			baseLeft.leftNeighbor=null;
			baseLeft.leftChild=null;
			baseLeft.rightChild=null;
			baseRight.rightNeighbor=null;
			baseRight.leftNeighbor=null;
			baseRight.leftChild=null;
			baseRight.rightChild=null;
			//��������������
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
		 * ��ʵ���У�������Ҫ�ָ����е������Σ�Ҳ���������޷ָ��˾���Ҫ��Ҫ�ָ�������ν���ѡ��
		 * Ҳ����˵��Ҫ֪��һ������ʱӦ�ü������зָ�������������ӵ�ľ��롢���α���Ĵֲڳ̶��йأ�
		 * ����֮����Զ���ĵ��κ�ƽ̹�ĵ��β���Ҫ��ϸ�ڵ���Ⱦ���Խ����ĵ��κ͸��ӵĵ�����Ҫ����������ϸ�ڲ��ܿ������塣
		 * ���ڷָ�ѡ����Ҫ���ǵõ����������ӵ�ľ���͵��δֲڳ̶ȡ�
		 * ��ʵ�������У������ӵ㵽�����εľ��벢�����ӣ����Ƕ��ڴ��ģ���ζ��ԣ����ɻ����CPU������
		 * ���ԣ�����ֱ�������ε�б���е��λ�ú��ӵ�ľ�������ӵ��������εľ��룬���ַ�����Ȼ����΢С��
		 * ���������ϲ���Ӱ����ε�LOD��ʾ����Ч�����˼��㸴�Ӷȣ��������Ⱦ֡�ٶȡ� 
		 * �涨ÿ�������εĴֲڶ���б���е��ʵ�ʸ߶��������������ߵ��е�߶Ȳ�ֵ���Ѹ�ֵ��ΪT��
		 * ÿ���ڵ�Ĵֲڶȣ������������ӽ��������Tֵ��
		 * 
		 * ����recursComputeVariance������ɸ����ڵ�ı�����
		 */
		public void computeVariance(){
			//�ֱ��������Ԫ���������еݹ�����
			currentVariance=varianceLeft;
			recursComputeVariance(0,patchSize,heightMap[y+patchSize][x],patchSize,0,heightMap[y][x+patchSize],0,0,heightMap[y][x],1);
			currentVariance=varianceRight;
			recursComputeVariance(patchSize,0,heightMap[y][x+patchSize],0,patchSize,heightMap[y+patchSize][x],patchSize,patchSize,heightMap[y+patchSize][x+patchSize],1);
			dirty=false;
		}
		
		/**
		 * �������ö�Ԫ���������нڵ�ı��
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
			//����б���е�����
			int centerX = (leftX + rightX) >>1;
			int centerY = (leftY + rightY) >>1;
			int myVariance;
			
			// ��ȡʵ�ʸ߶�ֵ
			int centerZ=heightMap[centerY][centerX];
			
			//������ֵ
			myVariance = Math.abs((int)centerZ - (((int)leftZ + (int)rightZ)>>1));
			
			//����Ч�ʵĿ��ǣ�����ֻ��8*8��С�Ŀ�Ϊ��λ���м���
			if ((Math.abs(leftX - rightX)>=8)||(Math.abs(leftY - rightY)>=8))
			{
				// ������ı��ֵ������ı��ֵ���бȽϣ�ȡ�������
				myVariance = Math.max( myVariance, recursComputeVariance( apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    index<<1 ) );
				myVariance = Math.max( myVariance, recursComputeVariance( rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(index<<1)) );
			}
			
			//��������ע����ֵ��Ϊ0
			if (index < currentVariance.length)
				currentVariance[index] = 1 + myVariance;
			return myVariance;
		}
		
		/**
		 * ϸ������
		 * ��ÿһ������ָ����������СƬ��ÿ��СƬ����һ�������ĵ���ֱ�������Σ�
		 * ��ÿ������ֱ�������εĶ��㵽����б�ߵ��е�ָ�������Ϊ�����µĵ���ֱ�������Σ���˵ݹ�ָֱ���ﵽϣ����ϸ�ڵȼ���
		 * �ָ��Ժ�ROAM�������ö����������ݽṹ���洢�ָ��������
		 */
		public void tessellate(){
			// �ָ�������Ԫ������
			currentVariance=varianceLeft;
			recursTessellate(baseLeft,x,y+patchSize,x+patchSize,y,x,y,1);
			currentVariance=varianceRight;
			recursTessellate(baseRight,x+patchSize,y,x,y+patchSize,x+patchSize,y+patchSize,1);
		}
		
		/**
		 * �ָ�һ��С�飬ֱ������һ��ϸ�ڲ��
		 */
		private void recursTessellate(TriTreeNode tri,int leftX,int leftY,int rightX,int rightY,int apexX,int apexY,int index)
		{
			float triVariance=0;
			//����б���е�
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
			
			//��������������ʾ֮ǰ�Ѿ��ָ���˽ڵ㣨�����ף�����˼����ָ�������ֻ��߱��ޣ�ҲҪ���зָ����
			if((index>=(1<<VARIANCE_DEPTH))||(triVariance > varianceLimit))
			{
				split(tri);
//				System.out.println("tri:"+tri);
				//������Ӳ��գ�������ָ����
				if(tri.leftChild!=null&&((Math.abs(leftX - rightX)>=3)||(Math.abs(leftY-rightY)>= 3)))
				{
					recursTessellate(tri.leftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    index<<1);
					recursTessellate(tri.rightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(index<<1));
				}
			}
		}
		
		/**
		 * �Ե�ǰ�ڵ���еݹ�ֽ�
		 * ���ָ�һ���ڵ�ʱ�������ֿ��ܣ�
		 * 1���ڵ�����ʯ��һ����---�ָ������������ڽڵ㡣
		 * 2���ڵ�������ı�---ֻ�ָ�����ڵ㡣
		 * 3���ڵ㲻����ʯ��һ����---ǿ�Ʒָ����ڽڵ㡣
		 * @param node
		 */
		public void split(TriTreeNode node){
			//����Ѿ��и�����ٴ���
			if(node.leftChild!=null)
				return;
			//����ڵ㲻����ʯ��һ����---ǿ�Ʒָ����ڽڵ�
			if(node.baseNeighbor!=null&&node.baseNeighbor.baseNeighbor!=node)
				split(node.baseNeighbor);
			//����
			node.leftChild=allocate();
			node.rightChild=allocate();
			//����������Ѿ��ÿ�
			if(node.leftChild==null)
				return;
			//�����ڵ�
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
			//����ڵ�����ʯ��һ����
			if(node.baseNeighbor!=null)
			{
				//����Ѿ��и�����ڵ�
				if(node.baseNeighbor.leftChild!=null)
				{
					node.baseNeighbor.leftChild.rightNeighbor=node.rightChild;
					node.baseNeighbor.rightChild.leftNeighbor=node.leftChild;
					node.leftChild.rightNeighbor=node.baseNeighbor.rightChild;
					node.rightChild.leftNeighbor=node.baseNeighbor.leftChild;
				}
				//�������ڽڵ㡣
				else
					split(node.baseNeighbor);
			}
			else
			{
				//��һ����Ե�ڵ㣬���
				node.leftChild.rightNeighbor=null;
				node.rightChild.leftNeighbor=null;
			}
//			System.out.println("split "+node);
		}
		
		/**
		 * ����Ƕ�װ�Χ���㷨���жϵ��εĿɼ��ԣ��Ӷ�ʵ�ֲü�
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
