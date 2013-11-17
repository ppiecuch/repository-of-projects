using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

//资源
public class Resource {

    public enum Type{
        Prefab = 0,
        Font
    }

    private Type type;
    private string path;
    private Vector3 pos = Vector3.zero;
    private Quaternion angle = Quaternion.identity;
    private Vector3 scale = Vector3.one;
    private List<Resource> children;

    /********************************************/
    //Getter
    /********************************************/
    public Vector3 getPosition() { return pos; }
    public Quaternion getAngle() { return angle; }
    public Vector3 getScale() { return scale; }
    public Type getType() { return type; }
    public string getPath() { return path; }
    public List<Resource> getChildren() { return children; }


    /********************************************/
    //Setter
    /********************************************/

    public void addChild(Resource res)
    {
        if (children == null)
            children = new List<Resource>();

        if (res == null)
            throw new NullReferenceException();

        if (children.Contains(res))
        {
            Logger.warn(Module.Framework,"Resource::addChild failed for already exist it:" + res);
            return;
        }

        children.Add(res);
    }

    public Resource(Type type, string path){
        this.type = type;
        this.path = path;
    }

    public Resource(Type type, string path, Vector3 pos, Quaternion angle, Vector3 scale)
    {
        this.type = type;
        this.path = path;
        this.pos = pos;
        this.angle = angle;
        this.scale = scale;
    }

    public override string ToString()
    {
        return "{type:"+type+",path:"+path+"}";
    }

}
