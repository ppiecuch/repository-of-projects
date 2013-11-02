
using System.Collections;
using System.Collections.Generic;

//操作
//TODO probability
public abstract class Operation
{
    public abstract void operate();
}

public class Operation0 : Operation
{
    public delegate void OperationHandler();

    private OperationHandler ch;

    public override void operate()
    {
        ch();
    }

    public Operation0(OperationHandler ch)
    {
        this.ch = ch;
    }
}

public class Operation1<T> : Operation
{
    public delegate T GetValueHandler();
    public delegate void OperationHandler(T obj);

    private OperationHandler ch;
    private GetValueHandler gvh;
    private T value;

    public override void operate()
    {
        if (gvh != null)
            value = gvh();
        ch(value);
    }

    public Operation1(OperationHandler ch, T value)
    {
        this.ch = ch;
        this.value = value;
    }

    public Operation1(OperationHandler ch, GetValueHandler gvh)
    {
        this.ch = ch;
        this.gvh = gvh;
    }
}

public class Operation2<T1, T2> : Operation
{
    public delegate T1 GetValue1Handler();
    public delegate T2 GetValue2Handler();
    public delegate void OperationHandler(T1 obj1, T2 obj2);

    private OperationHandler ch;
    private GetValue1Handler gvh1;
    private GetValue2Handler gvh2;
    private T1 value1;
    private T2 value2;

    public override void operate()
    {
        if (gvh1 != null)
            value1 = gvh1();
        if (gvh2 != null)
            value2 = gvh2();
        ch(value1, value2);
    }

    private Operation2(OperationHandler ch)
    {
        this.ch = ch;
    }

    public Operation2(OperationHandler ch, T1 value1, T2 value2)
        : this(ch)
    {
        this.value1 = value1;
        this.value2 = value2;
    }

    public Operation2(OperationHandler ch, GetValue1Handler gvh1, GetValue2Handler gvh2)
        : this(ch)
    {
        this.gvh1 = gvh1;
        this.gvh2 = gvh2;
    }

    public Operation2(OperationHandler ch, T1 value1, GetValue2Handler gvh2)
        : this(ch)
    {
        this.value1 = value1;
        this.gvh2 = gvh2;
    }

    public Operation2(OperationHandler ch, GetValue1Handler gvh1, T2 value2)
        : this(ch)
    {
        this.gvh1 = gvh1;
        this.value2 = value2;
    }
}
//deprecated
/*
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
*/