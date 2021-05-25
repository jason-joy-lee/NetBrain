SUBDIRS=util scheduler test

INCS := -I$(NET_BRAIN_ROOT)/include -I.
LIBS := -L$(NET_BRAIN_ROOT)/lib -lscheduler -lCsvHelper


default:
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE}); done)

install:
	mkdir -p $(NET_BRAIN_ROOT)/include/scheduler
	mkdir -p $(NET_BRAIN_ROOT)/include/util	
	mkdir -p $(NET_BRAIN_ROOT)/bin
	mkdir -p $(NET_BRAIN_ROOT)/lib
	mkdir -p $(NET_BRAIN_ROOT)/test
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE} install); done)
	
	#mkdir -p ${NET_BRAIN_ROOT}/doc
	#cp -f ./doc $(NET_BRAIN_ROOT)/doc

update:
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE} update); done)

%:
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE} $@); done)

#--------------------------------------------------------------------------
# 测试
#--------------------------------------------------------------------------
test:
	cd test
	@$(MAKE)
	cd ..
