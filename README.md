# http_multi_thread_download_tool
基于libcurl多线程下载工具，可实现HTTP下载(实现)，ftp下载(可扩展)等下载方式，可设置下载阀值(线程数根据该阀值自动配置生成)，程序会自动判断是否需要多线程分段下载，程序具有一定扩展性，采用模板多态方式，无需改动主体多线程下载逻辑，只需添加相应任务子类文件即可

a.	编译准备
编译安装curl。
Makefile文件，如下：
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
	$(CXX) -g -c $< -o $@ $(CFLAGS) $(CURL_LIB)

clean:
	@echo "cleaning project"
	-$(RM) $(OBJS)
	@echo "clean completed"
.PHONY: clean 

b.	编译环境
tu@tu-virtual-machine:~$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 17.10
Release:	17.10
Codename:	artful
tu@tu-virtual-machine:~$ uname -a
Linux tu-virtual-machine 4.13.0-46-generic #51-Ubuntu SMP Tue Jun 12 12:36:29 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
tu@tu-virtual-machine:~$ gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/7/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 7.2.0-8ubuntu3.2' --with-bugurl=file:///usr/share/doc/gcc-7/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++ --prefix=/usr --with-gcc-major-version-only --program-suffix=-7 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-libmpx --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 7.2.0 (Ubuntu 7.2.0-8ubuntu3.2) 
tu@tu-virtual-machine:~$ 

c.	使用帮助，如下：
tu@tu-virtual-machine:/mnt/hgfs/linux_study/work/tu/future/test_multi_thread_download$ ./multi_thread_download -h
description:
-H http方式下载.
-F ftp方式下载(可扩展，未实现).
-O 其他方式下载(可扩展，未实现).
-s 分段下载阀值size(单位m,不设置时默认值为20),当下载文件大于阀值启用分段下载，否则不开启.
-h help.

e.g:
test -H 'download url' -s 20
bye!
tu@tu-virtual-machine:/mnt/hgfs/linux_study/work/tu/future/test_multi_thread_download$ 

d.	输入命令，开始下载
tu@tu-virtual-machine:/mnt/hgfs/linux_study/work/tu/future/test_multi_thread_download$ ./multi_thread_download -H http://fastsoft.onlinedown.net/down/wireshark3.0.0.zip -s 50
[notice] create the 0th work thread
[notice] create the 1th work thread
[notice] create the 2th work thread
[notice] downloading...
[notice] generate wireshark3.0.0.zip in the current direction
[notice] download sucessfully!
bye!
tu@tu-virtual-machine:/mnt/hgfs/linux_study/work/tu/future/test_multi_thread_download$

