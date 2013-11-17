using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

//GameObject节点，支持树结构，主要用于ResourceManager
public class GameObjectNode {

    GameObject gameObject = null;
    private List<GameObjectNode> children = null;

    /********************************************/
    //Getter
    /********************************************/

    public GameObject getGameObject() { return gameObject; }
    public List<GameObjectNode> getChildren() { return children; }
    public GameObjectNode getChild(int index) { return children[index]; }

    /********************************************/
    //Setter
    /********************************************/

    public void setActive(bool on)
    {
        getGameObject().SetActive(on);
    }
    public void setColor(Color color, bool recursion=true)
    {
        if (getGameObject().renderer != null)
            getGameObject().renderer.material.color = color;
        if (!recursion)
            return;
        if (children != null)
        {
            foreach (GameObjectNode g in children)
            {
                g.setColor(color, true);
            }
        }
    }

    public void addChild(GameObjectNode gon)
    {
        if (children == null)
            children = new List<GameObjectNode>();

        if (gon == null)
            throw new NullReferenceException();

        if (children.Contains(gon))
        {
            Logger.warn(Module.Framework,"GameObjectNode::addChild failed for already exist it:" + gon);
            return;
        }

        children.Add(gon);
    }

    public GameObjectNode(GameObject obj)
    {
        this.gameObject = obj;
    }
}
