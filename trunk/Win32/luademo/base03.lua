-- �����﷨

-- 1����ֵ
-- lua���ԶԶ������ͬʱ��ֵ����������ֵ�ö��ŷָ�����ֵ����ұߵ�ֵ�����θ�����ߵı�����
-- ������������ֵ��ʱ������ı����ḳֵnil��
-- ��������������ֵʱ�������ֵ����ԣ�
-- ����
-- x,y = y,x -- ����x ��y ��ֵ
-- a,b,c = 0,1 -- a=0,b=1,c=nil
-- a,b = 3,2,1 -- a=3,b=2

-- 2���ֲ�����
-- luaʹ��local�����ֲ�����������������͸�c++���ƣ�������û��{}��ȡ����֮����do..end�����
-- ��Ҫ����һ���ֲ�����������Խ���������do..end���У�
-- lua��ʹ�þֲ������������ô���
--�� ����������ͻ
--�� ���ʾֲ��������ٶȱ�ȫ�ֱ�����
-- ����
-- do
-- local a = 2
-- b = 4
-- print (a,b) -- 2 4
-- end
-- print (a,b) -- nil 4

-- 3��if���
-- if conditions then
-- then-part
-- end;

-- if conditions then
-- then-part
-- else
-- else-part
-- end;

-- if conditions then
-- then-part
-- elseif conditions then
-- elseif-part
-- .. --->���elseif
-- else
-- else-part
-- end;

-- 4��while���
-- while condition do
-- statements;
-- end;

-- 5��repeat-until
-- repeat
-- statements;
-- until conditions;
-- ������ܱȽ���������ʵ����c++��do..while��䣬������statementsֱ��conditions Ϊ�档

-- 6����ֵforѭ��
-- for var=exp1,exp2,exp3 do
-- loop-part
-- end
-- for����exp3��Ϊstep��exp1����ʼֵ����exp2����ֵֹ����ִ��loop-part������exp3����ʡ�ԣ�
-- Ĭ��step=1.

-- 7������forѭ��
-- ������
-- for k in pairs(t) do print(k) end -- ������t�е�����Ԫ��
-- for i,v in ipairs(a) do print(v) end -- ������a��iΪ��������v Ϊ��ֵ��

-- 8��break��return
-- ����﷨��c++���ƣ�break�������˳�������䣬return �������˳������ġ�
-- ע����lua�У�break��return ������һ����ý������ֳ��֣������ĺ�߳��ֵı�����end��else��
-- until����������Ҫ��ĳ�����λ��ʹ�����ǣ�������ʽ��ʹ��do..end����������ʵ����Ҫ�Ĺ��ܡ�����
-- do return end;
-- �����´�ӡ 1245
print("1")
if true then
print("2")
else
print("3")
end

print("4")
repeat
do
break 
end
until true
print("5")
do
return 
end
print("6")