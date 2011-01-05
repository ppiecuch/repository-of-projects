import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

/**
 * A星算法
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-9-26 下午03:14:42
 *
 */
public class AStar {
	
	private List<Node> open;
	private List<Node> close;
	
	private Node start;//起点
	private Node end;//终点
	
	private boolean[][] grid;//地图数据
	private int gridWidth,gridHeight;//地图宽高
	
	public AStar(){
		open=new LinkedList<Node>();
		close=new LinkedList<Node>();
	}
	
	/**
	 * 初始化
	 * @param grid
	 */
	public void init(boolean[][] grid){
		this.grid=grid;
		this.gridWidth=grid[0].length;
		this.gridHeight=grid.length;
	}
	
	/**
	 * 寻找路径，如果找到则返回路径，否则返回null
	 * @param startX
	 * @param startY
	 * @param endX
	 * @param endY
	 * @return
	 */
	public int[][] search(int startX,int startY,int endX,int endY){
		open.clear();
		close.clear();
		start=new Node(startX,startY,null);
		end=new Node(endX,endY,null);
		
		open.add(start);
		evaluate(start);
		
		Node node=null;
		Node[] neighbour=new Node[8];//从上顺时针走向以0到8元素表示
		
		while(open.isEmpty()==false)
		{
			/* 在开启表中寻找f值最小的结点*/
			node=open.remove(0);
			/*将node添加到close表中*/
			close.add(node);
			/*
			 * 获取node的相邻节点并做如下处理：
			 * ①如果它不可通过或者已经在关闭列表中，略过它。反之如下。
			 * ②如果它不在开启列表中，把它添加进去。把当前格作为这一格的父节点。记录这一格的F,G,和H值。
			 * ③如果它已经在开启列表中，用G值为参考检查新的路径是否更好。更低的G值意味着更好的路径。如果是这样，就把这一格的父节点改成当前格，并且重新计算这一格的G和F值。如果你保持你的开启列表按F值排序，改变之后你可能需要重新对开启列表排序。
			 */
			getNeighbour(node, neighbour);
			int index;
			for(int i=0;i<neighbour.length;i++)
			{
				if(neighbour[i]==null)
					continue;
				if((index=contain(open, neighbour[i].x, neighbour[i].y))!=-1)
				{
					if(open.get(index).gn>neighbour[i].gn)
						open.set(index,neighbour[i]);
				}
				else
					open.add(neighbour[i]);
			}
			/*判断是否到达目标*/
			if((index=contain(open, end.x,end.y))!=-1)
			{
				int count=1;
				Node d=open.get(index);
				while(d.father!=null)
				{
					d=d.father;
					count++;
				}
				
				d=open.get(index);
				int[][] path=new int[count][];
				for(int i=count-1;i>=0;i--)
				{
					path[i]=new int[2];
					path[i][0]=d.x;
					path[i][1]=d.y;
					d=d.father;
				}
				return path;
			}
			/*对open表进行排序(按fn由小到大)*/
			Collections.sort(open);
		}
		return null;
		
	}
	
	
	private void getNeighbour(Node node,Node[] neighbour)
	{
		int x=node.x;
		int y=node.y;
		for(int i=0;i<neighbour.length;i++)
			neighbour[i]=null;
		neighbour[0]=getNode(x,  y-1,node);
		neighbour[1]=getNode(x+1,y-1,node);
		neighbour[2]=getNode(x+1,y,node);
		neighbour[3]=getNode(x+1,y+1,node);
		neighbour[4]=getNode(x,  y+1,node);
		neighbour[5]=getNode(x-1,y+1,node);
		neighbour[6]=getNode(x-1,y,node);
		neighbour[7]=getNode(x-1,y-1,node);
	}
	
	/*
	 * 对相邻的8格中的每一个,如果它不可通过或者已经在关闭列表中，略过它。
	 * 并计算它的评估值
	 */
	private Node getNode(int x,int y,Node father)
	{
		if(x>=0&&x<gridWidth&&y>=0&&y<gridHeight&&isPassable(x, y)&&contain(close,x,y)==-1)
		{
			Node node=new Node(x,y,father);
			evaluate(node);
			return node;
		}
		return null;
	}
	
	/*
	 * 如果包含则返回索引，否则返回-1
	 */
	private int contain(List<Node> nodes,int x,int y)
	{
		Node node=null;
		for(int i=0;i<nodes.size();i++)
		{
			node=nodes.get(i);
			if(node.x==x&&node.y==y)
				return i;
		}
		return -1;
	}
	
	/*
	 * 是否可通行
	 */
	private boolean isPassable(int x,int y)
	{
		return grid[y][x];
	}
	
	/*
	 * 路径评分
	 */
	private void evaluate(Node node){
		if(node.father!=null)
		{
			node.gn=node.father.gn+heuristic(node.father,node);
			node.fn=node.gn+heuristic(node, end);
		}
		else
		{
			node.gn=0;
			node.fn=heuristic(node, end);
		}
	}
	
	/*
	 * 启发式评估函数，使用曼哈顿距离
	 */
	private int heuristic(Node n1,Node n2)
	{
		return Math.abs(n1.x-n2.x)+Math.abs(n1.y-n2.y);
	}
	


	/*
	 * 选择路径中经过哪个方格的关键是下面这个等式：
	 * F = G + H
	 * 这里：
     * G = 从起点A，沿着产生的路径，移动到网格上指定方格的移动耗费。
     * H = 从网格上那个方格移动到终点B的预估移动耗费。
	 */
	class Node implements Comparable<Node>{
		int x,y;
		int fn;
		int gn;
		Node father;
		
		public Node(int x,int y,Node father){
			this.x=x;
			this.y=y;
			this.father=father;
		}
		
		public int compareTo(Node node) {
			return fn-node.fn;
		}
	}
	
	public static void main(String[] args){
		Calculagraph.getInstance("AStar").mark(null);
		boolean[][] grid={
				{true,true,true,false,true,true,true,true,true,true,true},
				{true,true,false,true,true,false,true,false,true,true,true},
				{true,true,false,false,false,false,false,false,true,true,true},
				{true,false,false,true,false,false,true,true,true,true,true},
				{true,true,true,false,true,false,true,false,true,false,true},
				{true,true,true,false,true,true,false,true,true,false,true},
				{true,true,true,true,false,false,false,true,true,true,true},
				{true,true,true,false,true,false,true,false,true,false,true},
				{true,true,true,false,true,true,false,false,true,false,true},
				{true,true,true,false,true,true,false,true,false,false,false},
				{true,true,true,false,true,true,false,true,true,true,true}
				};
		AStar a=new AStar();
		a.init(grid);
		int[][] path=a.search(0, 0, 10, 10);
		Calculagraph.getInstance("AStar").mark("finish");
		if(path!=null)
		{
			for(int i=0;i<path.length;i++)
			{
				if(i>0)
					System.out.print("-->");
				System.out.print(path[i][0]+","+path[i][1]);
			}
		}
		System.out.println();
		Calculagraph.getInstance("AStar").mark("print");
		System.out.println(Calculagraph.remove("AStar").getInfo());
	}

}
