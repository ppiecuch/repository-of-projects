
using System.Collections;
using System.Collections.Generic;

//条件
public class Condition
{
    public enum Type
    {
        Custom = 0,                 //自定义
        Timing,                     //计时
        CheckAim,                   //判断目标
        CheckState,                 //判断状态
        CheckDistance,              //判断距离
        COUNT
    }

    public enum Comparator
    {
        EQ = 0,
        NE,
        GE,
        GT,
        LE,
        LT
    }
    public static Condition TRUE = new Condition(() => { return true; });

    public delegate float GetFloatHandler();
    public delegate bool GetBoolHandler();
    public delegate bool CompareHandler(Comparator comparator, params object[] os);


    protected Type type;
    protected object[] args = null;
    protected Comparator comparator;
    protected GetBoolHandler boolHandler = null;

    public virtual CompareHandler getCompareHandler(){return null;}

    public Type getType() { return type; }
    public bool isTrue() {
        if (type == Type.Custom)
            return boolHandler();
        return getCompareHandler()(comparator, args); 
    }

    protected Condition(Condition.Type type,Condition.Comparator comparator, params object[] args)
    {
        this.type = type;
        this.comparator = comparator;
        this.args = args;
    }

    public Condition(GetBoolHandler gbh)
    {
        this.type = Type.Custom;
        this.boolHandler = gbh;
    }
}