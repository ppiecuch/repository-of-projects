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
public class TriTriangleNode {
	
	public TriTriangleNode leftChild;		//左子节点
	public TriTriangleNode rightChild;		//右子节点
	public TriTriangleNode baseNeighbor;	//对面邻接节点
	public TriTriangleNode leftNeighbor;	//左邻接节点
	public TriTriangleNode rightNeighbor;	//右邻接节点

}
