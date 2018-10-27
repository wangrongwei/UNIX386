# DeeppinkOS
&#160; &#160; &#160; &#160;一个i386架构的简单内核，借鉴了《30天自制操作系统》、《一个orange操作系统的实现》和《linux内核设计的艺术》相关代码，但是设计思路属于自创，多处初始化流程不在按照经典的启动流程来，采用较为简单的内核启动方式来完善DeeppinOS。</br>
&#160; &#160; &#160; &#160;DeeppinkOS有两个分支：master和new，master分支可直接clone运行，new分支属于正在开发中的分支，存在较多的bug，不能运行。

# 编译环境

 - 编译环境：ubuntu16.04 64位
 - 编译器：gcc-5.4.0
 - 汇编器：nasm
 - 链接器：ld
 - 仿真器：bochs
 - 其他工具：objcopy

# 说明：
&#160; &#160; &#160; &#160;最开始，打算按照《30天自制操作系统》一书再次慢慢研究操作系统的设计知识，最后由于将开发环境换成ubuntu，使用gcc和nasm编译整个内核，开始此计划时，内核相关知识有但是还缺少实验，再加上对gcc和nasm编译一个x86文件感觉不太可能，参考了许多大牛写的简单内核，最终确定可以修改原来的Makefile文件，使用gcc和nasm。</br>
&#160; &#160; &#160; &#160;使用gcc和nasm断断续续修改Makefile，最终完全解决和达到最初在windows下的效果，大概花了十多天时间，中间遇到许多问题，也不单单是gcc和nasm命令的问题，尤其是是否在第一个汇编文件boot.asm里就进入保护模式。。。。。还是放在kernel.asm里边。。。。
哎呀西八，TMD就是这花了我七八天时间，本来是将进入保护模式的程序放在kernel.asm里边
但是发现不成功，然后我怀疑是不是gcc和nasm的问题，还是代码对齐的问题-----最后没有
这么干，但是发现了问题，是生成的kernel.bin文件前有elf头（大概0x1000），其本质就是
在boot.asm里边跳转地址不对（这都是采用第二种方法后debug出来的） </br>
&#160; &#160; &#160; &#160;还一个说明，就是在保护模式下的跳转（jmp）不能跟原来一样了，，例如：</br>
&#160; &#160; &#160; &#160;jmp 0x08:_start;(_start在第二个文件开头，使用这个指令不知道jmp到什么地方去了) </br>
&#160; &#160; &#160; &#160;可以采用的跳转：</br>
&#160; &#160; &#160; &#160;MOV EAX,0x9000 </br>
&#160; &#160; &#160; &#160;JMP EAX </br>
&#160; &#160; &#160; &#160;感觉如果使用汇编不精通，写的代码啥都有可能发生，解决办法就一个：慢慢使用bochs慢慢单步调试，这些问题都能发现。


# DeeppinkOS文件结构(按照创建先后列出)

boot-------------------------------------------------------------   </br>
&emsp;&emsp;&emsp;|-------boot.asm   </br>
drivers----------------------------------------------------------  </br>
&emsp;&emsp;&emsp;|-------timer.c  </br>
&emsp;&emsp;&emsp;|-------keyboard.c  </br>
include----------------------------------------------------------  </br>
&emsp;&emsp;&emsp;|-------console.h  </br>
&emsp;&emsp;&emsp;|-------string.h  </br>
&emsp;&emsp;&emsp;|-------vargs.h  </br>
&emsp;&emsp;&emsp;|-------debug.h  </br>
&emsp;&emsp;&emsp;|-------descriptor.h  </br>
&emsp;&emsp;&emsp;|-------interrupt.h  </br>
&emsp;&emsp;&emsp;|-------timer.h  </br>
&emsp;&emsp;&emsp;|-------pmm.h  </br>
&emsp;&emsp;&emsp;|-------keyboard.h  </br>
&emsp;&emsp;&emsp;|-------keymap.h  </br>
init---------------------------------------------------------------  </br>
&emsp;&emsp;&emsp;|-------kernel.asm  </br>
&emsp;&emsp;&emsp;|-------start.c  </br>
&emsp;&emsp;&emsp;|-------console.c  </br>
&emsp;&emsp;&emsp;|-------interrupt.c  </br>
&emsp;&emsp;&emsp;|-------pmm.c  </br>
kernel-----------------------------------------------------------  </br>
&emsp;&emsp;&emsp;|-------prink.c  </br>
scripts----------------------------------------------------------  </br>
&emsp;&emsp;&emsp;|-------kernel.ld  </br>
doc----------------------------------------------------------  </br>
&emsp;&emsp;&emsp;|-------help.md  </br>
bochsout.txt--------------bochs调试输出（基本没用）  </br>
bochsrc-------------------bochs调试配置文件(使用指令：make bochs即可)  </br>
Makefile------------------make--->make qemu/make bochs  </br>
deeppink.img--------------最终生成的镜像文件  </br>
README.md-----------------帮助文档（遇到的相关bug和设计步骤）  </br>

# 运行说明
&#160; &#160; &#160; &#160;该工程在ubuntu16.04上已测试通过，需要安装最新的bochs运行，可支持三种方式：

- make bochs
- make qemu
- make debug

&#160; &#160; &#160; &#160;make debug表示以调试模式运行，详情可查看Makefile文档。

# 贡献者

| 贡献者 | 贡献内容 |
| ------ | -------- |
| @wangrongwei | Maintainer |
| @clee01 | Developer |
| @Zhangshuai95 | Developer |
| @Stephenhua | Developer |

# TODO
82258（DMA控制器）</br>
8259A（中断控制器）：Zhangshuai95</br>
8272（磁盘控制器） ：Stephenhua</br>
82385（Cache控制器）</br>
82062（硬盘控制器）：wangrongwei</br>
8254（可编程定时/计数器）：clee01</br>

&#160; &#160; &#160; &#160;实现系统调用（是实现多进程切换的前提），在<b>include/i386</b>文件下添加<b>sys.h</b>文件（包含系统库），初步打算再<b>kernel.asm</b>下实现系统调用。正在完善中...
