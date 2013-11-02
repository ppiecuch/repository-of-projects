using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _23StateMachine
{
    class Program
    {

        enum DummyState{
            Idle = 0,
            Run
        }

        enum DummyEvent{
            Push = 0,
            Hold
        }

        //   <---Hold---
        //Idle         Run
        //   ---Push--->
        static void Main(string[] args)
        {
            StateMachine<DummyState, DummyEvent> sm = StateMachine<DummyState, DummyEvent>.getInstance();

            //sm.pushRule(DummyState.Idle, DummyEvent.Push, DummyState.Run, (DummyState last, DummyState curr) => { System.Console.WriteLine("idle--->push:"+last+","+curr); });
            //sm.pushRule(DummyState.Run, DummyEvent.Hold, DummyState.Idle, (DummyState last, DummyState curr) => { System.Console.WriteLine("run--->hold:" + last + "," + curr); });


            sm.pushRule(DummyState.Idle, DummyEvent.Push, (DummyState state, DummyEvent evt) => { DummyState result = DummyState.Run; System.Console.WriteLine(state + "," + evt + "->" + result); return result; });
            sm.pushRule(DummyState.Run, DummyEvent.Hold, (DummyState state, DummyEvent evt) => { DummyState result = DummyState.Idle; System.Console.WriteLine(state + "," + evt + "->" + result); return result; });

            sm.sendEvent(DummyEvent.Push);
            sm.sendEvent(DummyEvent.Hold);
            sm.sendEvent(DummyEvent.Hold);

            System.Console.ReadKey();
        }
    }
}
