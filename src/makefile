SUBDIRS=util scheduler test

INCS := -I$(NET_BRAIN_BUILD)/include -I.
LIBS := -L$(NET_BRAIN_BUILD)/lib -lscheduler -lCsvHelper


default:
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE}); done)

install:
	mkdir -p $(NET_BRAIN_BUILD)/include/scheduler
	mkdir -p $(NET_BRAIN_BUILD)/include/util	
	mkdir -p $(NET_BRAIN_BUILD)/bin
	mkdir -p $(NET_BRAIN_BUILD)/lib
	mkdir -p $(NET_BRAIN_BUILD)/test
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE} install); done)
	
	#mkdir -p ${NET_BRAIN_BUILD}/doc
	#cp -f ./doc $(NET_BRAIN_BUILD)/doc

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
