
using System.Collections;
using System.Collections.Generic;

public interface IBehaviourTriggable
{
    Condition.CompareHandler getCompareHandler(Condition.Type type);
    Operation.OperationHandler getOperationHandler(Operation.Type type);

    void registerCondition(Condition.Type type, Condition.CompareHandler ch);
    void registerOperation(Operation.Type type, Operation.OperationHandler ch); 
    
}