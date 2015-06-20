#操作系统课程设计结题报告
岳士超  
2012011295  
tcplayer.yue@gmail.com

-----------------

```
github: https://github.com/TiPlayer/ucore-driver
```

##Linux Kernal Module Support
### 概述: 
> 本模块是在OsTrain2014/KernelModule的基础上，借鉴了OsTrain2012/DDE的实现方式，最终实现了对Linux Kernal Module的初步的支持

### 实现逻辑:
> 首先我们需要知道ucore目前有什么和没有什么。
> 
> 目前ucore有一套virtual device体系，代表是stdin & stdout, 这两个vdev并不在sfs体系内，而藏在ucore的深处一个叫`vdev_list`的链表里。每次用户`syscall::open`的时候，ucore会检查目标路径。如果目标路径里面有`:`字符，说明这是一个虚拟设备，那么ucore会在`vdev_list`里面遍历，寻找名称相同的虚拟设备，如果找到了就返回这个虚拟设备的handler, 否则会失败。
> 
> 这个逻辑和linux不同，性能比较差，但是简单暴力，易于操作。不需要mknod等支持。所以我利用这套系统实现了对linux char device的支持。
> 
> 首先一个char device有他的io函数和名称等。我们需要创建一个ucore::device来"装载"这个cdev, ucore::device缺少一些变量所以要修改对应的声明。由于ucore的device io的定义与linux的定义不同，所以要声明一些adapter让ucore的文件io方式与linux的统一。
> 
> 于是现在整个的加载及使用过程是这样的, 以mod-demo为例
> 
> 1. `insmod mod-demo`
> > 通过`insmod mod-demo`加载mod-demo这个kmodule, 这个module是一个名为chr_dev的字符设备，支持读和写操作。在加载时ucore将这个module加载进字符设备的hash列表中，在加载时得到主从设备号。但是由于ucore目前没有mknod所以这个主从设备号目前没有使用。在初始化的时候更重要的一步是新建一个`ucore::device`封装这个chr_dev, 把它放到vdev_list中。
> 
> 2. 一个样例用户程序如下，可以在`user-ucore/demo.c`找到
> > ```c
#define CHR_DEV_NAME "chr_dev:"
int main() {
  		int ret;
  		char hello[] = "Hello, world!\n";
  		char buffer[100];
  		int fd = open(CHR_DEV_NAME, O_RDWR);
  		if (fd < 0) {
    		printf("open file %s failed!\n", CHR_DEV_NAME);
    		return -2012;
  		}
  		write(fd, hello, 15);
  		read(fd, buffer, 32);
  		close(fd);
  		return 0;
}
```
> > 目前效果是cdev输出自己所得到的字符。
> >  

