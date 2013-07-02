#ifndef _MUI_LAYERMANAGER_H_
#define _MUI_LAYERMANAGER_H_

namespace mui{

	class LayerManager{
	public:

		/** 
		 *@brief Get top visible and enabled widget at specified position 
		 */
		//Widget* getWidgetFromPoint(int _left, int _top);
		Widget* getWidgetByPosition(s32 left, s32 top);
	};
}
#endif