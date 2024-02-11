VER := release ## 调试选项
OPTM := elide ## 构造器优化选项
STD := c++11
CPP := g++
CXXFLAGS := -Wall -O0 -std=$(STD) $(OPT)

## 头文件
HEADER := include \
		  lib
## 可执行程序
EXE_TEST := bin/test
EXE_RECOMMENDER := bin/offline1
EXE_SEARCHER := bin/offline2
## 源程序
SRC_TEST := $(wildcard src/test/*.cpp)
SRC_RECOMMENDER := $(wildcard src/offline/module1/*.cpp)
SRC_SEARCHER := $(wildcard src/offline/module2/*.cpp)

## 数据文件
DATA := data/*.dat

## 编译参数确定
ifeq ($(VER), debug) ## ifeq后要有空格
	OPT+=-g
	OPT+=-D __DEBUG__
endif

ifeq ($(OPTM), no-elide)
	OPT+=-fno-elide-constructors
endif

## 链接头文件
I_flags := $(HEADER:%=-I%)
CXXFLAGS += $(I_flags) 

## 分别进行编译
$(EXE_TEST) : $(SRC_TEST)
	$(CPP) $^ -o $@ $(CXXFLAGS)

$(EXE_RECOMMENDER) : $(SRC_RECOMMENDER)
	$(CPP) $^ -o $@ $(CXXFLAGS)

$(EXE_SEARCHER) : $(SRC_SEARCHER)
	$(CPP) $^ -o $@ $(CXXFLAGS)

## 执行可执行文件
test : $(EXE_TEST)
	./$(EXE_TEST)

recommender : $(EXE_RECOMMENDER)
	./$(EXE_RECOMMENDER)

searcher : $(EXE_SEARCHER)
	./$(EXE_SEARCHER)

debug :
	@echo $(DATA)

clean :
	rm -f bin/* $(DATA)

.PHONY : debug clean test recommender searcher
