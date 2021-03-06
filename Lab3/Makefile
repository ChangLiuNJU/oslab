# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
LD = ld
CFLAGS = -m32 -march=i386 -static -MD -std=gnu89 -ggdb \
		 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer \
		 -Wall -Werror -I./include
ASFLAGS = -ggdb -m32 -MD
LDFLAGS = -melf_i386
GITFLAGS = -q --author='tracer <tracer@njuoslab.org>' --no-verify --allow-empty
QEMU = qemu-system-i386

# 编译目标：src目录下的所有.c和.S文件
CFILES = $(shell find src/ -name "*.c")
SFILES = $(shell find src/ -name "*.S")
OBJS = $(CFILES:.c=.o) $(SFILES:.S=.o)

kernel.img: kernel
	@cd boot; make
	cat boot/bootblock kernel > kernel.img
	# -@git add . --ignore-errors &> /dev/null # KEEP IT
	-@(echo "> compile" && uname -a && uptime && pstree -A) | git commit -F - $(GITFLAGS) # KEEP IT

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -e entry -Ttext 0xC0100000 -o kernel $(OBJS)
	objdump -D kernel > code.txt
	readelf -a kernel > elf.txt

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: play clean debug
play: kernel.img
	# -@git commit -m "> play" $(GITFLAGS) # KEEP IT
	$(QEMU) -no-reboot -no-shutdown -serial stdio kernel.img

debug: kernel.img
	# -@git commit -m "> debug" $(GITFLAGS) # KEEP IT
	$(QEMU) -no-reboot -no-shutdown -serial stdio -s -S kernel.img

clean:
	@cd boot; make clean
	rm -f kernel kernel.img $(OBJS) $(OBJS:.o=.d)
