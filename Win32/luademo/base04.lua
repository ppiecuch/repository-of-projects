-- 函数

-- 1、函数的定义
-- function name (param)
-- return value
-- end;

-- 2、多返回值
-- lua是允许返回多个值的，如果函数有多个返回值，则对变量的赋值按照3.1赋值里边规定的方式赋值。
-- 例：
-- function foo0 () end
-- function foo1 () return 'a' end
-- function foo2 () return 'a','b' end
-- x,y = foo2() -- x='a', y='b'
-- x = foo2() -- x='a', 返回值'b'废弃
-- x,y,z = 10,foo2() -- x=10, y='a', z='b'、
-- x,y = foo0() -- x=nil, y=nil
-- x,y = foo1() -- x='a', y=nil
-- x,y,z = foo2() -- x='a', y='b', z=nil

function multi() 
	return 1,2,3,4,5 
end

local x,y,z,w,u=multi();
print(tostring(x)..tostring(y)..tostring(z)..tostring(w))


-- 3、可变参数
-- lua函数可以接受可变数目的参数，和C 语言类似在函数参数列表中使用三点（...）表示函数有可变的参
-- 数。lua将函数的参数放在一个叫arg的表中，除了参数以外，arg表中还有一个域n 表示参数的个数
-- 例：
-- printResult = ""
-- function test(...)
-- print ("haveing " .. arg.n .. " param")
-- for i,v in ipairs(arg) do
-- printResult = printResult .. tostring(v)
-- end;
-- print (printResult)
-- end
-- test('a','b','c','d','e')
-- 结果：
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

-- 4、命名参数
-- lua的调用过程中，参数是依次把实参传递给相应的形参的。但如果有时候我们很难记清参数的前后顺序，
-- 这个时候就可以使用命名参数。顾名思义，就是为每个参数起一个名字，调用的时候，只需要为这个名字
-- 的参数赋值即可。
-- 例：
-- function rename (arg)
-- return os.rename(arg.old, arg.new)
-- end
-- rename(old="temp.lua", new="temp1.lua") –调用的时候为命名参数赋值

-- 5、闭包
-- 首先明确一个概念，词法定界。当一个函数内部嵌套另一个函数定义时，内部的函数体可以访问外部的函
-- 数的局部变量，这种特征我们称作词法定界
-- 所谓“闭包”，官方解释指的是一个拥有许多变量和绑定了这些变量的环境的表达式（通常是一个函数），
-- 因而这些变量也是该表达式的一部分。
-- 是不是有点糊涂呢？举个例子：
-- function newCounter()
-- local i = 0
-- return function() -- 无名函数
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
-- 这段代码有两个特点，1.“匿名函数”嵌套在函数newCounter内，2. 函数newCounter把“匿名函数”
-- 返回了。
-- 在执行完c1 = newCounter()后，其实c1指向的是“匿名函数”，当我们执行c1的时候，就可以操作函
-- 数newCounter的局部变量i，这个i我们称之为外部的局部变量（external local variable）或者
-- upvalue。我们再次声明一个变量c2= newCounter()后，c1和c2其实是建立在同一个函数上的，但是
-- 他们的局部变量i确是两个不同的实例，即我们创建了两个闭包。就是说：当函数newCounter的内部函
-- 数（即上例的“匿名函数”）被函数newCounter外的变量c1引用的时候，就创建了一个闭包。
-- 闭包的应用场景：
-- · 保护函数内的变量安全。以最开始的例子为例，函数a中i只有函数b 才能访问，而无法通过其
-- 他途径访问到，因此保护了i的安全性。
-- · 在内存中维持一个变量。依然如前例，由于闭包，函数a中i的一直存在于内存中，因此每次执
-- 行c()，都会给i自加1。

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

-- 6、非全局函数
-- lua中函数可以作为全局变量也可以作为局部变量
-- 1. 表和函数放在一起
-- Lib = {}
-- Lib.foo = function (x,y) return x + y end
-- Lib.goo = function (x,y) return x - y end
-- 2. 使用表构造函数
-- Lib = {
-- foo = function (x,y) return x + y end,
-- goo = function (x,y) return x - y end
-- }
-- 3. lua提供的另一种语法方式
-- Lib = {}
-- function Lib.foo (x,y)
-- return x + y
-- end
-- function Lib.goo (x,y)
-- return x - y
-- end
-- 当我们将函数保存在一个局部变量内时，我们得到一个局部函数，也就是说局部函数像局部变量一样在一
-- 定范围内有效。定义局部函数的两种方式：
-- local f = function (...)
-- ...
-- end
-- local function f (...)
-- ...
-- end
-- 看如下代码：
-- local fact = function (n)
-- if n == 0 then
-- return 1
-- else
-- return n*fact(n-1) -- 此处lua不能识别fact，因为是局部函数，lua不能识别
-- end
-- end
-- 修改为先声明，如下：
-- local fact
-- fact = function (n)
-- if n == 0 then
-- return 1
-- else
-- return n*fact(n-1)
-- end
-- end