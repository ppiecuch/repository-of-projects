using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.masnun.com/2012/04/13/implementing-the-observer-pattern-in-c-code-samples.html
//http://www.codeproject.com/Articles/88278/Observer-in-NET-4-0-with-IObserver-T


//Supported in: 4.5, 4
//.NET platform offers IObserver and IObservable interfaces to quickly get up and running with the observer pattern. 

namespace _27Observer
{
    class Program
    {
        static void Main(string[] args)
        {

            // Create a subject and the observable, inject the subject into the observable
            Subject subject = new Subject();
            SubjectObservable observable = new SubjectObservable(subject);

            //Create two observers
            SubjectObserver zeus = new SubjectObserver(observable, "Zeus");
            SubjectObserver ares = new SubjectObserver(observable, "Ares");

            // Run a loop and trigger the observer
            for (int i = 0; i < 5; i++)
            {
                Console.WriteLine("Counter: " + subject.Counter);
                observable.inspectSubject();

            }

            Console.ReadLine();
        }

        //
        // Class Subject - It has a counter object that increases everytime you inspect it
        //

        class Subject
        {
            private int _counter;
            public int Counter
            {
                get
                {
                    return ++_counter;
                }
            }

        }

        //
        // Class Subject Observable - The observable object that observes the subject and notifies the observers
        //

        class SubjectObservable : IObservable<Subject>
        {
            // List of observers and the instance of subject
            private List<IObserver<Subject>> _subjectObservers;
            private Subject _subject;

            // Accept a subject instance which we shall observe
            public SubjectObservable(Subject subject)
            {
                _subjectObservers = new List<IObserver<Subject>>();
                _subject = subject;

            }

            // This method allows the observers to attach themselves. It returns a disposer object to the observer
            // which the observer can utilize to unsubscribe
            public IDisposable Subscribe(IObserver<Subject> observer)
            {
                if (!_subjectObservers.Contains(observer))
                {
                    _subjectObservers.Add(observer);
                }

                return new Disposer(_subjectObservers, observer);
            }

            // This method is used to inspect the subject over time. Usually used with a timer or an event
            public void inspectSubject()
            {
                foreach (IObserver<Subject> observer in _subjectObservers)
                {
                    observer.OnNext(_subject);
                }
            }

            //
            // Private class Disposer: Implements the IDisposable. Observable returns an instance to the observer for easy unsubscription
            //
            private class Disposer : IDisposable
            {
                // The observers list recieved from the observable
                private List<IObserver<Subject>> _subjectObservers;
                // The observer instance to unsubscribe
                private IObserver<Subject> _observer;

                public Disposer(List<IObserver<Subject>> _subObservers, IObserver<Subject> observer)
                {
                    _subjectObservers = _subObservers;
                    _observer = observer;
                }

                public void Dispose()
                {
                    if (_subjectObservers.Contains(_observer))
                    {
                        _subjectObservers.Remove(_observer);
                    }
                }
            }
        }

        //
        // Class Subject Observer - The object that attaches itself to a observable and recieves notifications
        //

        class SubjectObserver : IObserver<Subject>
        {
            // The disposer returned by the observable when subscribing
            private IDisposable _disposer;

            // Unique instance name to track the observer
            private string _instaceName;

            public SubjectObserver(IObservable<Subject> provider, string instanceName)
            {
                if (provider != null)
                {
                    _disposer = provider.Subscribe(this);
                    _instaceName = instanceName;
                }
            }

            // The observable invokes this method to pass the Subject object to the observer
            public void OnNext(Subject value)
            {
                Console.WriteLine("[" + _instaceName + "] " + value.Counter);
            }

            // Usually called when a transmission is complete. Not implemented.
            public void OnCompleted()
            {
                throw new NotImplementedException();
            }

            // Usually called when there was an error. Didn't implement.
            public void OnError(Exception error)
            {
                throw new NotImplementedException();
            }

            // Invoke the disposer recieved from the observable. This will unsubscribe
            public void Dispose()
            {
                _disposer.Dispose();
            }
        }
    }
}
