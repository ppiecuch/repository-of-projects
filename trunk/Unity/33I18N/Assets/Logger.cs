using UnityEngine;
using System;
using System.Collections;


public enum Module
{
    Framework = 0,      //总框架
    Net,                //网络模块
    UI,                 //UI框架
    StateMachine,       //状态机模块
    Resource,           //资源模块
    Login,              //登录模块
    Skill,              //技能模块
    Status,             //状态模块
    Behaviour,          //行为模块
    Command,            //指令模块
    Effect,             //特效模块
    Animation,          //动画模块
    Battle,             //战斗模块
    Hero,               //英雄
    Monster,            //怪物
    Town,               //领地
    GrowUp,             //角色成长
    MagicAcademy,       //魔法学院
    MainUI,             //主界面
    World,              //艾维斯世界
    Config,             //配置文件
    COUNT
}

public class Logger
{
    public enum Level{
        Debg = 0,
        Info,
        Warn,
        Eror
    }
    private static Level[] levels = new Level[(int)Module.COUNT];

    public static void init(){
        for(int i=0;i<(int)Module.COUNT;++i)
        {
            levels[i] = Level.Warn;
        }
    }
    public static void setLevel(Module module, Level lv)
    {
        levels[(int)module] = lv;
    }

    private static bool isEnable(Module m,Level lv)
    {
        return ((int)lv) >= ((int)levels[(int)m]);
    }

    public static void debug(Module module, object message)
    {
        if (isEnable(module,Level.Debg))
            Debug.Log("[DEBG][" + module + "]" + message );
    }
    public static void info(Module module, object message)
    {
        if (isEnable(module, Level.Info))
            Debug.Log("[INFO][" + module + "]" + message );
    }
    public static void warn(Module module, object message)
    {
        if (isEnable(module, Level.Warn))
            Debug.LogWarning("[WARN][" + module + "]" + message );
    }
    public static void error(Module module, object message)
    {
        if (isEnable(module, Level.Eror))
            Debug.LogError("[EROR][" + module + "]" + message );
    }
}
