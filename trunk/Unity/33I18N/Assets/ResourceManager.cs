using UnityEngine;
using System.Collections;
using System.Collections.Generic;

//资源管理器
public class ResourceManager {

    private static ResourceManager instance = null;

    public static ResourceManager getInstance(){
        if(instance==null)
            instance=new ResourceManager();
        return instance;
    }

    public GameObjectNode load(IResourcable r, GameObject parent)
    {
        return load(r.getResource(), parent);
    }

    public GameObjectNode load(Resource res, GameObject parent)
    {
        return load(res, parent, null);
    }

    private GameObjectNode load(Resource res, GameObject parent, GameObjectNode parentGon)
    {
        Object o = Resources.Load(res.getPath());
        if (o == null)
        {
            Logger.warn(Module.Resource,"Resources.Load:" + res + " failed!");
            return null;
        }
        if(res.getType()==Resource.Type.Font)
        {
            GameObjectNode g = new GameObjectNode(o as GameObject);
            return g;
        }
        GameObject obj = GameObject.Instantiate(o) as GameObject;

        GameObjectNode gon = new GameObjectNode(obj);

        if(parentGon!=null)
            parentGon.addChild(gon);

        List<Resource> ress = res.getChildren();
        if(ress!=null&&ress.Count>0)
        {
            foreach(Resource r in ress)
            {
                load(r, obj, gon);
            }
        }

        obj.transform.parent = parent.transform;
        obj.transform.name = subtractBracket(obj.transform.name);
        obj.transform.localScale = res.getScale();
        obj.transform.rotation = res.getAngle(); ;
        obj.transform.localPosition = res.getPosition();

        return gon;
    }

    private ResourceManager()
    {
    }

    private static string subtractBracket(string name)
    {
        if (name.EndsWith("(Clone)"))
        {
            return name.Substring(0, name.Length - 7);
        }
        return name;
    }

}
