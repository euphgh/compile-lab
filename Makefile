# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 助教老师们遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
LOGOUT ?= 1
CFLAGS = -std=gnu2x -g -DLOGOUT=$(LOGOUT) -I./include
CXXFLAGS = -std=gnu++20 -g -DLOGOUT=$(LOGOUT) -I./include

# 编译目标：src目录下的所有.c 和 .cc文件
BUILD_DIR = build
CXXFILES = $(shell find src -name "*.cc")
OBJS += $(CXXFILES:src/%.cc=$(BUILD_DIR)/%.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)
HEADER += $(shell find ./include -name "*.hh")
HEADER += $(shell find ./include -name "*.h")
BINARY = cc

$(BINARY): $(YFO) $(OBJS) $(HEADER)
	@mkdir -p $(BUILD_DIR)
	@echo + LD $(BINARY)
	@$(CXX) -o $(BINARY) $(OBJS) $(YFO) -lfl -ly -lfmt

$(OBJS): $(BUILD_DIR)/%.o:src/%.cc $(HEADER)
	@mkdir -p $(dir $@)
	@echo + CXX $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(YFO): $(LFC) $(YFC)
	@echo + CC $(YFC)
	@$(CC) -c -g $(YFC) -o $(YFO) $(CFLAGS)

$(LFC): $(LFILE)
	@echo + FLEX $(LFILE)
	@$(FLEX) -o $(LFC) $(LFILE)

$(YFC): $(YFILE)
	@echo + BISON $(YFILE)
	@$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test syntax syntax lexical
syntax: $(YFO)
syntax-c: $(YFC)
lexical: $(LFC)
NUM ?= 1
ARGS = ../Test/test$(NUM).cmm
EXEC_CL = ./$(BINARY) $(ARGS)
gdb: $(BINARY)
	gdb -s $(BINARY) --args $(EXEC_CL)
test: $(BINARY)
	$(EXEC_CL)
clean:
	rm -f ./$(BINARY) ./lex.yy.c ./syntax.tab.c ./syntax.tab.h ./syntax.output
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -rf ./$(BUILD_DIR)
