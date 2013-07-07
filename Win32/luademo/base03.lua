-- 基本语法

-- 1、赋值
-- lua可以对多个变量同时赋值，各变量和值用逗号分隔，赋值语句右边的值会依次赋给左边的变量；
-- 当变量个数比值多时，多出的变量会赋值nil；
-- 当变量个数少于值时，多余的值会忽略；
-- 例：
-- x,y = y,x -- 交换x 和y 的值
-- a,b,c = 0,1 -- a=0,b=1,c=nil
-- a,b = 3,2,1 -- a=3,b=2

-- 2、局部变量
-- lua使用local创建局部变量，它的作用域就跟c++类似，不过它没有{}，取而代之的是do..end，如果
-- 想要设置一个局部变量，则可以将它定义在do..end当中；
-- lua中使用局部变量有两个好处：
--· 避免命名冲突
--· 访问局部变量的速度比全局变量快
-- 例：
-- do
-- local a = 2
-- b = 4
-- print (a,b) -- 2 4
-- end
-- print (a,b) -- nil 4

-- 3、if语句
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
-- .. --->多个elseif
-- else
-- else-part
-- end;

-- 4、while语句
-- while condition do
-- statements;
-- end;

-- 5、repeat-until
-- repeat
-- statements;
-- until conditions;
-- 这个可能比较眼生，其实类似c++的do..while语句，即运行statements直到conditions 为真。

-- 6、数值for循环
-- for var=exp1,exp2,exp3 do
-- loop-part
-- end
-- for将用exp3作为step从exp1（初始值）到exp2（终止值），执行loop-part。其中exp3可以省略，
-- 默认step=1.

-- 7、范型for循环
-- 遍历表
-- for k in pairs(t) do print(k) end -- 遍历表t中的所有元素
-- for i,v in ipairs(a) do print(v) end -- 遍历表a，i为其索引，v 为其值。

-- 8、break和return
-- 这个语法与c++类似，break是用来退出控制语句，return 是用来退出函数的。
-- 注：在lua中，break和return 必须在一个块得结束部分出现，即它的后边出现的必须是end、else、
-- until，如果真的想要在某个语句位置使用它们，可以显式的使用do..end来包含它们实现想要的功能。例：
-- do return end;
-- 如以下打印 1245
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