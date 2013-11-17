using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


/// <summary>
/// This class represents an observable object, or "data"
/// in the model-view paradigm. It can be subclassed to represent an 
/// object that the application wants to have observed. 
/// 
/// An observable object can have one or more observers. An observer 
/// may be any object that implements interface IObserver. After an 
/// observable instance changes, an application calling the 
/// Observable's notifyObservers method causes all of its observers to
/// be notified of the change by a call to their update method. 
/// </summary>
public class Observable
{
    public delegate void SetChangedHandler();
    public delegate void ClearChangedHandler();

    private bool changed = false;
    private LinkedList<IObserver> obs = null;

    public Observable()
    {
        obs = new LinkedList<IObserver>();
    }

    //User with IObserverable
    public Observable(out SetChangedHandler handler1,out ClearChangedHandler handler2) : this()
    {
        handler1 = setChanged;
        handler2 = clearChanged;
    }

    //TODO sync
    public void addObserver(IObserver o)
    {
        if (o == null)
            throw new NullReferenceException();
        if (!obs.Contains(o))
        {
            obs.AddLast(o);
        }
    }

    //TODO sync
    public void deleteObserver(IObserver o)
    {
        obs.Remove(o);
    }

    public void notifyObservers()
    {
        notifyObservers(null);
    }

    //TODO sync
    public void notifyObservers(params object[] args)
    {
        if (!changed)
            return;
        foreach (IObserver i in obs)
        {
            i.update(this, args);
        }
    }

    //TODO sync
    protected void setChanged()
    {
        changed = true;
    }

    //TODO sync
    protected void clearChanged()
    {
        changed = false;
    }

    //TODO sync
    public bool hasChanged()
    {
        return changed;
    }

    //TODO sync
    //Returns the number of observers of this Observable object.
    public int countObservers()
    {
        return obs.Count;
    }
}