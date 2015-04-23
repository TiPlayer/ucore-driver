CCFLAG = -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Iinclude/ -Iinclude/uapi/ -L/usr/lib32
CC = gcc

demo: demo_chr_dev.o demo.dummy.o
	$(CC) -m32 -L/usr/lib32 demo_chr_dev.o demo.dummy.o -o demo

demo_chr_dev.o: demo_chr_dev.c
	$(CC) $(CCFLAG) demo_chr_dev.c -c

demo.dummy.o: demo.dummy.c
	$(CC) $(CCFLAG) demo.dummy.c -c