### 实现方式:
> 为了可以实现对Linux Kernal Module的*无修改*支持，我一共在OsTrain2014/KernelModule的基础上做了以下的修改
> 
> 1. 框架声明的完善
> > 由于在Linux Header下编译的KModule相关变量(比如入口函数，出口函数)的偏移量与uCore不同，所以需要对uCore的kModule添加一定的padding，使得uCore可以正常读取Linux的kModule.   
> > 另外需要对比如inode和device等函数添加Linux的读取方式。因为Linux的文件IO参数和uCore不同，所以需要两套读写系统。    
> > 其他细节问题在这里不加赘述
> 
> 2. Linux Header与uCore框架的融合
> > 这是一个很棘手的问题，因为Linux和uCore存在大量的name conflict, typedef conflict, marco conflict.  
> > 为了解决这个问题，我一共采用了如下的几种方法。
> > 
> > 1. 宏定义
> > > 本方法是从OsTrain2012/DDE中学习来的，在引用某一个Header的时候情况如下
> > >
```cpp
#define __NO_UCORE_TYPE__
#include <device.h>
```
> > > 然后在device.h中有类似于如下的定义:
> > > 
```cpp
#ifdef __NO_UCORE_TYPE__
struct ucore_device {
#else 
struct device {
#endif
```
> > > 这样就可以实现在不改变其他的源代码的情况下实现部分linux和ucore的融合。这种实现方式非常值得借鉴。然后OsTrain2012/DDE的部分代码还遗留在OsTrain2014/KernelModule中，所以我可以直接搬过来用。感觉非常好。
> > 
> > 2. Adapter框架
> > > 为了实现kernel module的知识。进一步实现一些Linux kernal module所必需的支持函数。但是在实现支持函数的时候不能引用大量的ucore header，否则会有大量的命名冲突。所以在实现这些linux函数的时候要严格控制ucore的header 引用数量。  
> > > 但有的时候为了实现需要我们无法避免要进行深度ucore header引用，为此我设计了一个新的架构，具体如下:
> > > 
```
-- LinuxAdapter
|
| - ucoreAdapter 
|  (只能引用ucore的header, 
|   对外暴露最少的接口，减少header引用)
|
| - linuxFunctions 
|  (引用所有需要的Linux Header, 
|   尽可能少的引用ucore struct定义header, 
|   所有需要的函数全部靠extern)
|
| - include 
|  (一个通过header-gen生成的linux-3.16 header组) 
```
> >
> > 3. 源代码修改
> > > 即使使用了以上的技巧，还是由一些问题需要手动修改源代码解决。主要集中在以下两类
> > > 
> > > 1. 单一函数在头文件中的实现
> > > > 比如``linux::copy_to_user`` 函数  
> > > > copy_to_user在linux header中是inline实现的，但是无法在ucore中使用，ucore中有相同名称的函数但是参数不同，不能直接调用。而且在kmodule中是不能直接调用ucore的header的，因为我们希望可以不加修改的使用linux header.  
> > > > 
> > > > 对于这种情况我使用了如下的处理技巧:  
> > > > 
> > > > 首先在ucoreAdapter中实现一个``__ucore_copy_to_user``函数，其参数和返回值和linux一样，然后内部用``ucore::copy_to_user``实现，即默认是要把current中的vmm地址中制定的部分复制到kernel空间中。之后把这个函数export成symbol. 然后再修改linux header中具体的实现，把原实现删掉，改为直接return新实现的这个参数。这样就可以避免命名冲突，同时可以实现这个函数的功能。虽然比较步骤比较复杂，但是还是可以维护的。
> > > 
> > > 2. linux内函数声明的修改
> > > > 在linux header中存在着大量的函数声明和ucore中所声明的函数名称一样但是参数类型不一样的情况。在这种时候我选择直接修改linux header中相关函数的参数类型.
> > > > 
> > > > 比如如下情况
> > > > 
> > > > 在`include/asm/string_64.h`中有如下函数声明
> > > > 
```cpp
int memcmp(const void *cs, const void *ct, ssize_t count);
```
> > > > 但是在ucore中最后一个参数`count`的类型是`size_t`, 我在不改变原来参数的意义的情况下会让linux函数类型和ucore统一，即把`ssize_t`改为`size_t`
> > > > 
> > 4. 具体实现细节
> > > 本工作基于OsTrain2014/KernelModule, 这个ucore_plus已经可以较好的支持linux模块的加载，经过我上述所说的header声明完善之后已经可以加载linux hello kmodule, 这个在`/kern-ucore/kmodule/modules/mod-hello/`找到. 为了可以在加载时在初始化函数的最后输出hello, 我们需要实现linux的输出函数`printk `, 具体实现方式可以在ucoreAdapter中找到，需要注意的一点是要在末尾处增加`EXPORT_SYMBOL(printk)`，否则会找不到符号。
> > > 
> > > 为了进一步支持一个简单的字符设备，需要实现以下参数:
> > > 
```cpp
static struct char_device_struct * __register_chrdev_region(unsigned int major, unsigned int baseminor, int minorct, const char *name)
int alloc_chrdev_region(dev_t * dev, unsigned int baseminor, unsigned int count, const char * name) 
int cdev_add(struct cdev * cdev, dev_t dev, unsigned int count)
void cdev_init(struct cdev * cdev, const struct file_operations * fops) 
static struct char_device_struct * __unregister_chrdev_region(unsigned major, unsigned baseminor, int minorct) 
void unregister_chrdev_region(dev_t from, unsigned int count
```
> > > 具体实现可以在linuxFunctions中找到。我基本复制了linux的实现方式，并做了一些修改使得它可以在ucore上跑，用到的技巧主要是如下这种方式.
> > > 
> > > 因为在实现这些函数的时候需要涉及到linux::device和ucore::device, 但明显不可能同时在c这种没有命名空间的语言里完成这种工作。所以需要绕开。首先通过我上述所讲的方式在引用ucore header之前加入声明NO_UCORE_DEVICE，讲ucore::device改名为ucore_device. 然后在ucoreAdapter中实现了自己所需要用到的函数。之后在linuxFunctions中，首先通过extern的方式得到这个函数的声明。由于c的函数声明不需要参数，所以在这里我们可以直接改变extern声明时的参数类型，比如
> > > 
```cpp
//原ucore中实现
void addLinuxDevice(struct device* udev);
//在linuxFunctions中的声明
extern void addLinuxDevice(struct ucore_device* udev);
```
通过这种方式我们就可以避免命名冲突同时可以区分ucore::device和linux::device。不过使用这种方式需要极其清晰的大脑，否则容易把自己绕进去。
> > 
> > 5. 设备IO的Adapter
> > > 由于Linux Device的设备IO函数和ucore中定义有所差异，所以我们需要自己实现一些Adapter, 让这些字符设备理解ucore传给他们的参数。所以要做如下事情。
> > > 
> > > 首先在`ucore::device`中保存cdev设备IO函数指针，然后真正的设备IO函数指向自己所写的Adapter, 在Adapter中将所有的参数转化成符合linux标准的样子，再传给cdev
> > > 
> > > 所有的cdev的IO Adapter可以在`kern-ucore/kmodule/linuxAdapter/linuxFunctions/cdev.c`中找到
> > > 具体如下:
> > > 
```c
int ucore_cdev_open_adapter(struct ucore_device * dev, uint32_t open_flags);
int ucore_cdev_close_adapter (struct ucore_device * dev);
int ucore_cdev_io_adapter (struct ucore_device * dev, struct ucore_iobuf * iob, bool write) ;
int ucore_cdev_ioctl_adapter (struct ucore_device * dev, int op, void *data) {;
```

