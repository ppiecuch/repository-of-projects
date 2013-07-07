-- 表达式

-- 1、算数运算符
-- 跟c++一样，加减乘除啥的。

-- 2、关系运算符
-- 跟c++类似，就是!=换为~=。

-- 3、逻辑运算符
-- 这个可比较特别哦，要多加注意，lua的逻辑运算符有not、and、or；
-- lua中只有false和nil 为假，其它都为真；
-- not a： a为false，值为true，a为true，值为false；
-- a and b： 如果a为false，则取a值，否则取b值；
-- a or b： 如果a为true，则取a值，否则取b值；
-- c++中的三目运算符a?b:c 在lua中的实现：(a and b) or c。

-- 4、连接运算符
-- .. 这个前边介绍过，就是连接两个字符串

-- 5、优先级
-- 类似c++，按老套路用就行

-- 6、表的构造
-- 这是lua的重头戏，它的构造函数是{}。
-- table = {} -- 创建一个空表
-- 表里边可以存放任意类型的成员，就把它当做一个c++的类即可，以下聚几个例子
-- test = {“a”,”b”,”c”,”d”,”e”,”f”,”g”}
-- print (test[4]) -- d // 这里注意，lua的下标操作是从1开始计算的
-- test = {x=1,y=2,z=3}
-- print (test.x,test.y,test.z) -- 1 2 3
-- test[1] = "a"
-- test1 = {"b","c"}
-- test1.a = test
-- print (test[1],test.x,test.y,test.z) -- a 1 2 3
-- print (test1.a[1],test1.a.x,test1[1]) -- a 1 b