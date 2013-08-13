

//refer to:理解 MyGUI 界面库的 IUnlinkWidget 
//http://blog.csdn.net/liigo/article/details/7851835

//MyGUI::WidgetManager内部维护着一个IUnlinkWidget对象数组（std::vector<IUnlinkWidget*>），当某个MyGUI::Widget（窗口组件）即将被销毁之前，
//调用每个IUnlinkWidget对象的_unlinkWidget()方法。也就是说，用户通过向WidgetManager注册一个IUnlinkWidget对象，就可以监控任意Widget的销毁动作，以便在其被销毁之前做一些清理工作。

//在MyGUI内部，也多次使用了IUnlinkWidget机制监听窗口组件被销毁事件。
//如ToolTipManager在Widget销毁前重置提示框，LayerManager在Widget销毁前使其脱离layer，PointerManager在Widget销毁前重置鼠标指针，
//InputManager在Widget销毁前处理窗口焦点和模态窗口，ControllerManager在Widget销毁前更新其管理的动态窗口，Gui在Widget销毁前清理该Widget的所有帧事件delegate对象。

//最后给出一个思考题：为什么MyGUI不使用更具通用性的delegate事件委派机制处理Widget将被销毁事件？
//既然delegate已经在MyGUI内广泛使用了，为什么还要设计出另外一套IUnlinkWidget机制呢？后者比前者有何区别和优势？
