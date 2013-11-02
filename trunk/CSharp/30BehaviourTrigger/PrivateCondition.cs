using System.Collections;
using System.Collections.Generic;

//私有条件
public class PrivateCondition : Condition
{
    private CompareHandler ch = null;

    public override CompareHandler getCompareHandler()
    {
        if(ch==null)
            Debug.LogWarning("WARN: no CompareHandler with type:" + type + ", have you set it?");
        return ch;
    }

    public void setCompareHandler(CompareHandler ch){
        this.ch=ch;
    }

    public PrivateCondition(Condition.Type type, Condition.Comparator comparator, params object[] args)
        : base(type,comparator,args)
    {
    }
}