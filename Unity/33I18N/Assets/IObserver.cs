using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


/// <summary>
///  * A class can implement the IObserver interface when it wants to be informed of changes in observable objects.
/// </summary>
public interface IObserver
{
    /// <summary>
    /// This method is called whenever the observed object is changed. 
    /// An application calls an Observableobject's notifyObservers method to have all the object's
    /// observers notified of the change.
    /// </summary>
    /// <param name="o">the observable object.</param>
    /// <param name="method">an argument passed to the notifyObservers method</param>
    void update(Observable o, params object[] args);
}