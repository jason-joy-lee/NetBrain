#--------------------------------------------------------------------------
# 本文演示如何使用jmake编译工具集来编译C++文件
# 1、定义环境变量JMAKE_PATH，指向存放jmake的路径
# 2、在下面填上自己要编译的主目标、源文件列表、头文件列表等
# 3、引入需要的辅助编译文件
# 4、完成。gmake:编译主目标，gmake clean:删除主目标相关文件
#
# 如果想加入其他编译标签，可以在后面自行定义。
#
# 2008/7/25, sunjun, 修改：集中用户修改区域，融合clean标签到compiler.mk
#--------------------------------------------------------------------------
TGT  = testAll
SRCS = testAll.cpp

INCS := -I../
LIBS := -L../util -lscheduler -L../scheduler -lCsvHelper


MY_CPPFLAGS=
MY_CXFLAGS = 
MY_LDFLAGS =

#--------------------------------------------------------------------------
# 这里请勿修改
#--------------------------------------------------------------------------
include $(JMAKE_PATH)/compiler.mk
export TGT SRCS INCS LIBS MY_CPPFLAGS MY_CXFLAGS MY_LDFLAGS

#--------------------------------------------------------------------------
# 在这里加入所需要的辅助编译文件、编译标签等
#--------------------------------------------------------------------------
#include $(JMAKE_PATH)/ace.mk
#include $(JMAKE_PATH)/lua.mk
#include $(JMAKE_PATH)/dbms.mk
#include $(JMAKE_PATH)/huge.mk

#--------------------------------------------------------------------------
# 测试
#--------------------------------------------------------------------------
MY_LIB = $(LIBS) -L$(dir $(TGT)) -l$(patsubst lib%.so,%,$(notdir $(TGT))) \
#	-lreadline -lhistory -lcurses
test:
	@$(MAKE) -f $(JMAKE_PATH)/compiler.mk --no-print-directory \
		TGT=test SRCS="test.cpp" INCS="$(INCS)" LIBS="$(MY_LIB)"

#--------------------------------------------------------------------------
# 清除所有
#--------------------------------------------------------------------------
cleanall:
	@$(MAKE) --no-print-directory clean
	rm -f test test.o

#--------------------------------------------------------------------------
# 安装
#--------------------------------------------------------------------------
install:$(TGT)
	@echo "installing..."	
	cp -f $(TGT) $(NET_BRAIN_ROOT)/test
	cp -f test_data_1.csv $(NET_BRAIN_ROOT)/test
	cp -f test_data_2.csv $(NET_BRAIN_ROOT)/test
	cp -f test_data_empty.csv $(NET_BRAIN_ROOT)/test
	@echo "install complete"

#--------------------------------------------------------------------------
# 更新-安装除配置、脚本以外的目标文件、头文件、库文件等
#--------------------------------------------------------------------------
update:$(TGT)
	@echo "updating..."
	cp -f $(TGT) $(NET_BRAIN_ROOT)/test
	@echo "update complete"

#--------------------------------------------------------------------------
# 卸载
#--------------------------------------------------------------------------
uninstall:
	@echo "uninstalling..."
	rm -f $(NET_BRAIN_ROOT)/test/$(TGT)
	@echo "uninstall complete"
