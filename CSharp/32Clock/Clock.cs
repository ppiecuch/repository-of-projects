using System;
using System.Collections;
using System.Collections.Generic;

public class Clock
{
    public enum Stage{
		None = -3,
		Start = -2,
        Complete = -1,
		Setup = 0,
		Tick,
		Finish,
        COUNT
	}
	private readonly ulong delay;           //0表示没有延迟
	private readonly ulong interval;        //0表示没有间隔限制
	private readonly ulong duration;		//0表示没有时间限制
	private readonly uint limitation;	    //0表示没有次数限制

    private readonly byte probability;      //触发机率[0,100]
    private readonly ulong triggerInterval; //触发最小时间间隔,0表示没有间隔限制

	public delegate void CallbackHandler(Clock c, Stage s,uint counter);

    private ulong startTime;
    private ulong latestTriggerTime;        //最近一次的触发时间
    private uint counter;                   //触发次数
    private Stage stage;
    private CallbackHandler[] onTriggers = null;
    private Random random = null;

    private Clock(){
        this.onTriggers=new CallbackHandler[(int)Stage.COUNT];
        this.counter=0;
        this.startTime=0;
        this.latestTriggerTime = 0;
        this.stage=Stage.None;
    }

    public Clock(ulong duration, ulong delay, ulong interval, uint limitation, byte probability, ulong triggerInterval)
        : this()
    {
        if (probability > 100)
        {
            probability = 100;
            //TODO WARN
        }
        else
            random = new Random();
        this.duration = duration;
        this.delay = delay;
        this.interval = interval;
        this.limitation = limitation;
        this.probability = probability;
        this.triggerInterval = triggerInterval;
    }

    public Clock(ulong duration,ulong delay,ulong interval,uint limitation)
        :this(duration,delay,interval,limitation,100,0)
    {
    }

	public Clock(CallbackHandler tick)
        :this(0,0,0,1)
    {
        setTick(tick);
    }

    public ulong getRemainingTime(ulong time){
        if (time <= (startTime + delay))
            return duration;
        if (time >= (startTime + delay) + duration)
            return 0;
        return (startTime + delay) + duration - time;
    }

    public Clock setSetup(CallbackHandler ch)
    {
        onTriggers[(int)Stage.Setup]=ch;
        return this;
    }
    public Clock setTick(CallbackHandler ch)
    {
        onTriggers[(int)Stage.Tick]=ch;
        return this;
    }
    public Clock setFinish(CallbackHandler ch)
    {
        onTriggers[(int)Stage.Finish]=ch;
        return this;
    }

	public void start(ulong time){
        startTime=time;
        stage=Stage.Start;
    }

	public void finish(){
        stage=Stage.Finish;
    }

    private bool hitOrMiss(ulong time)
    {
        if (probability == 100)
            return true;
        if (time < latestTriggerTime + triggerInterval)
            return false;
        return random.Next(100) < probability;
    }

    private void tick(ulong time){
        if (hitOrMiss(time) == false)
            return;
        latestTriggerTime = time;
        ++counter;
        if (onTriggers[(int)Stage.Tick] != null)
            onTriggers[(int)Stage.Tick](this, Stage.Tick, counter);
    }

	public bool update(ulong time)
    {
        if (stage == Stage.None || stage == Stage.Complete)
            return true;

        if (stage == Stage.Start)
        {
            if(time - startTime < delay)
                return false;
            else
                stage = Stage.Setup;
        }

        if(stage==Stage.Setup)
        {
            if (onTriggers[(int)Stage.Setup] != null)
                onTriggers[(int)Stage.Setup](this, Stage.Setup, 1);
            stage = Stage.Tick;
        }

        if (stage == Stage.Tick)
        {
            //if(limitation==0&&interval==0)
            if(interval==0)
            {
                tick(time);
            }
            else
            {
                while ((limitation == 0 || counter < limitation) && time >= (startTime + delay) + counter * interval)
                {
                    tick(time);
                }
            }
        }

        if ((limitation == 0 || counter == limitation) && time >= startTime + delay + duration)
            stage = Stage.Finish;

        if(stage==Stage.Finish)
        {
            if (onTriggers[(int)Stage.Finish] != null)
                onTriggers[(int)Stage.Finish](this, Stage.Finish, 1);
            stage = Stage.Complete;
        }

        return false;
    }

}
