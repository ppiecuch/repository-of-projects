
using System.Collections;
using System.Collections.Generic;

//通用条件
public class CommonCondition : Condition
{
    private readonly CompareHandler ch = null;

    public override CompareHandler getCompareHandler(){
        return ch;
    }

    public CommonCondition(CompareHandler ch, Condition.Type type, Condition.Comparator comparator, params object[] args)
        : base(type,comparator,args)
    {
        this.ch = ch;
    }
}