###下一步
> 如果希望在这部分工作的基础上进行下一步完善，可以从以下几个方向入手
> 
> 1. mknod
> > 这个功能我一直想做但是最后去做了External Interrupt Handler Frame, 要想做好这部分需要更加清楚的了解文件系统的工作原理，然后ucore的sfs上再搭载一个dev-fs. 然后删除vdev-list, 让stdin, stdout以正规的inode方式存在，以open方式打开。
> 
> 2. 与中断系统结合
> > 按向勇老师所说，可以试试加载一个处理IDE中断的驱动。然后一起测试这两部分。不过这个工作量比较大，首先要把已经有的IDE驱动程序变为标准的linux kmodule, 然后注册驱动处理程序。最后看看能不能正常运行。


##External Interrupt Handler Frame

###概述
> ucore的interrupt处理机制即`trap_dispatch`函数目前是一个switch-case, 所有中断号的处理机制都是写死的没有变化的空间。但是如果我们希望未来ucore可以通过kmodule等方式加入设备驱动并且可以动态加入新的设备的时候，必然涉及到动态添加新的中断。所以这一部分就是在为这种情况考虑。
> 
> 通过这部分的工作目前ucore可以动态注册中断处理函数，动态添加新的中断号。并且值得强调的一点是这个中断处理框架我尽量希望可以做到通用，在设计的时候考虑到外留接口，希望可以不仅能在x86上使用，而且可以不加修改的在其他平台(如ARM)上使用。但是由于对其他平台的了解不够充分，也没有配置相应的平台，所以没有进行相应的测试，但理论上是可以工作的。


###实现逻辑  
                                                                                                                                           
> 首先硬件会给一个硬件终端号我们称之为`vector`, 然后传到cpu中，这个框架首先会调用一个平台相关的函数把`vector`转换成逻辑中断号`irq`. 
> 
> 然后通过这个`irq`可以到hash表中查询到一个中断处理函数链表，然后一个一个的查询。每一个函数会返回以下三种情况之一: 1.是我的且处理好了, 2:是我的但没处理好, 3:不是我的。如果不是这个处理函数的中断，则继续查询这个链表中下一个函数，直到遇到链表的末尾。如果都不是的话直接忽视这个中断，否则的话就停止中断处理，返回处理结果。
> 
> 添加设备中断的函数如下:
> 
```c
int register_intr_handler(int logic_no, if_t func, void* data, int solo);
```
> 在添加中断处理函数的时候，首先要考虑这个中断处理函数是不是可以判断这个中断来自己自己的目标设备。因为有些设备是有中断寄存器的，比如lapic的timer，可是查询得到当前设备是不是发出了中断信号。对于这种中断函数他们不需要独占一个中断号。否则的话他们需要独占中毒号, 即solo=1. 我们在添加的时候，需要判断solo的值，如果是一的话则需要保证链表里对应中断号只有这一个处理程序，否则返回失败。
> 
> 逻辑中断号的获得是由另一个接口函数规定的:
> 
```c
int default_get_logic_irq(int me);
```
> 目前由于ucore没有PCI总线等必要的支持，无法动态的获得设备的逻辑中断号，所以这部分是写死的，工作原理就是一个switch-case, 我告诉你我是谁，你要告诉我我能使用的中断号是什么。如果之后PCI总线完善了，这个函数的参数或许会变化，但是实现的功能不变。
> 
> 这个框架的初始化函数如下:
> 
```c
void irq_manager_init(int(*p2l_irq)(int), int (*get_logic_irq)(int), void (*eoi)());
```
> 其中`p2l_irq`是平台相关的物理中断号到逻辑中断号的映射函数。`get_logic_irq`是设备获取自己的逻辑中断号的函数，和`default_get_logic_irq`地位相同。但是由于目前ucore没有对应的支持所以这个参数没有用。`eoi`是为某些中断处理设备准备的，比如lapic。 这些设备在处理完中断后需要发送eoi。但是目前ucore没有用lapic而是pic，所以`eoi`这个参数也没有使用。在x86平台上，这个函数调用后，当cpu得到一个大于等于32的物理中断号的时候，ucore不会再用原来的switch-case中断处理函数，而是会转到新的处理平台上。
> 
> 最后这个函数是给平台相关的中断处理程序用的，接受的参数是一个物理逻辑中断号
> 
```c
void external_irq_handler(int vector);
```
> 通过调用这个函数就可以处理相应的物理中断。

###下一步
> 这个框架目前还不能体现它真正的价值，因为目前ucore没有PCI总线。我认为最重要的一步就是PCI了。只有有了PCI，才能动态的添加删除物理设备，也只有这样才会涉及到动态的中断注册机制。
> 
> 现在xv6中有一个基本可用的PCI机制，可以考虑将这一部分移植到ucore中，然后再把中断处理机制融合进来。
> 

