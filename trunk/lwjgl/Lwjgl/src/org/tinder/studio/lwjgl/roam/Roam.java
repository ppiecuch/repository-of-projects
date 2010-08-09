package org.tinder.studio.lwjgl.roam;


/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-23 ����05:47:56
 * ԭ��
 * ע�⣺
 * 1����ͼ��С������2^n+1
 */
public class Roam {
	
	private short[][] heightWeights;
	private float[] scales;	//�Ŵ�ϵ��     scale>0
	private float delicate;	//ϸ��ϵ��
	
	public Roam(short[][] heightWeights,float[] scales,float delicate)
	{
		this.heightWeights=heightWeights;
		this.scales=scales;
		this.delicate=delicate;
	}
	
	public void init(){
		
	}
	
	public void drawFrame(){
		
	}
	
	public TriTreeNode allocate(){
		return null;
	}
	
	/**
	 * 
	 * ��ʯ�������Խǵĵ��������ι��ɣ�ÿ���������γ�һ�������Ķ�Ԫ������
	 *
	 */
	class Diamond{
		public TriTreeNode baseLeft;// ���Ԫ������
		public TriTreeNode baseRight;// �Ҷ�Ԫ������
		public int x,y;
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
	}

}
