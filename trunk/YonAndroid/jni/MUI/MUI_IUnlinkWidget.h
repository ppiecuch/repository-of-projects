

//refer to:��� MyGUI ������ IUnlinkWidget 
//http://blog.csdn.net/liigo/article/details/7851835

//MyGUI::WidgetManager�ڲ�ά����һ��IUnlinkWidget�������飨std::vector<IUnlinkWidget*>������ĳ��MyGUI::Widget���������������������֮ǰ��
//����ÿ��IUnlinkWidget�����_unlinkWidget()������Ҳ����˵���û�ͨ����WidgetManagerע��һ��IUnlinkWidget���󣬾Ϳ��Լ������Widget�����ٶ������Ա����䱻����֮ǰ��һЩ��������

//��MyGUI�ڲ���Ҳ���ʹ����IUnlinkWidget���Ƽ�����������������¼���
//��ToolTipManager��Widget����ǰ������ʾ��LayerManager��Widget����ǰʹ������layer��PointerManager��Widget����ǰ�������ָ�룬
//InputManager��Widget����ǰ�����ڽ����ģ̬���ڣ�ControllerManager��Widget����ǰ���������Ķ�̬���ڣ�Gui��Widget����ǰ�����Widget������֡�¼�delegate����

//������һ��˼���⣺ΪʲôMyGUI��ʹ�ø���ͨ���Ե�delegate�¼�ί�ɻ��ƴ���Widget���������¼���
//��Ȼdelegate�Ѿ���MyGUI�ڹ㷺ʹ���ˣ�Ϊʲô��Ҫ��Ƴ�����һ��IUnlinkWidget�����أ����߱�ǰ���к���������ƣ�
