
using System.Collections;
using System.Collections.Generic;



public class BehaviourTrigger
{
    private class Reflex{
        public List<Condition> conditions = null;
        public List<Operation> operations = null;

        public bool hasOperation() { return operations != null && operations.Count > 0; }

        public void addCondition(Condition condition)
        {
            if (conditions == null)
                conditions = new List<Condition>();
            conditions.Add(condition);
        }

        public void addOperation(Operation op)
        {
            if (operations == null)
                operations = new List<Operation>();
            operations.Add(op);
        }
    }
    private Callback.Type callback;
    private List<Reflex> reflexes = null;
    private Reflex currentReflex = null;
    private readonly BehaviourTrigger parent = null;
    private List<BehaviourTrigger> children = null;
    private IBehaviourTriggable behaviourTriggable = null;
    
    private bool enable = true;

    public BehaviourTrigger(){}
    private BehaviourTrigger(BehaviourTrigger parent){
        this.parent = parent;
    }

    /********************************************/
    //Getter
    /********************************************/

    public BehaviourTrigger getParent() { return parent; }

    public bool isEnable() { return enable; }


    /********************************************/
    //Setter
    /********************************************/

    private void initCompareHandler()
    {
        foreach(Reflex r in reflexes)
        {
            if (r.conditions != null)
            {
                foreach (Condition c in r.conditions)
                {
                    PrivateCondition pc = c as PrivateCondition;
                    if (pc != null)
                    {
                        Condition.CompareHandler ch = behaviourTriggable.getCompareHandler(c.getType());
                        if (ch == null)
                            Debug.LogWarning("WARN: no CompareHandler with Condition.Type:" + c.getType() + ", have you set it?");
                        else
                            pc.setCompareHandler(ch);
                    }
                }
            }
        }
    }

    private void initOperationHandler()
    {
        foreach (Reflex r in reflexes)
        {
            if (r.operations != null)
            {
                foreach (Operation op in r.operations)
                {
                    PrivateOperation po = op as PrivateOperation;
                    if (po != null)
                    {
                        Operation.OperationHandler oh = behaviourTriggable.getOperationHandler(po.getType());
                        if (oh == null)
                            Debug.LogWarning("WARN: no OperationHandler with Operation.Type:" + po.getType() + ", have you set it?");
                        else
                        {
                            Debug.Log("setOperationHandler:" + oh);
                            po.setOperationHandler(oh);
                        }
                    }
                }
            }
        }
    }

    public void setBehaviourTriggable(IBehaviourTriggable behaviourTriggable)
    {
        this.behaviourTriggable=behaviourTriggable;
        this.initCompareHandler();
        this.initOperationHandler();
        if(children!=null)
        {
            foreach(BehaviourTrigger bt in children)
            {
                bt.setBehaviourTriggable(behaviourTriggable);
            }
        }
    }

    public BehaviourTrigger setCallback(Callback.Type type)
    {
        this.callback=type;
        return this;
    }

    public BehaviourTrigger addCondition(Condition condition)
    {
        if (reflexes == null)
            reflexes = new List<Reflex>();
        if (currentReflex == null || currentReflex.hasOperation())
        {
            currentReflex = new Reflex();
            reflexes.Add(currentReflex);
        }
        currentReflex.addCondition(condition);
        return this;
    }

    public BehaviourTrigger addOperation(Operation op)
    {
        if (reflexes == null || currentReflex == null)
        {
            Debug.LogWarning("WARN: no condition checked, you must call addCondition first!");
            return this;
        }
        currentReflex.addOperation(op);
        return this;
    }

    public BehaviourTrigger addChild(){
        if (children == null)
            children = new List<BehaviourTrigger>();
        BehaviourTrigger bt=new BehaviourTrigger(this);
        children.Add(bt);
        return bt;
    }

    //TODO out
    public void trigger(Callback.Type type)
    {
        //Debug.Log("trigger:" + type);
        //TODO 子节点检测
        if (callback != type)
            return;

        if(!enable || reflexes==null)
            return;

        foreach(Reflex r in reflexes)
        {
            bool result = true;

            foreach (Condition c in r.conditions)
            {
                if (c.isTrue() == false)
                {
                    result = false;
                    break;
                }
            }

            if (result == false)
                continue;

            foreach (Operation op in r.operations)
            {
                //TODO probability
                op.operate();
            }
        }
    }

    public void setEnable(bool on)
    {
        this.enable = on;
    }

    
}