#ifndef _MUI_LAYERMANAGER_H_
#define _MUI_LAYERMANAGER_H_

namespace mui{

	class LayerManager{
	public:
		LayerManager();

		void initialize();
		void shutdown();

		/** 
		 *@brief Get top visible and enabled widget at specified position 
		 */
		//Widget* getWidgetFromPoint(int _left, int _top);
		Widget* getWidgetByPosition(s32 left, s32 top);

		/** 
		* @brief Attach widget to specific layer
		* @param name Layer name
		* @param item Widget pointer
		*/
		void attachToLayerNode(const core::stringc& name, Widget* item);

		/** 
		* @brief Detach widget from layer
		* @param _item Widget pointer
		*/
		void detachFromLayer(Widget* item);

		/** 
		* @brief Up widget to be on top of its layer
		* @param item Widget pointer
		*/
		//void upLayerItem(Widget* _item);
		void frontLayerItem(Widget* item);

		/** 
		* @brief Check is layer exist 
		*/
		//bool isExist(const std::string& _name) const;
		bool exist(const core::stringc& name) const;

		/** 
		* @brief Get layer by name 
		*/
		//ILayer* getByName(const std::string& _name, bool _throw = true) const;
		ILayer* getLayer(const core::stringc& name) const;

		u32 getLayerCount() const;

		ILayer* getLayer(u32 index);

		/** 
		* @brief Render all layers to specified target 
		*/
		void renderToTarget(IRenderTarget* target, bool update);

		//void resizeView(const IntSize& _viewSize);
		void onResize(const core::dimension2di& viewSize);
	};
}
#endif