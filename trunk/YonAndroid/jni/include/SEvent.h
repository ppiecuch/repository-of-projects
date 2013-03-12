#ifndef _YON_GUI_SEVENT_H_
#define _YON_GUI_SEVENT_H_

namespace yon{
namespace gui{

	//refer to:http://www.w3.org/TR/DOM-Level-3-Events/#event-flow

	/*
	tabindex, accesskey, onfocus, onblur, onselect, onchange, 
	onclick, ondblclick, onmousedown, onmouseup, onmouseover, 
	onmousemove, onmouseout, onkeypress, onkeydown, onkeyup
	*/

	//Event objects are dispatched to an event target. At the beginning of the dispatch, implementations must first determine the event object's propagation path.

	//The propagation path must be an ordered list of current event targets through which the event object must pass. 
	//For DOM implementations, the propagation path must reflect the hierarchical tree structure of the document. 
	//The last item in the list must be the event target; the preceding items in the list are referred to as the target's ancestors, 
	//and the immediately preceding item as the target's parent. Once determined, the propagation path must not be changed; 
	//for DOM implementations, this applies even if an element in the propagation path is moved within the DOM. or removed from the DOM.

	//Exceptions thrown inside event listeners must not stop the propagation of the event or affect the propagation path. 
	//The exception itself must not propogate outside the scope of the event handler.

	//This specification defines three event phases: capture phase; target phase; and bubble phase. 
	//Event objects complete these phases in the specified order using the partial propagation paths as defined below. 
	//A phase must be skipped if it is not supported, or if the event object's propagation has been stopped. 
	//For example, if the Event.bubbles attribute is set to false, the bubble phase will be skipped, 
	//and if Event.stopPropagation() has been called prior to the dispatch, all phases must be skipped.

	//1¡¢The capture phase: 
	//the event object must propagate through the target's ancestors from the defaultView to the target's parent. 
	//This phase is also known as the capturing phase. Event listeners registered for this phase must handle the event before it reaches its target.

	//2¡¢The target phase: 
	//the event object must arrive at the event object's event target. This phase is also known as the at-target phase. 
	//Event listeners registered for this phase must handle the event once it has reached its target. 
	//If the event type indicates that the event must not bubble, the event object must halt after completion of this phase.

	//3¡¢The bubble phase: 
	//the event object propagates through the target's ancestors in reverse order, starting with the target's parent and ending with the defaultView.
	//This phase is also known as the bubbling phase. Event listeners registered for this phase must handle the event after it has reached its target.

	class SEvent{
	public:
		enum ENUM_PHASE{
			CAPTURE = 0,
			TARGET,
			BUBBLE,
			PHASE_COUNT
		};

		enum ENUM_EVENT{

		};

		/// Constructor
		SEvent();
		/// Destructor
		virtual ~SEvent();

		/// Get the current propagation phase.
		/// @return Current phase the event is in.
		ENUM_PHASE getPhase() const;
		/// Set the current propagation phase
		/// @param phase Switch the phase the event is in
		void setPhase(ENUM_PHASE phase);

		/// Set the current element in the propagation.
		/// @param[in] element The current element.
		void setCurrentElement(Element* element);
		/// Get the current element in the propagation.
		/// @return The current element in propagation.
		Element* getCurrentElement() const;

		/// Get the target element
		/// @return The target element of this event
		Element* getTargetElement() const;

		/// Has the event been stopped?
		/// @return True if the event is still propogating
		bool isPropagating() const;
		/// Stops the propagation of the event wherever it is
		void stopPropagation();

	private:
		bool Interruptible;
		bool Interruped;
		ENUM_PHASE Phase;
	};
}
}
#endif