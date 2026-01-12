# 项目名称
TARGET = vuln_pingtai

# 编译器
CC = gcc

# 编译选项：启用所有警告，包含头文件路径
CFLAGS = -Wall -I./yuanma -I./yuanma/hexin -I./yuanma/gongneng

# 链接器选项
LDFLAGS = 

# 源文件列表
SRCS = yuanma/main.c \
       yuanma/hexin/xianshi.c \
       yuanma/gongneng/web_guanli.c \
       yuanma/gongneng/pwn_guanli.c \
       yuanma/gongneng/nx_guanli.c

# 目标文件列表（由源文件自动推导）
OBJS = $(SRCS:.c=.o)

# 默认目标：构建可执行文件
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# 编译规则：将.c文件编译为.o文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标声明
.PHONY: all clean