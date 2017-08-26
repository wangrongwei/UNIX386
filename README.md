# DeeppinkOS
操作系统研究，借鉴了许多《30天自制操作系统》、《一个orange操作系统的实现》和
《linux内核设计的艺术》上面的代码

最开始在Windows下借鉴《30天～～～》，最后将代码移植到ubuntu下
编译环境：
系统：ubuntu16.04 64位
#说明：
    最开始，打算按照《30天自制操作系统》一书再次慢慢研究操作系统的设计知识，最后
由于将开发环境换成ubuntu，使用gcc和nasm编译整个内核，开始此计划时，内核相关知识有
但是还缺少实验，再加上对gcc和nasm编译一个x86文件感觉不太可能，参考了许多大牛写的
简单内核，最终确定可以修改原来的Makefile文件，使用gcc和nasm
    使用gcc和nasm断断续续修改Makefile，最终完全解决和达到最初在windows下的效果，
大概花了十多天时间，中间遇到许多问题，也不单单是gcc和nasm命令的问题，尤其是是否
在第一个汇编文件boot.asm里就进入保护模式。。。。。还是放在kernel.asm里边。。。。
哎呀西八，TMD就是这花了我七八天时间，本来是将进入保护模式的程序放在kernel.asm里边
但是发现不成功，然后我怀疑是不是gcc和nasm的问题，还是代码对齐的问题-----最后没有
这么干，但是发现了问题，是生成的kernel.bin文件前有elf头（大概0x1000），其本质就是
在boot.asm里边跳转地址不对（这都是采用第二种方法后debug出来的）
    还一个说明，就是在保护模式下的跳转（jmp）不能跟原来一样了，，例如：
    jmp 0x08:_start;(_start在第二个文件开头，使用这个指令不知道jmp到什么地方去了)
    可以采用的跳转：
    MOV EAX,0x9000
    JMP EAX
感觉如果使用汇编不精通，写的代码啥都有可能发生，解决办法就一个：慢慢使用bochs慢慢
单步调试，这些问题都能发现

#DeeppinkOS文件结构(每个文件下按照创建先后列出)
boot----------------------
    |-------boot.asm
include-------------------
    |-------console.h
    |-------string.h
    |-------vargs.h
    |-------debug.h
    |-------descriptor.h
init----------------------
    |-------kernel.asm
    |-------start.c
    |-------console.c
kernel--------------------
    |-------prink.c
scripts-------------------
    |-------kernel.ld
bochsout.txt--------------bochs调试输出（基本没用）
bochsrc-------------------bochs调试配置文件
Makefile------------------
deeppink.img--------------最终生成的镜像文件
README.md-----------------帮助文档（遇到的相关bug和设计步骤）

##############################################################
##############################################################
#       开发过程中遇到的相关bug和流程
##############################################################
##############################################################

-------->2017.05.13
C:\kernel developer\tolset\helloos0>make

C:\kernel developer\tolset\helloos0>..\z_tools\make.exe
../z_tools/make.exe -r img
make.exe[1]: Entering directory `C:/kernel developer/tolset/helloos0'
../z_tools/make.exe -r wrwos.img
make.exe[2]: Entering directory `C:/kernel developer/tolset/helloos0'
../z_tools/nask.exe bootfirst.nas bootfirst.bin bootfirst.lst
NASK : LSTBUF is not enough
make.exe[2]: *** [bootfirst.bin] Error 19
make.exe[2]: Leaving directory `C:/kernel developer/tolset/helloos0'
make.exe[1]: *** [img] Error 2
make.exe[1]: Leaving directory `C:/kernel developer/tolset/helloos0'
..\z_tools\make.exe: *** [default] Error 2
    这个错误是没有将bootfirst后边填充数据的代码去掉，由于添加了其他代码，空间已经不过用了

-------->2017.05.14
C:\kernel developer\tolset\helloos0>make

C:\kernel developer\tolset\helloos0>..\z_tools\make.exe
../z_tools/make.exe -r img
make.exe[1]: Entering directory `C:/kernel developer/tolset/helloos0'
../z_tools/make.exe -r wrwos.img
make.exe[2]: Entering directory `C:/kernel developer/tolset/helloos0'
../z_tools/nask.exe bootfirst.nas bootfirst.bin bootfirst.lst
../z_tools/edimg.exe   imgin:../z_tools/fdimg0at.tek \
        wbinimg src:bootfirst.bin len:512 from:0 to:0 \
        copy from:bootsecond.sys to:@: \
        imgout:wrwos.img
imgout BPB data error.
make.exe[2]: *** [wrwos.img] Error 37
make.exe[2]: Leaving directory `C:/kernel developer/tolset/helloos0'
make.exe[1]: *** [img] Error 2
make.exe[1]: Leaving directory `C:/kernel developer/tolset/helloos0'
..\z_tools\make.exe: *** [default] Error 2
FAT12文件的很重要，不能随意更改其中的文件名字（只是更改名字会出错）


-------->2017.05.24
       向64h端口写入的字节，被认为是对8042芯片发布的命令（Command）： 写入的字节将会被存放Input Register中； 同时会引起Status Register的Bit-3自动被设置为1，表示现在放在Input Register中的数据是一个Command，而不是一个Data；
       在向64h端口写某些命令之前必须确保键盘是被禁止的，因为这些被写入的命令的返回结果将会放Output Register中，而键盘如果不被禁止，则也会将数据放入到Output Register中，会引起相互之间的数据覆盖；
       在向64h端口写数据之前必须确保Input Register是空的（通过判断Status Register的Bit-1是否为0）。60h端口（读操作），对60h端口进行读操作，将会读取Output Register的内容。Output Register的内容可能是：来自8048的数据。这些数据包括Scan Code，对8048发送的命令的确认字节（ACK)及回复数据。 通过64h端口对8042发布的命令的返回结果。在向60h端口读取数据之前必须确保Output Register中有数据（通过判断Status Register的Bit-0是否为1）。
       60h端口（写操作）向60h端口写入的字节，有两种可能： 
      1．如果之前通过64h端口向8042芯片发布的命令需要进一步的数据，则此时写入的字节就被认为是数据； 
      2．否则，此字节被认为是发送给8048的命令。 在向60h端口写数据之前，必须确保Input Register是空的（通过判断Status Register的Bit-1是否为0）。

-------->2017.06.21
向一个空软盘保存文件的时候,
1.文件名会写在0x2600以后的地方(这里我没有去验证);
2.文件的内容会写入到0x4200以后的地方.
决定后边的0xc200

-------->2017.06.23
      使用一般的编译器编译操作系统，不再依赖原来作者自己的编译器
>>使用linux系统，在ubuntu下选择和组合一套工具编译运行这个操作系统

2017/08/22
   当前对系统只将堆栈指针指向0x7c00，此处在C代码里边定义一个全局变量，似乎没有存储
这个全局变量的空间，如果设置成局部变量，达到了预想的效果，猜测可能是没有在链接脚本
指定全局变量空间
   debug:此处的问题不是链接脚本的问题，是生成的kernel.bin文件是一个elf文件，文件最
开始是elf的头，所以是跳转的地址不对

2017/08/23
   完善Deeppink的显示字符串函数(console_puts)，添加对字符串里换行等的支持，添加
光标移动函数(console_movecursor)

2017/08/24
   添加kernel/printk.c文件
2017/08/25
   添加descriptor.h文件，使用c语言重新初始化gdt和idt表
   问题：在使用bochs调试时，sreg命令显示的valid=1 or 31是什么意思

