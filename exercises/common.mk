ifeq ($(shell uname -s),Linux)
	CROSS_COMPILE := $(shell which riscv64-unknown-elf-gcc > /dev/null 2>&1 && echo riscv64-unknown-elf- || echo riscv64-elf-)
	OPENOCD_DIR = ../../MRS_Toolchain_Linux_x64_V1.80/OpenOCD/bin
	GDB = gdb-multiarch
else ifeq ($(shell uname -s),Darwin)
	CROSS_COMPILE = riscv64-elf-
	OPENOCD_DIR = ../../MRS_Toolchain_MAC_V190/openocd_x86_64/openocd_x86_64/bin
	GDB = riscv64-elf-gdb
endif

CFLAGS = -nostdlib -fno-builtin -march=rv32imac_zicsr -mabi=ilp32 -g -Wall

OPENOCD = ${OPENOCD_DIR}/openocd
CFG = ${OPENOCD_DIR}/wch-riscv.cfg

CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
