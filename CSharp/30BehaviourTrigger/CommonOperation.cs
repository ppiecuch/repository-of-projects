using System.Collections;
using System.Collections.Generic;

//通用操作
public class CommonOperation : Operation
{
    private readonly OperationHandler operation = null;

    public override OperationHandler getOperationHandler()
    {
        return operation;
    }

    public CommonOperation(OperationHandler operation, Operation.Type type, params object[] args)
        : base(type,args)
    {
        this.operation = operation;
    }
}