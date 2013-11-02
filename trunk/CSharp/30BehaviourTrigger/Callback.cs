using System.Collections;
using System.Collections.Generic;

//回调
public class Callback
{
    public enum Type
    {
        OnUpdate = 0,               //每帧更新时
        OnCall,                     //主动呼叫时
        OnDiscover,                 //发现敌人时，TODO可能废弃
        OnMove,                     //开始移动时
        OnAttack,                   //起手攻击时
        OnDischarge,                //动作到达攻击点时
        OnHit,                      //命中时
        COUNT
    }
}