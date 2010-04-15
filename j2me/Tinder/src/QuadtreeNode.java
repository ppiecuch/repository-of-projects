import java.util.Vector;


public class QuadtreeNode {
	
	private QuadtreeNode[] children=null;
	private Vector entitys=new Vector();
	
	/**
	 * 构造level层四叉树
	 * 注:因为使用了递归,有待优化
	 * @param level
	 */
	public QuadtreeNode(int level)
	{
		if(level>0)
		{
			this.children=new QuadtreeNode[4];
			for(int i=0;i<this.children.length;i++)
				this.children[i]=new QuadtreeNode(level-1);
			
		}
		System.out.println(level+":"+this);
	}

	public QuadtreeNode[] getChildren() {
		return children;
	}

	public void setChildren(QuadtreeNode[] children) {
		this.children = children;
	}

	public Vector getEntitys() {
		return entitys;
	}

	public void setEntitys(Vector entitys) {
		this.entitys = entitys;
	}
	
	

}
