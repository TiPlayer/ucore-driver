#uCore plus for Linux Driver

*岳士超*
tcplayer.yue@gmail.com  

----

##mod-hello

mod-hello 是一个示例kernel module, 它区别于其他uCore的kmodule在于它引用的是linux header，而不是ucore的header
为了可以正常编译运行， 我们需要如下步骤:   

```cpp
将hello.c放入header-gen的linux文件夹下
用header-gen生成所需要的头文件hello.d和hello.dummy.c
将hello.d和hello.c, hello.dummy.c拖入kmodule/mod-hello下
为了可以使用ucore的函数，需要在hello.c里引用一个ucore的头文件
需要更改linux header中一些函数定义，使之与ucore无冲突(已完成)
需要增添一些header-gen生成的头文件中缺少的宏定义，使之没有编译错误(已完成)
需要更改ucore中module的定义，增加padding使位移量与linux定义相同(已完成)
需要更改ucore的makefile使之能引用linux header(已完成，但需要改进)
```
具体makefile更改位置在ucore_driver/ucore/src/kern-ucore/Makefile.subdir  
如果可以的话这个makefile可以更加通用一些，不需要为了某一个特定的mod做更改  

之后再有新的mod的时候可以直接将hello.d复制并覆盖新的头文件即可

ucore-driver/mod-hello-test是一个可以编译并且运行的C程序，可以在里面尝试各种Linux的数据类型，看struct里每一项的偏移量等，是一个很好地实验程序。可以根据自己的需求随意更改
