using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace _27Observer
{
    public interface IObserver<T>
    {
        void OnCompleted();
        void OnError(Exception error);
        void OnNext(T value);
    }
}
