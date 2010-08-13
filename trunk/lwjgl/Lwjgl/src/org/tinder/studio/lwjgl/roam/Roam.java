package org.tinder.studio.lwjgl.roam;

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
	private int mapSize=1024;//��ͼ��С
	private int patchNumPerSide=16;//ÿ�߿���
	private int varianceLimit=50;//���ֵ����
	private int patchSize=mapSize/patchNumPerSide;//���С
	private Diamond[][] patchs;
	private float[] viewPosition;
	private TriTreeNode[] pool;
	public int nextTriNode=0;
	private float gFovX = 90.0f;
	private float gClipAngle;
	
	private static final float PI_DIV_180 = (float) (Math.PI / 180.0f);
	
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
	
	public void drawFrame(){
		
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
			}
			
			//��������������ʾ֮ǰ�Ѿ��ָ���˽ڵ㣨�����ף�����˼����ָ�������ֻ��߱��ޣ�ҲҪ���зָ����
			if((index>=(1<<VARIANCE_DEPTH))||(triVariance > varianceLimit))
			{
				split(tri);
				//������Ӳ��գ�������ָ����
				if(tri.leftChild!=null&&((Math.abs(leftX - rightX)>=3)||(Math.abs(leftY-rightY)>= 3)))
				{
					recursTessellate( tri.leftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    index<<1);
					recursTessellate( tri.rightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(index<<1));
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
		}

		public boolean isVisible() {
			return visible;
		}

		public boolean isDirty() {
			return dirty;
		}
		
	}

}
