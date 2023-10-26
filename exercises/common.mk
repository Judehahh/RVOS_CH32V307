CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -nostdlib -fno-builtin -march=rv32imac_zicsr -mabi=ilp32 -g -Wall

OPENOCD_DIR = ../../MRS_Toolchain_Linux_x64_V1.80/OpenOCD/bin
OPENOCD = ${OPENOCD_DIR}/openocd
CFG = ${OPENOCD_DIR}/wch-riscv.cfg

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
