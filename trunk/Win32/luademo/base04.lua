-- ����

-- 1�������Ķ���
-- function name (param)
-- return value
-- end;

-- 2���෵��ֵ
-- lua�������ض��ֵ�ģ���������ж������ֵ����Ա����ĸ�ֵ����3.1��ֵ��߹涨�ķ�ʽ��ֵ��
-- ����
-- function foo0 () end
-- function foo1 () return 'a' end
-- function foo2 () return 'a','b' end
-- x,y = foo2() -- x='a', y='b'
-- x = foo2() -- x='a', ����ֵ'b'����
-- x,y,z = 10,foo2() -- x=10, y='a', z='b'��
-- x,y = foo0() -- x=nil, y=nil
-- x,y = foo1() -- x='a', y=nil
-- x,y,z = foo2() -- x='a', y='b', z=nil

function multi() 
	return 1,2,3,4,5 
end

local x,y,z,w,u=multi();
print(tostring(x)..tostring(y)..tostring(z)..tostring(w))


-- 3���ɱ����
-- lua�������Խ��ܿɱ���Ŀ�Ĳ�������C ���������ں��������б���ʹ�����㣨...����ʾ�����пɱ�Ĳ�
-- ����lua�������Ĳ�������һ����arg�ı��У����˲������⣬arg���л���һ����n ��ʾ�����ĸ���
-- ����
-- printResult = ""
-- function test(...)
-- print ("haveing " .. arg.n .. " param")
-- for i,v in ipairs(arg) do
-- printResult = printResult .. tostring(v)
-- end;
-- print (printResult)
-- end
-- test('a','b','c','d','e')
-- �����
-- haveing 5 param
-- abcde

function uncertain(...)
	print("contain " .. arg.n .. " params")
	local tmp=""
	for i,v in ipairs(arg) do
		tmp = tmp .. tostring(v)
	end
	print(tmp)
end
uncertain(1,2,3,4,5)

-- 4����������
-- lua�ĵ��ù����У����������ΰ�ʵ�δ��ݸ���Ӧ���βεġ��������ʱ�����Ǻ��Ѽ��������ǰ��˳��
-- ���ʱ��Ϳ���ʹ����������������˼�壬����Ϊÿ��������һ�����֣����õ�ʱ��ֻ��ҪΪ�������
-- �Ĳ�����ֵ���ɡ�
-- ����
-- function rename (arg)
-- return os.rename(arg.old, arg.new)
-- end
-- rename(old="temp.lua", new="temp1.lua") �C���õ�ʱ��Ϊ����������ֵ

-- 5���հ�
-- ������ȷһ������ʷ����硣��һ�������ڲ�Ƕ����һ����������ʱ���ڲ��ĺ�������Է����ⲿ�ĺ�
-- ���ľֲ������������������ǳ����ʷ�����
-- ��ν���հ������ٷ�����ָ����һ��ӵ���������Ͱ�����Щ�����Ļ����ı��ʽ��ͨ����һ����������
-- �����Щ����Ҳ�Ǹñ��ʽ��һ���֡�
-- �ǲ����е��Ϳ�أ��ٸ����ӣ�
-- function newCounter()
-- local i = 0
-- return function() -- ��������
-- i = i + 1
-- return i
-- end
-- end
-- c1 = newCounter()
-- print(c1()) --> 1
-- print(c1()) --> 2
-- c2 = newCounter()
-- print(c2()) --> 1
-- print(c1()) --> 3
-- print(c2()) --> 2
-- ��δ����������ص㣬1.������������Ƕ���ں���newCounter�ڣ�2. ����newCounter�ѡ�����������
-- �����ˡ�
-- ��ִ����c1 = newCounter()����ʵc1ָ����ǡ�������������������ִ��c1��ʱ�򣬾Ϳ��Բ�����
-- ��newCounter�ľֲ�����i�����i���ǳ�֮Ϊ�ⲿ�ľֲ�������external local variable������
-- upvalue�������ٴ�����һ������c2= newCounter()��c1��c2��ʵ�ǽ�����ͬһ�������ϵģ�����
-- ���ǵľֲ�����iȷ��������ͬ��ʵ���������Ǵ����������հ�������˵��������newCounter���ڲ���
-- �����������ġ�������������������newCounter��ı���c1���õ�ʱ�򣬾ʹ�����һ���հ���
-- �հ���Ӧ�ó�����
-- �� ���������ڵı�����ȫ�����ʼ������Ϊ��������a��iֻ�к���b ���ܷ��ʣ����޷�ͨ����
-- ��;�����ʵ�����˱�����i�İ�ȫ�ԡ�
-- �� ���ڴ���ά��һ����������Ȼ��ǰ�������ڱհ�������a��i��һֱ�������ڴ��У����ÿ��ִ
-- ��c()�������i�Լ�1��

function closure()
	local i = 0
	return function()
		i = i + 1
		return i
	end
end
local ac=closure()
local bc=closure()
print(ac())
print(ac())

print(bc())
print(bc())
print(bc())

-- 6����ȫ�ֺ���
-- lua�к���������Ϊȫ�ֱ���Ҳ������Ϊ�ֲ�����
-- 1. ��ͺ�������һ��
-- Lib = {}
-- Lib.foo = function (x,y) return x + y end
-- Lib.goo = function (x,y) return x - y end
-- 2. ʹ�ñ��캯��
-- Lib = {
-- foo = function (x,y) return x + y end,
-- goo = function (x,y) return x - y end
-- }
-- 3. lua�ṩ����һ���﷨��ʽ
-- Lib = {}
-- function Lib.foo (x,y)
-- return x + y
-- end
-- function Lib.goo (x,y)
-- return x - y
-- end
-- �����ǽ�����������һ���ֲ�������ʱ�����ǵõ�һ���ֲ�������Ҳ����˵�ֲ�������ֲ�����һ����һ
-- ����Χ����Ч������ֲ����������ַ�ʽ��
-- local f = function (...)
-- ...
-- end
-- local function f (...)
-- ...
-- end
-- �����´��룺
-- local fact = function (n)
-- if n == 0 then
-- return 1
-- else
-- return n*fact(n-1) -- �˴�lua����ʶ��fact����Ϊ�Ǿֲ�������lua����ʶ��
-- end
-- end
-- �޸�Ϊ�����������£�
-- local fact
-- fact = function (n)
-- if n == 0 then
-- return 1
-- else
-- return n*fact(n-1)
-- end
-- end