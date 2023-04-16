# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 助教老师们遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
LOGOUT ?= 0
CFLAGS = -std=gnu2x -g -DLOGOUT=$(LOGOUT) -I./include
CXXFLAGS = -std=gnu++20 -g -DLOGOUT=$(LOGOUT) -I./include

# 编译目标：src目录下的所有.c 和 .cc文件
CFILES = $(shell find ./ -name "*.c")
CXXFILES = $(shell find ./ -name "*.cc")
OBJS += $(CFILES:.c=.o)
OBJS += $(CXXFILES:.cc=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)
HEADER += $(shell find ./include -name "*.hh")
HEADER += $(shell find ./include -name "*.h")
BINARY = cc

$(BINARY): syntax $(filter-out $(LFO),$(OBJS)) $(HEADER)
	$(CXX) -o $(BINARY) $(filter-out $(LFO),$(OBJS)) -lfl -ly -lfmt

syntax: lexical syntax-c
	$(CC) -c -g $(YFC) -o $(YFO) $(CFLAGS)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
CMM ?= opt/test3
ARGS = ../Test/$(CMM).cmm
EXEC_CL = $(BINARY) $(ARGS)
gdb: $(BINARY)
	gdb -s $(BINARY) --args $(EXEC_CL)
test: $(BINARY)
	$(EXEC_CL)
clean:
	rm -f ./$(BINARY) ./lex.yy.c ./syntax.tab.c ./syntax.tab.h ./syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
