using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.cplusplus.com/forum/beginner/74144/

namespace _23StateMachine
{

    class StateMachine<S, E>
    {
        public delegate S OnStateChangeHandler(S state,E evt);

        protected class Key
        {
            S state;
            E evt;

            public Key(S state, E evt)
            {
                this.state = state;
                this.evt = evt;
            }

            public override bool Equals(object obj)
            {
                Key r = obj as Key;

                if (r == null)
                    return false;

                return state.Equals(r.state) && evt.Equals(r.evt);
            }

            public override int GetHashCode()
            {
                return state.GetHashCode() ^ evt.GetHashCode() + state.GetHashCode();
            }
        }

        protected Dictionary<Key, OnStateChangeHandler> rules = null;
        protected S last, current;

        private static StateMachine<S, E> instance = null;

        public static StateMachine<S, E> getInstance()
        {
            if (instance == null)
                instance = new StateMachine<S, E>();
            return instance;
        }

        protected StateMachine()
        {
            rules = new Dictionary<Key, OnStateChangeHandler>();
            last = current = default(S);
        }

        public void pushRule(S curr, E evt, OnStateChangeHandler osch)
        {
            Key k = new Key(curr, evt);
            if (!rules.ContainsKey(k))
            {
                rules.Add(k, osch);
                return;
            }
            //TODO Warn
            System.Console.WriteLine("WARN: already exist rule for " + curr + "," + evt);
        }

        public void sendEvent(E evt)
        {
            Key k = new Key(current, evt);
            if (rules.ContainsKey(k))
            {
                S tmp = current;
                current = rules[k](current, evt);
                last = tmp;
                return;
            }
            throw new Exception("Receive unexpected event:" + evt + ", have you pushed the rule?");
        }

        public S getState()
        {
            return current;
        }
    }

    /*
    class StateMachine<S,E>
    {
        public delegate void OnStateChangeHandler(S last, S curr);

        protected class Key
        {
            S state;
            E evt;

            public Key(S state, E evt)
            {
                this.state = state;
                this.evt = evt;
            }

            public override bool Equals(object obj)
            {
                Key r = obj as Key;

                if (r == null)
                    return false;

                return state.Equals(r.state) && evt.Equals(r.evt);
            }

            public override int GetHashCode()
            {
                return state.GetHashCode() ^ evt.GetHashCode() + state.GetHashCode();
            }
        }

        protected class Value
        {
            public S state { get; set; }
            public OnStateChangeHandler onStateChange { get; set; }

            public Value(S state, OnStateChangeHandler onStateChange)
            {
                this.state = state;
                this.onStateChange = onStateChange;
            }
        }

        protected Dictionary<Key, Value> rules = null;
        protected S last,current;

        private static StateMachine<S, E> instance = null;

        public static StateMachine<S, E> getInstance()
        {
            if (instance == null)
                instance = new StateMachine<S, E>();
            return instance;
        }

        protected StateMachine()
        {
            rules = new Dictionary<Key, Value>();
            last = current = default(S);
        }

        public void pushRule(S curr, E evt, S next, OnStateChangeHandler osch){
            Key k = new Key(curr, evt);
            if(!rules.ContainsKey(k))
            {
                rules.Add(k, new Value(next,osch));
                return;
            }
            //TODO Warn
            System.Console.WriteLine("WARN: already exist rule for " + curr+","+evt);
        }

        public void sendEvent(E evt){
            Key k = new Key(current, evt);
            if (rules.ContainsKey(k))
            {
                last = current;
                current = rules[k].state;
                rules[k].onStateChange(last, current);
                return;
            }
            throw new Exception("Receive unexpected event:" + evt + ", have you pushed the rule?");
        }

        public S getState(){
            return current;
        }
    }
     */
}
