
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class ConditionHelper
{
    /*
    public static bool checkDistance(Condition.Comparator comparator, params object[] args)
    {
        Fightable f1 = args[0] as Fightable;
        Fightable f2 = args[1] as Fightable;
        double value = Convert.ToDouble(args[2]);
        switch (comparator)
        {
            case Condition.Comparator.LE:
                return f1.getDistanceTo(f2) <= value;
            default:
                Debug.LogWarning("WARN: unexpect checkDistance!");
        }
        return false;
    }

    public static bool timing(Condition.Comparator comparator,params object[] args)
    {
        Condition.GetFloatHandler real = args[0] as Condition.GetFloatHandler;
        double interval = Convert.ToDouble(args[1]);
        switch(comparator)
        {
            case Condition.Comparator.GE:
                return Time.time - real() >= expected;
            default:
                Debug.LogWarning("WARN: unexpect timing!");
        }
        return false;
    }

    
    public static void pathFinding(params object[] args)
    {
        AStarHelper aStarHelper = args[0] as AStarHelper;
        Vector3 pos = (Vector3)args[1];
        aStarHelper.markTarget(pos);
    }*/
}