package org.tinder.studio.lwjgl.roam;
/**
 *
 * 二元三角形
 * TriTreeNode Struct
 * Store the triangle tree data, but no coordinates!
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-21上午12:26:33
 *
 */
public class TriTreeNode {
	
	public TriTreeNode leftChild;		//左子节点
	public TriTreeNode rightChild;		//右子节点
	public TriTreeNode baseNeighbor;	//基邻接节点
	public TriTreeNode leftNeighbor;	//左邻接节点
	public TriTreeNode rightNeighbor;	//右邻接节点

}
