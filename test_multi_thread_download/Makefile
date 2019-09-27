
CXX = g++
CURL_INC = /usr/local
INCLUDE_PATH = $(CURL_INC)/include/
#LIB_PATH = ./lib/

CURL_LIBS = -lcurl
CFLAGS = -I$(INCLUDE_PATH) 
#CXXFLAGS = -ldl -lpthread -lz -lm -static $(CURL_LIBS) -L./lib   
CXXFLAGS = $(CURL_LIBS) -ldl -lpthread -lz -lm   


TARGET = multi_thread_download
DIRS = .
OBJS_FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
HEADERS = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.h))
OBJS = $(patsubst %.cpp, %.o, $(OBJS_FILES))
RM = rm -f

#$(wildcard *.c) #搜索当前目前下匹配*.c的所有文件并返回文件列表
#$(patsubst %.c, %.o, $(wildcard *.c)) #将搜索到的与*.c匹配的所有以.c后缀的文件替换成以.o后缀的文件
#$(foreach dir, $(DIRS), $(wildcard $(dir)/*.c)) #循环$(DIRS)中的目录(以空格分隔)依次赋给变量dir，然后搜索该目录下匹配*.c的所有文件并返回文件列表

all:$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS)


$(OBJS): %.o:%.cpp
	$(CXX) -g -c $< -o $@ $(CFLAGS)


clean:
	@echo "cleaning project"
	-$(RM) $(OBJS)
	@echo "clean completed"
.PHONY: clean

