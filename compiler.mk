#--------------------------------------------------------------------------
#                                 声明
# compiler.mk：一个简单而又灵活的基础编译环境文件，它的易用性甚至可以让
# 程序员不必了解甚至不用编写makefile即可完成程序的编译，而它的可配置性又
# 可以让熟悉编译器的程序员充分发挥编译器的优势。它小巧易用，而且易于平台
# 和编译器方面的扩展。我很高兴完成了这一工作，不过它还需要更多的完善。
# 
# 您可以任意修改、发布本文件或者本文件的任意部分，但请您保留本声明，尊重
# 作者的辛苦，谢谢！
#
# 作者：孙军
# 日期：2008/6/20
# 电邮：j_sun@126.com
#--------------------------------------------------------------------------
# compiler.mk -- Compiler & linker environment
# DSC -- 本文件涉及到如下方面的设置（若无明确说明，则第一选项为默认选项）：
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
# 1: 2008/6/13 SUN JUN 创建本文件
# 2: 2008/6/20 SUN JUN 确定本文件用法为USG所示的用法
#    如果要支持一个新的OS或者CX，只需填写“OS列表,CX列表”和相应的“OS-CX处理模块”即可
#--------------------------------------------------------------------------
override SHELL:=/bin/sh
.SUFFIXES:
.SUFFIXES:.cpp .o
.PHONY: all clean install uninstall

#--------------------------------------------------------------------------
# 定义被本文件支持的编译器列表、操作系统列表、操作系统-默认编译器映射表
#--------------------------------------------------------------------------
override VA_LST  := xlc xlc_r xlC xlC_r
override GNU_LST := gcc g++
override ACC_LST := aCC

override CX_LST := $(VA_LST) $(GNU_LST) $(ACC_LST)
override OS_LST := AIX Linux HP-UX

override OS_CX_MAP := AIX.xlC Linux.g++ HP-UX.aCC
#--------------------------------------------------------------------------
# 自动检测操作系统、编译器、链接器等变量，强烈建议使用硬件架构配套的编译器
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
# 确保关键变量被正确定义，关键变量有：OS、CX、BLD、SRCS、OBJS、TGT、TL
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
    # 用户自定义线程库
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
    # 用户自定义线程库
    ifneq ($(TL), ST)
      LIBS += $(TL)
    endif
  endif
endif

#--------------------------------------------------------------------------
# 找不到符合条件的系统时使用默认参数，编译器默认为gcc/g++
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
    # 用户自定义线程库
    ifneq ($(TL), ST)
      LIBS += $(TL)
    endif
  endif
endif

#--------------------------------------------------------------------------
# 通用编译和链接规则
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
