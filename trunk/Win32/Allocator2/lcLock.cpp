#include "lcLock.h"


CriticalSectionLock::CriticalSectionLock(){
	//InitializeCriticalSectionAndSpinCount()
	//refer to:http://hi.baidu.com/sei_zhouyu/item/6c2e22fd5e4e2c1cfe358294
	//ʵ���϶� CRITICAL_SECTION �Ĳ����ǳ�������Ϊʲô��Ҫ������ת���Ķ����أ���ʵ��������ڵ�cpu�ĵ������ǲ������õģ�ֻ�е������ϴ��ڲ�ֹһ��cpu������һ��cpu����˵�ʱ�򣬲Ź��á� 
	//����ٽ������������Ĳ�����ʱ�ǳ����ݣ�������Ǳ���һ��reference counter������ĳһ��flag����ô����ʱ�������Ժ�ͻ��뿪�ٽ�����
	//���ǵ����thread��û���뿪�ٽ���֮ǰ������һ��thread��ͼ������ٽ��������������ֻ�ᷢ���ڶ�˻���smp��ϵͳ�ϡ��������޷����룬�������thread�����˯�ߣ�Ȼ��ᷢ��һ���������л���
	//����֪�� context switch��һ���ȽϺ�ʱ�Ĳ�������˵��Ҫ��ǧ��ʱ�����ڣ���ô��ʵ����ֻҪ�ٵȶ༸��ʱ�����ھ��ܹ������ٽ���������ȴ������ǧ��ʱ�����ڵĿ����������ǿ����벻���̡� 
	//���Ծ�������InitializeCriticalSectionAndSpinCount���������ĵ�һ��������ָ��cs��ָ�룬�ڶ�����������ת�Ĵ�����
	//�ҵ�������һ��ѭ������������˵N����ô����˵��ʱEnterCriticalSection()�������ڲ�ѭ���жϴ��ٽ����Ƿ���Խ��룬ֱ�����Խ������N������
	//�������ӵĿ��������N��ѭ�������ǿ��ܻ�õĺ�������ǧ��ʱ�����ڡ������ٽ����ں̵ܶĲ����������������ĺô��Ǵ��ġ� 
	//MSDN��˵�����Ƕ��ڶѹ�����ʹ����N=4000����ת����Ȼ��This gives great performance and scalability in almost all worst-case scenarios.�� �ɼ����Ǻ����õ�:-)
	::InitializeCriticalSectionAndSpinCount(&m_cs, 4000);
}
CriticalSectionLock::~CriticalSectionLock(){
	::DeleteCriticalSection(&m_cs);
}
void CriticalSectionLock::lock(){
	::EnterCriticalSection(&m_cs);
}
void CriticalSectionLock::unlock(){
	::LeaveCriticalSection(&m_cs);
}
bool CriticalSectionLock::canCrossProcess() const{
	return false;
}
Lock::Type CriticalSectionLock::getType() const{
	return CRITICAL_SECTION;
}


