using UnityEngine;
using System.Collections;
using System.Collections.Generic;

//可带资源的
public interface IResourcable {

    Resource getResource();

}

public abstract class MutableResourcable<T> : IResourcable
{
    private T mutableValue;

    public Resource getResource(){
        return getResource(this.mutableValue);
    }
    public void setMutableValue(T t){
        this.mutableValue = t;
    }
    public abstract Resource getResource(T t);

}

