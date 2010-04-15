import javax.microedition.m3g.Appearance;
import javax.microedition.m3g.IndexBuffer;
import javax.microedition.m3g.VertexBuffer;


/**
 * ΚµΜε
 * @author micheal hong
 *
 */
public class Entity {
	private VertexBuffer vertexBuffer;
	private IndexBuffer indexBuffer;
	private Appearance appearance;
	
	public VertexBuffer getVertexBuffer() {
		return vertexBuffer;
	}
	public void setVertexBuffer(VertexBuffer vertexBuffer) {
		this.vertexBuffer = vertexBuffer;
	}
	public IndexBuffer getIndexBuffer() {
		return indexBuffer;
	}
	public void setIndexBuffer(IndexBuffer indexBuffer) {
		this.indexBuffer = indexBuffer;
	}
	public Appearance getAppearance() {
		return appearance;
	}
	public void setAppearance(Appearance appearance) {
		this.appearance = appearance;
	}
}
