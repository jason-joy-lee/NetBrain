#--------------------------------------------------------------------------
#                                 ����
# compiler.mk��һ���򵥶������Ļ������뻷���ļ�����������������������
# ����Ա�����˽��������ñ�дmakefile������ɳ���ı��룬�����Ŀ���������
# ��������Ϥ�������ĳ���Ա��ַ��ӱ����������ơ���С�����ã���������ƽ̨
# �ͱ������������չ���Һܸ����������һ����������������Ҫ��������ơ�
# 
# �����������޸ġ��������ļ����߱��ļ������ⲿ�֣�����������������������
# ���ߵ����࣬лл��
#
# ���ߣ����
# ���ڣ�2008/6/20
# ���ʣ�j_sun@126.com
#--------------------------------------------------------------------------
# compiler.mk -- Compiler & linker environment
# DSC -- ���ļ��漰�����·�������ã�������ȷ˵�������һѡ��ΪĬ��ѡ���
#        1 - compiler: CX=[gcc|xlC|...], default by OS
#        2 - linker: CX
#        3 - compiler options: CXFLAGS
#        4 - linker options: LDFLAGS
#        5 - definitions: CPPFLAGS
#        6 - build mode: BLD=[DEBUG|RELEASE]
#        7 - thread library: TL=[ST|MT|...], single|multi|user-defined
#        8 - include files: INCS
#        9 - library files: LIBS
#       10 - source files: SRCS
#       11 - object files: OBJS
#       12 - target files: TGT=[*|*.so|*.a], exe-file|so-file|a-file
#       13 - user-defined: MY_CPPFLAGS/MY_CXFLAGS/MY_LDFLAGS
# IN  -- requisite: SRCS TGT
#     -- optional: CX TL BLD INCS LIBS MY_CPPFLAGS MY_CXFLAGS MY_LDFLAGS
#     -- risky: CXFLAGS LDFLAGS CPPFLAGS OBJS OS
# OUT -- every macro
#
# USG -- $(MAKE) {-f compiler.mk TGT= SRCS=} [CX= TL= BLD= INCS= LIBS= MY_CPPFLAGS= MY_CXFLAGS= MY_LDFLAGS=]
#      or
#        export {TGT SRCS} [CX TL BLD INCS LIBS MY_CPPFLAGS MY_CXFLAGS MY_LDFLAGS]
#        $(MAKE) -f compiler.mk
#
# History:
# 1: 2008/6/13 SUN JUN �������ļ�
# 2: 2008/6/20 SUN JUN ȷ�����ļ��÷�ΪUSG��ʾ���÷�
#    ���Ҫ֧��һ���µ�OS����CX��ֻ����д��OS�б�,CX�б�����Ӧ�ġ�OS-CX����ģ�顱����
#--------------------------------------------------------------------------
override SHELL:=/bin/sh
.SUFFIXES:
.SUFFIXES:.cpp .o
.PHONY: all clean install uninstall

#--------------------------------------------------------------------------
# ���屻���ļ�֧�ֵı������б�����ϵͳ�б�����ϵͳ-Ĭ�ϱ�����ӳ���
#--------------------------------------------------------------------------
override VA_LST  := xlc xlc_r xlC xlC_r
override GNU_LST := gcc g++
override ACC_LST := aCC

override CX_LST := $(VA_LST) $(GNU_LST) $(ACC_LST)
override OS_LST := AIX Linux HP-UX

override OS_CX_MAP := AIX.xlC Linux.g++ HP-UX.aCC
#--------------------------------------------------------------------------
# �Զ�������ϵͳ�����������������ȱ�����ǿ�ҽ���ʹ��Ӳ���ܹ����׵ı�����
#--------------------------------------------------------------------------
# detecting OS
override OS := $(shell uname)

# detecting compiler & linker
CX ?= $(patsubst .%,%,$(suffix $(filter $(OS).%,$(OS_CX_MAP))))

# build mode
BLD ?= DEBUG

# thread library
TL ?= ST

