using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _30BehaviourTrigger
{
    class Fightable : IBehaviourTriggable{

        protected List<BehaviourTrigger> behaviourTriggers = null;

        protected Condition.CompareHandler[] compares = null;
        protected Operation.OperationHandler[] operations = null;

        protected List<Fightable> sameSideFightables = null;
        protected List<Fightable> otherSideFightables = null;

        public void callback(Callback.Type cb)
        {
            foreach (BehaviourTrigger bt in behaviourTriggers)
                bt.trigger(cb);
        }

        public Condition.CompareHandler getCompareHandler(Condition.Type type)
        {
            return compares[(int)type];
        }
        public Operation.OperationHandler getOperationHandler(Operation.Type type)
        {
            return operations[(int)type];
        }

        public void registerCondition(Condition.Type type, Condition.CompareHandler ch)
        {
            compares[(int)type] = ch;
        }
        public void registerOperation(Operation.Type type, Operation.OperationHandler ch)
        {
            operations[(int)type] = ch;
        }

        public void pushBehaviourTrigger(BehaviourTrigger bt)
        {
            //TODO check null
            bt.setBehaviourTriggable(this);
            this.behaviourTriggers.Add(bt);
        }

        public Fightable()
        {
            this.behaviourTriggers = new List<BehaviourTrigger>();
            this.compares = new Condition.CompareHandler[(int)Condition.Type.COUNT];
            this.operations = new Operation.OperationHandler[(int)Operation.Type.COUNT];
        }

        
    }

    class Demo1<T1>{
        T1 obj;
    }

    class Demo2<T1,T2>
    {
        T1 obj1;
        T2 obj2;
    }

    class Demo3<T1, T2, T3>
    {
        T1 obj1;
        T2 obj2;
        T3 obj3;
    }

    class Demo<T,S>{

        public Demo(Demo1<T> obj){}

        public Demo(Demo2<T,S> obj){}

    }

    class Program
    {
        public static void generateDamage(){
            Debug.Log("generateDamage");
        }

        static void Main(string[] args)
        {
            Fightable fightable = new Fightable();

            BehaviourTrigger bt = null;

            bt = new BehaviourTrigger();
            bt.setCallback(Callback.Type.OnUpdate)
            .addCondition(Condition.TRUE)
            .addOperation(new Operation(generateDamage));
            fightable.pushBehaviourTrigger(bt);

            fightable.callback(Callback.Type.OnUpdate);

            Console.ReadKey();
        }
    }
}
