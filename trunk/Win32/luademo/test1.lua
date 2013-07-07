print("Hello lua");

local temp = {} 
temp[1]=9 
temp["test1 key"]="test1 value" 
temp[2]="test 2" 
temp["test 99"]=99 
 
for i,n in pairs(temp) 
do  
print(i,n) 
end 

local c = CMyClass()
-- 成员方法的调用
--obj:method(a1, a2, …) -- 等价于
--obj.method(obj, a1, a2, ...)
c:Greet();
c.Greet(c);
CMyClass.staticGreet(CMyClass,1);
CMyClass:staticGreet(2);