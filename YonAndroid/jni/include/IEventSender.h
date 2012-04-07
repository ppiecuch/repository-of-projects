#ifndef _YON_EVENT_IEVENTSENDER_H_
#define _YON_EVENT_IEVENTSENDER_H_

#include "IEventReceiver.h"
#include "yonList.h"

namespace yon{
namespace event{

	class IEventSender{
	protected:
		core::list<IEventReceiver*> m_pReceivers;
	public:
		void registerReceiver(const IEventReceiver* receiver){
			m_pReceivers.push_back(receiver);
		}
		bool unregisterReceiver(const IEventReceiver* receiver){
			core::list<IEventReceiver*>::Iterator it = m_pReceivers.begin();
			for (; it != m_pReceivers.end(); ++it){
				if ((*it) == receiver)
				{
					m_pReceivers.erase(it);
					return true;
				}
			}
			return false;
		}
		void dispatchEvent(event::SEvent& event){
			core::list<IEventReceiver*>::Iterator it = m_pReceivers.begin();
			for (; it != m_pReceivers.end(); ++it){
				(*it)->onEvent(event);
			}
		}
	};
}
}
#endif