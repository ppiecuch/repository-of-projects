using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _23StateMachine
{
    class StateMachine<S,E,P>
    {
        public delegate void OnEvent(S last, S curr)
    }
}