# deduce object-list from source-list
OBJS = $(SRCS:.cpp=.o)
#--------------------------------------------------------------------------
# ȷ���ؼ���������ȷ���壬�ؼ������У�OS��CX��BLD��SRCS��OBJS��TGT��TL
#--------------------------------------------------------------------------
ifndef OS
  $(error OS not defined)
endif
ifeq (, $(findstring $(OS), $(OS_LST)))
  $(error OS "$(OS)" not support)
endif

ifndef CX
  $(error c/c++ compiler & linker CX not defined)
endif
ifeq (, $(findstring $(CX), $(CX_LST)))
  $(error CX "$(CX)" not support)
endif

ifndef BLD
  $(error build-mode BLD not defined)
endif
ifeq (, $(findstring $(BLD), DEBUG RELEASE))
  $(error invalid build-mode $(BLD))
endif

ifndef SRCS
  $(error source-file-list SRCS not defined)
endif

ifndef OBJS
  $(error object-file-list OBJS not defined)
endif

ifndef TGT
  $(error target-file TGT not defined)
endif

ifndef TL
  $(error thread-library TL not defined)
endif

#--------------------------------------------------------------------------
# OS-CX=(AIX, VA)
#--------------------------------------------------------------------------
override AIX_VA_LST := $(addprefix AIX., $(VA_LST))
ifneq (, $(findstring $(OS).$(CX), $(AIX_VA_LST)))
  # common flags
  CXFLAGS  += -bhalt:5 -bdynamic -qlonglong
  LDFLAGS  += -bhalt:5 -lperfstat
  CPPFLAGS += -DAIX

  # build mode
  ifeq ($(BLD), DEBUG)
    CXFLAGS  += -g
    CPPFLAGS += #-D_DEBUG
  else
    CXFLAGS  += -O2
    CPPFLAGS += -DNDEBUG
  endif
  
  # build target
  SUFF := $(suffix $(TGT))
  ifeq ($(SUFF), .so)
    LDFLAGS += -G
  else
    ifeq ($(SUFF), .a)
      $(error static target not support yet!)
    else
      LDFLAGS += -brtl
    endif
  endif
  
  # thread library
  ifeq ($(TL), MT)
    CX = xlC_r
  else
    ifneq ($(TL), ST)
      LIBS += $(TL)
    endif
  endif
endif

#--------------------------------------------------------------------------
# OS-CX=(Linux, GNU)
#--------------------------------------------------------------------------
override LINUX_GNU_LST := $(addprefix Linux., $(GNU_LST))
ifneq (, $(findstring $(OS).$(CX), $(LINUX_GNU_LST)))
  # common flags
  CXFLAGS  += -Wall -Winline -finline-functions -fpic
  LDFLAGS  +=
  CPPFLAGS += -DLINUX
  
  # build mode
  ifeq ($(BLD), DEBUG)
    CXFLAGS  += -g
    CPPFLAGS += -D_DEBUG
  else
    CXFLAGS  += -O2
    CPPFLAGS += -DNDEBUG
  endif
  
  # build target
  SUFF := $(suffix $(TGT))
  ifeq ($(SUFF), .so)
    LDFLAGS += -shared
  else
    ifeq ($(SUFF), .a)
      $(error static target not support yet!)
    else
      LDFLAGS +=
    endif
  endif
  
  # thread library
  ifeq ($(TL), MT)
    $(error I don't know gnu's default MT:))
  else
    # �û��Զ����߳̿�
    ifneq ($(TL), ST)
      LIBS += $(TL)
    endif
  endif
endif

