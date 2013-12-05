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

        public enum Event{
            Enter = 0,
        }

        abstract class BaseState{
            public enum State{
                A = 0,
                B
            }
            protected BaseState(){}
            public abstract State getState();
            public virtual void enter(BaseState from)
            {
                Console.WriteLine(this+" enter-from:" + from);
            }
            public virtual void exit(BaseState to)
            {
                Console.WriteLine(this + " exit-to:" + to);
            }
            public override string ToString()
            {
                return getState().ToString();
            }

            public override bool Equals(object obj)
            {
                BaseState bs = obj as BaseState;
                if (bs == null)
                    return false;
                return bs.getState() == getState();
            }

            public override int GetHashCode(){
                return getState().GetHashCode();
            }
        }

        class StateA : BaseState{
            private static StateA instance = null;
            public static StateA getInstance(){
                if (instance == null)
                    instance = new StateA();
                return instance;
            }
            protected StateA(){}
            public override State getState() { return State.A; }
        }

        class StateB : BaseState
        {
            private static StateB instance = null;
            public static StateB getInstance()
            {
                if (instance == null)
                    instance = new StateB();
                return instance;
            }
            protected StateB() { }
            public override State getState() { return State.B; }
        }

        //   <---Hold---
        //Idle         Run
        //   ---Push--->
        static void Main(string[] args)
        {
            /*StateMachine<DummyState, DummyEvent> sm = StateMachine<DummyState, DummyEvent>.getInstance();

            //sm.pushRule(DummyState.Idle, DummyEvent.Push, DummyState.Run, (DummyState last, DummyState curr) => { System.Console.WriteLine("idle--->push:"+last+","+curr); });
            //sm.pushRule(DummyState.Run, DummyEvent.Hold, DummyState.Idle, (DummyState last, DummyState curr) => { System.Console.WriteLine("run--->hold:" + last + "," + curr); });


            sm.pushRule(DummyState.Idle, DummyEvent.Push, (DummyState state, DummyEvent evt) => { DummyState result = DummyState.Run; System.Console.WriteLine(state + "," + evt + "->" + result); return result; });
            sm.pushRule(DummyState.Run, DummyEvent.Hold, (DummyState state, DummyEvent evt) => { DummyState result = DummyState.Idle; System.Console.WriteLine(state + "," + evt + "->" + result); return result; });

            sm.sendEvent(DummyEvent.Push);
            sm.sendEvent(DummyEvent.Hold);
            sm.sendEvent(DummyEvent.Hold);*/

            StateMachine<BaseState, Event> sm2 = StateMachine<BaseState, Event>.getInstance();
            sm2.pushRule(null, Event.Enter, (BaseState from, Event evt) =>
            {
                BaseState to = StateA.getInstance();
                if (from != null)
                    from.exit(to);
                to.enter(from);
                return to;
            });
            sm2.pushRule(StateA.getInstance(), Event.Enter, (BaseState from, Event evt) => {
                BaseState to=StateB.getInstance();
                if (from != null)
                    from.exit(to);
                to.enter(from);
                return to;
            });
            sm2.pushRule(StateB.getInstance(), Event.Enter, (BaseState from, Event evt) =>
            {
                BaseState to = StateA.getInstance();
                if (from != null)
                    from.exit(to);
                to.enter(from);
                return to;
            });

            sm2.sendEvent(Event.Enter);
            sm2.sendEvent(Event.Enter);
            sm2.sendEvent(Event.Enter);

            System.Console.ReadKey();
        }
    }
}
