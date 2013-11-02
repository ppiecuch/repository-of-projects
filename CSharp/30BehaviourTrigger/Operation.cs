
using System.Collections;
using System.Collections.Generic;

//操作
//TODO probability
public class Operation
{
    public enum Type
    {
        Custom = 0,                 //自定义
        Pathfinding,                //寻路
        StopMovement,               //停止移动
        PlayAnimation,              //播放动画
        UpdateAim,                  //更新目标
        Damage,                     //造成伤害
        COUNT
    }

    public delegate void VoidOperationHandler();
    public delegate void OperationHandler(params object[] os);


    protected Type type;
    protected object[] args = null;
    protected VoidOperationHandler voidOperationHandler = null;


    public virtual OperationHandler getOperationHandler() { return null; }

    public Type getType() { return type; }
    public void operate() {
        if (type == Type.Custom)
            voidOperationHandler();
        else
            getOperationHandler()(args); 
    }

    public Operation(Operation.Type type, params object[] args)
    {
        this.type = type;
        this.args = args;
    }

    public Operation(VoidOperationHandler vop)
    {
        this.type = Type.Custom;
        this.voidOperationHandler = vop;
    }
}