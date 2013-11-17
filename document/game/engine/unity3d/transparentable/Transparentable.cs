using UnityEngine;
using System.Collections;

public class Transparentable : MonoBehaviour {

    private Shader normalShader = null;
    private Shader transparentShader = null;
    private bool transparent;

    private void setShader(Shader s)
    {
        MeshRenderer[] mrs = GetComponentsInChildren<MeshRenderer>();
        foreach (MeshRenderer mr in mrs)
        {
            foreach (Material m in mr.materials)
                m.shader = s;
        }
        SkinnedMeshRenderer[] smrs = GetComponentsInChildren<SkinnedMeshRenderer>();
        foreach (SkinnedMeshRenderer smr in smrs)
        {
            foreach (Material m in smr.materials)
                m.shader = s;
        }
    }

    public void setTransparent(bool on)
    {
        if (transparent == on)
            return;
        setShader(on ? transparentShader : normalShader);
        transparent = on;

        if (on)
            foreach (Material m in renderer.materials)
                m.SetColor("_Color", new Color(0.5f, 0.5f, 0.5f, 0.3f));
    }

	// Use this for initialization
	void Start () {
        this.normalShader = Shader.Find("Unlit/Transparent Cutout");
        this.transparentShader = Shader.Find("Mobile/Transparent/Vertex Color");
        this.transparent = false;
	}
	
	// Update is called once per frame
	void Update () {
	}
}