#--------------------------------------------------------------------------
# OS-CX=(HP-UX, ACC)
# +z: puts the position-independent code (PIC) into the object file
# -AA: use STL2.0
# +DAportable: To generate code compatible across PA-RISC 1.1 and
#      2.0 workstations and servers, use the +DAportable option.
#--------------------------------------------------------------------------
override HPUX_ACC_LST := $(addprefix HP-UX., $(ACC_LST))
ifneq (, $(findstring $(OS).$(CX), $(HPUX_ACC_LST)))
  # common flags
  CXFLAGS  += +z -AA -mt -Wl,-E,+s +W2191 +W4285 +DAportable
  LDFLAGS  += -AA -mt +Z -Wl,-E,+s
  CPPFLAGS += -DHPUX
  
  # build mode
  ifeq ($(BLD), DEBUG)
    CXFLAGS  += -g
    CPPFLAGS += -D_DEBUG
  else
    CXFLAGS  += -O
    CPPFLAGS += -DNDEBUG
  endif
  
  # build target
  SUFF := $(suffix $(TGT))
  ifeq ($(SUFF), .so)
    TGT := $(subst .so,.sl,$(TGT))
    LDFLAGS += -b
  else
    ifeq ($(SUFF), .sl)
      LDFLAGS += -b
    endif
    ifeq ($(SUFF), .a)
      $(error static target not support yet!)
    else
      LDFLAGS +=
    endif
  endif
  
  # thread library
  ifeq ($(TL), MT)
    $(error I don't know gnu's default MT:))
  else
    # �û��Զ����߳̿�
    ifneq ($(TL), ST)
      LIBS += $(TL)
    endif
  endif
endif

#--------------------------------------------------------------------------
# �Ҳ�������������ϵͳʱʹ��Ĭ�ϲ�����������Ĭ��Ϊgcc/g++
#--------------------------------------------------------------------------
ifndef CXFLAGS
  # common flags
  CXFLAGS  += -Wall
  LDFLAGS  +=
  CPPFLAGS +=
  
  # build mode
  ifeq ($(BLD), DEBUG)
    CXFLAGS  += -g
    CPPFLAGS += -D_DEBUG
  else
    CXFLAGS  += -O2
    CPPFLAGS += -DNDEBUG
  endif
  
  # build target
  SUFF := $(suffix $(TGT))
  ifeq ($(SUFF), .so)
    LDFLAGS += -shared
  else
    ifeq ($(SUFF), .a)
      $(error static target not support yet!)
    else
      LDFLAGS +=
    endif
  endif
  
  # thread library
  ifeq ($(TL), MT)
    $(error I don't know gnu's default MT:))
  else
    # �û��Զ����߳̿�
    ifneq ($(TL), ST)
      LIBS += $(TL)
    endif
  endif
endif

#--------------------------------------------------------------------------
# ͨ�ñ�������ӹ���
#--------------------------------------------------------------------------
$(TGT):$(OBJS)
	$(CX) -o $@ $(LDFLAGS) $(MY_LDFLAGS) $(OBJS) $(LIBS)

%.o:%.cpp
	$(CX) $(CPPFLAGS) $(MY_CPPFLAGS) $(CXFLAGS) $(MY_CXFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f core $(TGT) $(patsubst %.cpp, %.o, $(SRCS))

default:
	@$(MAKE) --no-print-directory

%:
	@$(MAKE) --no-print-directory $@

#--------------------------------------------------------------------------
# check_compiler
#--------------------------------------------------------------------------
check_compiler:
	@echo dumping COMPILER-ENV...
	@echo "\tOS=$(OS)"

	@echo "\tCX=$(CX)"
	@echo "\tTL=$(TL)"
	@echo "\tBLD=$(BLD)"
	@echo "\tCXFLAGS=$(CXFLAGS)"
	@echo "\tLDFLAGS=$(LDFLAGS)"
	@echo "\tCPPFLAGS=$(CPPFLAGS)"

	@echo "\tINCS=$(INCS)"
	@echo "\tLIBS=$(LIBS)"
	@echo "\tTGT=$(TGT)"
	@echo "\tSRCS=$(SRCS)"
	@echo "\tOBJS=$(OBJS)"

	@echo "\tMY_CPPFLAGS=$(MY_CPPFLAGS)"
	@echo "\tMY_CXFLAGS=$(MY_CXFLAGS)"
	@echo "\tMY_LDFLAGS=$(MY_LDFLAGS)"
