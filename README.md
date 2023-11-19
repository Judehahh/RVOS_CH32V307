# RVOS_ch32v307

此仓库为 RVOS 课程在 ch32v307 开发板上的移植，目前已完成：
- [x] 00-bootstrap
- [x] 01-helloRVOS
- [x] 02-memanagement
- [x] 03-contextswitch
- [x] 04-multitask
- [x] 05-traps
- [x] 06-interrupts
- [x] 07-hwtimer
- [x] 08-preemptive
- [x] 09-lock
- [ ] 10-swtimer
- [ ] 11-syscall
- [x] exercise-7-1-helloRVOS-asm
- [x] exercise-7-2-helloRVOS-echo
- [x] exercise-8-1-memanagement-byte
- [x] exercise-9-1-multitask-with param, priority and exit
- [x] exercise-9-2-multitask-scheduletask
- [ ] exercise-11-1-uart-write-interrupt
- [ ] exercise-12-1-digital-clock
- [ ] exercise-14-1-spinlock
- [ ] ...

## 准备

### Linux

> 如果觉得 linux 下的 OpenOCD 工具不那么好用，可以选择在 linux 下编译后在 Windows 中再使用 wch 的相关工具进行刷入或调试。

#### 工具安装：

**Debian / Ubuntu**：
```shell
sudo apt install gcc-riscv64-unknown-elf binutils-riscv64-unknown-elf gdb-multiarch
```

**Archlinux**：
```shell
sudo pacman -S riscv64-elf-gcc riscv64-elf-binutils riscv64-elf-gdb
```

#### 下载代码以及 OpenOCD 工具：
```shell
git clone https://github.com/Judehahh/RVOS_ch32v307.git
cd RVOS_ch32v307
wget http://file.mounriver.com/tools/MRS_Toolchain_Linux_x64_V1.80.tar.xz
tar -xvf MRS_Toolchain_Linux_x64_V1.80.tar.xz
```

### MACOS

工具安装：
```shell
brew install riscv64-elf-binutils riscv64-elf-gcc riscv64-elf-gdb
```

下载代码以及 OpenOCD 工具：
```shell
git clone https://github.com/Judehahh/RVOS_ch32v307.git
cd RVOS_ch32v307
wget wget http://file.mounriver.com/tools/MRS_Toolchain_MAC_V190.zip
unzip MRS_Toolchain_MAC_V190.zip
cd MRS_Toolchain_MAC_V190
unzip openocd_x86_64.zip
cd ..
```

## 运行

编译：
```shell
cd 01-helloRVOS
make
```

将 wch-link 接入电脑，运行以下命令将代码刷入开发板中：
```shell
make flash
```

使用`57600`波特率打开串口，按下开发板上的`RESET`按钮，即可观察到输出：    
![output](/pic/output.png)

调试代码：
```shell
make debug
```
目前 debug 存在在关闭 gdb 后无法关闭 openocd 的问题，需要手动 kill 掉进程

查看反汇编：
```shell
make code
```

## 可能遇到的问题

### 1. 找不到libhidapi-hidraw.so.0

运行 `make flash` 报错：
```
../MRS_Toolchain_Linux_x64_V1.80/OpenOCD/bin/openocd: error while loading shared libraries: libhidapi-hidraw.so.0: cannot open shared object file: No such file or directory
```

解决：
```shell
cd MRS_Toolchain_Linux_x64_V1.80/beforeinstall/
./start.sh
```

### 2. LIBUSB_ERROR_ACCESS

运行 `make flash` 报错：
```
Error: libusb_open() failed with LIBUSB_ERROR_ACCESS
Error: open failed
```

解决：

在执行过 `start.sh` 之后执行以下命令
```shell
sudo groupadd plugdev
sudo usermod -a -G plugdev $USER
sudo udevadm control --reload-rules && sudo udevadm trigger
```

## 说明

### 05. Trap 和 Exception

青稞 V4 中使用*可编程快速中断控制器*（Programmable Fast Interrupt Controller,PFIC）来管理异常和中断，与 Qemu 中的 PLIC 不同，具体可参考 `CH32FV2x_V3xRM.PDF` 的 `第 9 章 中断和事件` 以及 `QingKeV4_Processor_Manual.PDF` 的 `第 3 章 PFIC 与中断控制`。

异常处理与课程中基本一致。

### 06. 外部设备中断

PFIC 的设置与课程中有些许不同，若要开启某个中断，需在 PFIC init 时 `PFIC_IENRx` (x = 1, 2, 3, 4) 寄存器中将该中断号对应的位置位。如“USART1 全局中断”的编号为 `53`，需要将 `PFIC_IENR2` 的第 $53 - 32 = 21$ 位置1。
```c
/* enable usart1 interrupts. */
*(uint32_t *)PFIC_IENR2 |= (uint32_t)(0x00200000);
```

### 07. 硬件定时器

由于外设寄存器数量增加，参照沁恒官方的写法，修改成以 `struct` 的方式来读写寄存器，使寄存器读写操作更加方便；同时将 rcc 的初始化工作分离出来到 `rcc.c` 中。

对于 CH32V307 的定时器，青稞 V4 内部设计了一个 SysTick，具体参考 `QingKeV4_Processor_Manual.PDF` 的第五章。

要开启定时器中断，需要在 PFIC 将 `IENR1` 的第 12 位置 1（即 `12` 号中断）。
```c
/* enable SysTick interrupts. */
PFIC_REG->IENR[0] |= (uint32_t)(0x00001000);
```

在每次时钟中断发生后，需要清除计数值比较标志，否则会一直触发中断，同时清除定时器计数值（或者继续增加 CMP 的值）：
```c
void timer_handler()
{
    STK_REG->CTLR |= (uint32_t)(1 << 5);
    // STK_REG->CMP += (uint64_t)(p_ms * 1000);

    STK_REG->SR &= ~(1 << 0);
}
```

### 08. 抢占式多任务

软件中断中断号为 `14`，需在 `PFIC_REG->IENR[0]` 中打开。

### 09. 任务同步和锁

本章代码与课程相同。
