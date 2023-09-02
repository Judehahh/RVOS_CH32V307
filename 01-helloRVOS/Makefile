include ../common.mk

SRCS_ASM = \
	start.S \

SRCS_C = \
	kernel.c \
	uart.c \

OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.S=.o)

.DEFAULT_GOAL := all
all: os.elf

# start.o must be the first in dependency!
os.elf: ${OBJS}
	${CC} ${CFLAGS} -Ttext=0x00000000 -o os.elf $^
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
	@echo "---------------------------------------------------------"
# @echo "Press Ctrl-C and then input 'quit' to exit GDB and OpenOcd"
	@echo "---------------------------------------------------------"
	@${OPENOCD} -f ${CFG} &
	@${GDB} os.elf -q -x ../gdbinit

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean
clean:
	rm -rf *.o *.bin *.elf
