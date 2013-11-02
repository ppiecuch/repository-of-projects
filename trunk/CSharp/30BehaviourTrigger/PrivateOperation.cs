
using System.Collections;
using System.Collections.Generic;

//私有操作
public class PrivateOperation : Operation
{
    private OperationHandler operation = null;

    public override OperationHandler getOperationHandler()
    {
        if(operation==null)
            Debug.LogWarning("WARN: no OperationHandler with type:" + type +", have you set it?");
        return operation;
    }

    public void setOperationHandler(OperationHandler operation){
        this.operation=operation;
    }

    public PrivateOperation(Operation.Type type, params object[] args)
        : base(type,args)
    {
    }
}