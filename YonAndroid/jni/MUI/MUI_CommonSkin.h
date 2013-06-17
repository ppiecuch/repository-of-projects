#ifndef _MUI_COMMONSKIN_H_
#define _MUI_COMMONSKIN_H_

namespace mui{

	//''BasisSkin'' with type ''MainSkin'' or ''SubSkin'' is an actual part of texture that will be rendered.
	//Usually skins contain one part (part of texture stretched over whole widget) - SubSkin_type should be ''MainSkin'',
	//three parts (left part, stretched central part and right part) - SubSkin_type should be ''SubSkin''
	//or nine parts (one in center and 8 around) - SubSkin_type should be SubSkin.
	//You may have any amount of ''BasisSkin'' elements one for each texture part.

	/*
	Skin format

	MyGUI 3.0 skin format description. For skin examples look at Media/MyGUI_Media/core_skin.xml - all default skins described there.

	<?xml version="1.0" encoding="UTF-8"?>
	 
	 <MyGUI type="Resource" version="1.1">
	 
 		<Resource type="ResourceSkin" name = "__Skin_name__" size = "__width height__" texture = "__Texture_file_name__" >
 		<!- Size property is default widget size, subskins and child widgets alignment also rely on this size -->
 			<Property key="__Property_name__" value = "__Property_value__" /> 
 			<!- Some widet types have special properties, you can specify them in skin instead setting properties for each widget copy in layout or in code -->
	 
 			<Child type="__Widget_type__" skin="__Some_skin_name__" offset = "__left top width height__" align = "__Some_align__" name = "__Child_widget_name__">
 			<!- Offset and align is properties of child widget inside skin, they works exactly as position and align in layout files and in code when you creating widgets -->
 				<Property key="__Property_name__" value = "__Property_value__" />
 				<!- Child widgets also may have some properties --> 
 			</Child>
 			<!- Skin may have child widgets, for example Window have buttons on it and each button is child widget -->
	 
 			<BasisSkin type="__SubSkin_type__" offset = "__left top width height__" align = "__Some_align__">
 			<!- Offset and align here works same way as for child widgets -->
 				<State name="__state_name__" offset = "__left top width height__"/>
 				<!- Here is defined part of texture for different widget states. ''Offset'' is offset in texture that shown part of it that will be rendered. -->
 			</BasisSkin>
 			<!- ''BasisSkin'' with type ''MainSkin'' or ''SubSkin'' is an actual part of texture that will be rendered.
 				Usually skins contain one part (part of texture stretched over whole widget) - SubSkin_type should be ''MainSkin'',
 						  three parts (left part, stretched central part and right part) - SubSkin_type should be ''SubSkin''
 						  or nine parts (one in center and 8 around) - SubSkin_type should be SubSkin.
 				You may have any amount of ''BasisSkin'' elements one for each texture part.
 			-->
	 
 			<BasisSkin type="SimpleText" offset = "__left top width height__" align = "__Some_align__">
 			<!- Offset and align here works same way as for child widgets -->
 				<State name="__state_name__" colour="__R G B or #RRGGBB (hex)__"/>
 				<!- ''SimpleText'' may contain state description, it is optional. If it contain node - default colour will be used for all states. -->
 			</BasisSkin>
 			<!- ''SimpleText'' is place where widget text is placed. Also if this text should be selectable or editable you should use ''EditText'' type instead. >
 		</Resource>
	 
 		<!- Lots of other skins here ... -->
	 </MyGUI>
	 */

	//MyGUI_BlueWhiteSkins.xml的内容比较多，根节点下的一系列Resource子节点为的type属性均为ResourceSkin。ResourceSkin的属性包括了名字，大小，以及所使用的贴图文件。
	//每个ResourceSkin下包含若干个BasisSkin节点。
	//BasisSkin的type属性为SubSkin，其他属性包括了偏移和对齐方式。其下又包含了若干个State子节点，分别定义了不同状态的名字和对应的图标在贴图文件中的左上位置，高度和宽度。
	//（MyGUI_CommonSkins.xml中的结构与这个文件类似，但是却没有定义贴图文件，这样的结果暂时不明。）

	//class SubSkin
	class CommonSkin : public IShapWidget{
	protected:
		ILayerNode* m_pNode;
		RenderItem* m_pRenderItem;
		video::SColor m_color;

		bool m_bEmptyView;
		//IntCoord mCurrentCoord;
		core::recti m_croppedCoordinate;

		virtual void updateView();
		virtual void repairView();

		virtual void setAlign(const dimension2di& old);

		virtual void setUVRect(const core::rectf& rect);
		virtual void setColor(const video::SColor& value);
	public:
		CommonSkin();
		virtual ~CommonSkin();

		//TODO 是否可与setColor合并
		virtual void setAlpha(f32 alpha);

		virtual void setVisible(bool on);

		//virtual void setStateData(IStateInfo* _data);

		virtual void createDrawItem(const video::SMaterial& material, ILayerNode* node);
		virtual void destroyDrawItem();

		virtual void preRender();

		/*internal:*/
		//virtual void _updateView();
		//virtual void _correctView();

		//virtual void _setAlign(const IntSize& _oldsize);

		//virtual void _setUVSet(const FloatRect& _rect);
		//virtual void _setColour(const Colour& _value);

	};
}
#endif