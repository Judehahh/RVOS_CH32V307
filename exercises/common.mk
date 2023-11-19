ifeq ($(shell uname -s),Linux)
	ifneq ($(shell which riscv64-unknown-elf-gcc),)
		CROSS_COMPILE := riscv64-unknown-elf-
	else
		CROSS_COMPILE := riscv64-elf-
	endif
	OPENOCD_DIR = ../../MRS_Toolchain_Linux_x64_V1.80/OpenOCD/bin
	ifneq ($(shell which gdb-multiarch),)
		GDB := gdb-multiarch
	else
		GDB := riscv64-elf-gdb
	endif
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
