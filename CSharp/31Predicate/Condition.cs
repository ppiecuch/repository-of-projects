using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public abstract class Condition{

    public virtual bool isTrue(){
        return true;
    }
}

public class Condition0 : Condition{

    public delegate bool ConditionHandler();

    private ConditionHandler ch;

    public override bool isTrue()
    {
        return ch();
    }

    public Condition0(ConditionHandler ch){
        this.ch = ch;
    }
}

public class Condition1<T> : Condition
{
    public delegate T GetValueHandler();
    public delegate bool ConditionHandler(T obj);

    private ConditionHandler ch;
    private GetValueHandler gvh; 
    private T value;

    public override bool isTrue()
    {
        if (gvh != null)
            return ch(gvh());
        return ch(value);
    }

    public Condition1(ConditionHandler ch, T value)
    {
        this.ch = ch;
        this.value = value;
    }

    public Condition1(ConditionHandler ch, GetValueHandler gvh)
    {
        this.ch = ch;
        this.gvh = gvh;
    }
}

public class Condition2<T1,T2> : Condition
{
    public delegate T1 GetValue1Handler();
    public delegate T2 GetValue2Handler();
    public delegate bool ConditionHandler(T1 obj1,T2 obj2);

    private ConditionHandler ch;
    private GetValue1Handler gvh1;
    private GetValue2Handler gvh2;
    private T1 value1;
    private T2 value2;

    public override bool isTrue()
    {
        if (gvh1 != null)
            value1=gvh1();
        if (gvh2 != null)
            value2 = gvh2();
        return ch(value1,value2);
    }

    private Condition2(ConditionHandler ch)
    {
        this.ch = ch;
    }

    public Condition2(ConditionHandler ch, T1 value1, T2 value2)
        :this(ch)
    {
        this.value1 = value1;
        this.value2 = value2;
    }

    public Condition2(ConditionHandler ch, GetValue1Handler gvh1, GetValue2Handler gvh2)
        : this(ch)
    {
        this.gvh1 = gvh1;
        this.gvh2 = gvh2;
    }

    public Condition2(ConditionHandler ch, T1 value1, GetValue2Handler gvh2)
        : this(ch)
    {
        this.value1 = value1;
        this.gvh2 = gvh2;
    }

    public Condition2(ConditionHandler ch, GetValue1Handler gvh1, T2 value2)
        : this(ch)
    {
        this.gvh1 = gvh1;
        this.value2 = value2;
    }
}
