include ../common.mk

SRCS_ASM = \
	start.S \
	mem.S \
	entry.S \

SRCS_C = \
	kernel.c \
	uart.c \
	printf.c \
	page.c \
	sched.c \
	user.c \
	trap.c \
	pfic.c \
	rcc.c \
	timer.c \

OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.S=.o)

.DEFAULT_GOAL := all
all: os.elf

# start.o must be the first in dependency!
os.elf: ${OBJS}
	${CC} ${CFLAGS} -T os.ld -o os.elf $^
	${OBJCOPY} -O binary os.elf os.bin

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

.PHONY : flash
flash: all
	@echo "------------------------"
	@echo "Flashing os.bin to board"
	@echo "------------------------"
	@${OPENOCD} -f ${CFG}  -c init -c halt  -c "program os.bin"  -c exit

.PHONY : debug
debug: all
	@echo "-----------------------------------------------"
	@echo "Please manually kill the openocd after gbd quit"
	@echo "-----------------------------------------------"
	@${OPENOCD} -f ${CFG} &
	@${GDB} os.elf -q -x ../gdbinit

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean
clean:
	rm -rf *.o *.bin *.elf
