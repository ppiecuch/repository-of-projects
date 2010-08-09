package org.tinder.studio.lwjgl.roam;


/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-23 下午05:47:56
 * 原理：
 * 注意：
 * 1、地图大小必须是2^n+1
 */
public class Roam {
	
	private short[][] heightWeights;
	private float[] scales;	//放大系数     scale>0
	private float delicate;	//细致系数
	
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
	 * 钻石由两个对角的等腰三角形构成，每个三角形形成一个独立的二元三角树
	 *
	 */
	class Diamond{
		public TriTreeNode baseLeft;// 左二元三角树
		public TriTreeNode baseRight;// 右二元三角树
		public int x,y;
//		public byte height;
		
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
		}
	}

}
