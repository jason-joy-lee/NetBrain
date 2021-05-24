export NET_BRAIN_ROOT=../src/build
export JMAKE_PATH=../src/

default:
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE}); done)

install:
	mkdir -p ${NET_BRAIN_ROOT}/include/scheduler
	mkdir -p ${NET_BRAIN_ROOT}/include/util	
	mkdir -p ${NET_BRAIN_ROOT}/bin
	mkdir -p ${NET_BRAIN_ROOT}/test
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE} install); done)
	
	#mkdir -p ${NET_BRAIN_ROOT}/doc
	#cp -f ./doc $(NET_BRAIN_ROOT)/doc

update:
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE} update); done)
	
%:
	@(for i in ${SUBDIRS}; do (cd $$i; ${MAKE} $@); done)

