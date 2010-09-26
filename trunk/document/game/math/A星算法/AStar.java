import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

/**
 * A���㷨
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-9-26 ����03:14:42
 *
 */
public class AStar {
	
	private List<Node> open;
	private List<Node> close;
	
	private Node start;//���
	private Node end;//�յ�
	
	private boolean[][] grid;//��ͼ����
	private int gridWidth,gridHeight;//��ͼ���
	
	public AStar(){
		open=new LinkedList<Node>();
		close=new LinkedList<Node>();
	}
	
	/**
	 * ��ʼ��
	 * @param grid
	 */
	public void init(boolean[][] grid){
		this.grid=grid;
		this.gridWidth=grid[0].length;
		this.gridHeight=grid.length;
	}
	
	/**
	 * Ѱ��·��������ҵ��򷵻�·�������򷵻�null
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
		Node[] neighbour=new Node[8];//����˳ʱ��������0��8Ԫ�ر�ʾ
		
		while(open.isEmpty()==false)
		{
			/* �ڿ�������Ѱ��fֵ��С�Ľ��*/
			node=open.remove(0);
			/*��node��ӵ�close����*/
			close.add(node);
			/*
			 * ��ȡnode�����ڽڵ㲢�����´���
			 * �����������ͨ�������Ѿ��ڹر��б��У��Թ�������֮���¡�
			 * ����������ڿ����б��У�������ӽ�ȥ���ѵ�ǰ����Ϊ��һ��ĸ��ڵ㡣��¼��һ���F,G,��Hֵ��
			 * ��������Ѿ��ڿ����б��У���GֵΪ�ο�����µ�·���Ƿ���á����͵�Gֵ��ζ�Ÿ��õ�·����������������Ͱ���һ��ĸ��ڵ�ĳɵ�ǰ�񣬲������¼�����һ���G��Fֵ������㱣����Ŀ����б�Fֵ���򣬸ı�֮���������Ҫ���¶Կ����б�����
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
			/*�ж��Ƿ񵽴�Ŀ��*/
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
			/*��open���������(��fn��С����)*/
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
	 * �����ڵ�8���е�ÿһ��,���������ͨ�������Ѿ��ڹر��б��У��Թ�����
	 * ��������������ֵ
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
	 * ��������򷵻����������򷵻�-1
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
	 * �Ƿ��ͨ��
	 */
	private boolean isPassable(int x,int y)
	{
		return grid[y][x];
	}
	
	/*
	 * ·������
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
	 * ����ʽ����������ʹ�������پ���
	 */
	private int heuristic(Node n1,Node n2)
	{
		return Math.abs(n1.x-n2.x)+Math.abs(n1.y-n2.y);
	}
	


	/*
	 * ѡ��·���о����ĸ�����Ĺؼ������������ʽ��
	 * F = G + H
	 * ���
     * G = �����A�����Ų�����·�����ƶ���������ָ��������ƶ��ķѡ�
     * H = ���������Ǹ������ƶ����յ�B��Ԥ���ƶ��ķѡ�
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
