#ifndef _MUI_CONTROLLERMANAGER_H_
#define _MUI_CONTROLLERMANAGER_H_

namespace mui{

	class ControllerManager{
	private:
		//void _unlinkWidget(Widget* _widget);
		void frameEntered(f32 time);
		void clear();

		//typedef std::pair<Widget*, ControllerItem*> PairControllerItem;
		struct PairControllerItem{
			Widget* widget;
			ControllerItem* controllerItem;
		};
		typedef core::list<PairControllerItem> ListControllerItem;
		ListControllerItem m_listItem;

		bool m_bIsInitialise;
	public:
		ControllerManager();

		void initialise();
		void shutdown();

		/** 
		* @brief Create controller.
		* @param type Controller type name. Should be on of internal controllers or registered by user.
		*/
		ControllerItem* createItem(const core::stringc& type);

		/** 
		* @brief Add controller over widget.
		* 
		* If specified widget already have controller with same type previous controller removed.
		* @param widget to be controlled
		* @param item controller created through createItem(const core::stringc& type)
		* @note item will be deleted automatically at end of controller lifetime
		* (if not removed by removeItem(Widget* _widget) before)
		*/
		void addItem(Widget* widget, ControllerItem* item);

		/** 
		* @brief Stop and remove all controllers over specified widget.
		* @param widget Pointer to widget for removing control from.
		*/
		void removeItem(Widget* widget);
	};
}
#endif