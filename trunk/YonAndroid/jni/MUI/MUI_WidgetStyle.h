#ifndef _MUI_WIDGETSTYLE_H_
#define _MUI_WIDGETSTYLE_H_

namespace mui{

	struct WidgetStyle{
	private:
		Enum value;
	public:
		//�Ӽ�/����/�ص�
		//����Window�µĴ��ڣ����������ԣ�
		//�Ӵ������ص����ڻ򵯳����ڵ���Ҫ�������Ӵ���ֻ�ܳ�������һ�������У������Ӵ��ڵ��κζ��ಿ�ֶ����ô�����ȥ����е���
		//�ص����ں͵������ڵ���Ҫ�����ǵ������ڳ���ʱ����û�б���(Ҳ��Ϊ������)��
		//��������ͨ���ԶԻ������Ϣ�����ʽ���û��Ի���
		//�ص�����ͨ�����ڽ���Ӧ�ó��������ڡ���ʵ�ϣ���ʱҲ�������������ڻ��ߡ���ܡ����ڡ�
		//�Ӵ���ͨ��������ͼ(View)�У������ı��༭���е��ı���ʾ��Ҳ���ڿؼ��У����ڶԻ����е�OK��ť��������Щ��������ť��ؼ����Ӵ��ڣ�Ҳ��Ϊ���ؼ������ڡ�
		enum Enum
		{
			Child, /**< child widget, cropped by parent widget borders, no overlapping (used by default for child widgets) */
			Popup, /**< popup widget, have parent widget, but not cropped on its borders */
			Overlapped, /**< child widget, cropped by parent widget borders, can overlap (used by default for root widgets) */
			COUNT
		};

		WidgetStyle() : 
			value(COUNT){
		}

		WidgetStyle(Enum value) :
			value(value){
		}

		friend bool operator == (const WidgetStyle& a, const WidgetStyle& b)
		{
			return a.value == b.value;
		}

		friend bool operator != (const WidgetStyle& a, const WidgetStyle& b)
		{
			return a.value != b.value;
		}

		const char* getValueName() const
		{
			static const char* values[COUNT + 1] = { "Child", "Popup", "Overlapped", "" };
			return values[(value < COUNT && value >= Child) ? value : COUNT];
		}
	};
}
#endif