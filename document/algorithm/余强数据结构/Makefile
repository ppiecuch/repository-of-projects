# ------------------------------------------------------------------------------------------
#
# 文  件： Makefile for ALL
# 作  者： 余    强
# 日  期： 2009-12-09
# 版  权： Copyleft (c) GPL
# E-mail: yuembed@126.com
# 博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562
#
# ------------------------------------------------------------------------------------------
	
# pick up common rules
include $(YDS_MAKE)/common.mak

YDS_TARGETS := all
YDS_EXPORTS :=
YDS_IMPORTS := AVL树 八皇后 二叉查找树 二叉树 红黑树 链表 链式队列 链式栈

include $(YDS_MAKE)/default.mak
# 编译指定的一个模块
ifneq "$(MODULE)" ""
$(YDS_TARGETS):edir_check
	$(MAKE) -C $(MODULE)
else
# 编译整个开发树
$(YDS_TARGETS):edir_check $(IMPORTS_LIBS)
endif

# 清除指定的模块或整个开发树
ifneq "$(MODULE)" ""
# 清除指定的一个模块
clean:
	-@$(MAKE) -C $(wildcard $(YDS_ROOT)/$(MODULE)) $@
else
# 清除整个开发树
clean:$(IMPORTS_CLEAN)
endif

help:helpme
# ----------------end of Makefile-------------------------------------
# 有水的地方就有余 http://hi.baidu.com/20065562
#
