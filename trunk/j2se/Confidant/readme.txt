��Ŀ����Confidant
������ʵ�ְ�����¼�������ű���ִ��
���ܣ�
1��֧����ꡢ���̰�����¼
2��֧�ֽű����ɼ��ű�����
3��֧�ֶ��ʱ�����м�ʱ
4������¼�����ڵ��������
5��ֻ��¼��������¼�������ֻ��¼���click�¼�������¼�϶��¼�
6��ֻ��¼���̰����İ����¼�
7������ű���ʱ������Ϊ��λ���ڴ��������Ժ���Ϊ��λ(�ڶ���д��ʱ����ת��)
8����ʱ��֧��ֹͣ������

����ɣ�
1��TaskThread�Ĺ��ܲ��ԣ��Ƿ���ڿ�ʱwait(��)
2�����ӻ�ģ�����Ʊ���
3�������ڲ���¼�ļ���ʵ��
4��Recorder�У���һ��������delay��0�ػ��ǰ���ʽ���м��㣿
6������ĳЩ�����������������ν����
7��XMLFormator��������������(dom4j)

���ȣ��������->ϵͳ���->��ܹ�->����ʵ��

beta01:
1��֧����ꡢ���̰�����¼
2��֧�ֽű����ɼ��ű�����
3��ֻ֧�ֵ���ʱ��
4��֧�ּ�¼ģʽ��ģ��ģʽ
5���Ǵ��ڻ�
6��������������ļ���Ϊscript.txt

beta02:
1��������ý���ļ���ΪXML��ʽ
2��������ģ��ģʽ���˳�ͣ�͵����� 
3������˱���XML�ļ���ʽ������ 

beta02-v2:
1��������script.xml�����е�BUG
2�������˽���ģ��ģʽ����ڲ˵�ģʽ�ٽ���ʱ���쳣��BUG
3������Ҽ�����֧�֣�ʹ�ø����꣩

beta03:
1��֧��������ɫ�ж�
2��֧�ֶ�ʱ(ָ��һ������ʱ��ִ��)��ѭ����֧�ֶ�����ʱ�䣩����ģʽ
7��֧�־�������Ϣ���͹���

3��֧�ֹػ�
4��֧�ּ��̰�ť��������������Ҽ�ģ��
5��֧��ִ��ָ�����Զ�ʱ��ѭ�����ߵľ����ᵼ��һЩָ��Ĵ��ң�
6��¼��ֻ֧��loopģʽ������ֻ֧�ּ��̡�������Ҽ�����ɫ��¼�ƣ���������ֻ��ͨ���ֹ�����ʵ��
8��֧��Alt,Shift,Ctrl���Ӽ�
<?xml version="1.0" encoding="UTF-8"?>
<System>
	<commander mode="loop" time="*/30">
		<command type="key" value="51" delay="1000" ctrl="true" alt="true" shift="true"/>
		<command type="leftMouse" x="500" y="500" delay="1000" ctrl="true"/>
		<command type="call" x="500" y="500" ref="id02" alt="true"/>
		<command type="checkColor" x="500" y="500" value="0xFFFFFF" delay="1000" trueRef="id00" falseRef="id01"/>
		<command type="rightMouse" x="500" y="500" delay="1000" shift="true"/>
	</commander>
	<commander mode="timer" time="20:30">
		<command type="shutDown" delay="1000"/>
	</commander>
	<commander id="id00" mode="bunch">
		<command type="key" value="51" delay="1000"/>
		<command type="leftMouse" x="500" y="500" delay="1000"/>
		<command type="rightMouse" x="500" y="500" delay="1000"/>
		<command type="sendUDPMsg" value="fgagdafddsaf" delay="1000" ip="127.0.0.1"/>
	</commander>
</System>