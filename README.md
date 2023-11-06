# RVOS_ch32v307

此仓库为 RVOS 课程在 ch32v307 开发板上的移植，目前已完成：
- [x] 00-bootstrap
- [x] 01-helloRVOS
- [x] 02-memanagement
- [x] 03-contextswitch
- [x] 04-multitask
- [x] 05-traps
- [x] 06-interrupts
- [ ] 07-hwtimer
- [x] exercise-7-1-helloRVOS-asm
- [x] exercise-7-2-helloRVOS-echo
- [x] exercise-8-1-memanagement-byte
- [x] exercise-9-1-multitask-with param, priority and exit
- [x] exercise-9-2-multitask-scheduletask
- [ ] exercise-11-1-uart-write-interrupt
- [ ] ...

## 准备

### Linux

> 以下工作仅在 debian 中得到验证，仅供参考。

> 如果觉得 linux 下的 OpenOCD 工具不那么好用，可以选择在 linux 下编译后在 Windows 中再使用 wch 的相关工具进行刷入或调试。

工具安装：
```shell
sudo apt install gcc-riscv64-unknown-elf binutils-riscv64-unknown-elf gdb-multiarch
```

下载代码以及 OpenOCD 工具：
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

## 说明

### 07. 硬件定时器

由于外设寄存器数量增加，参照沁恒官方的写法，修改成以 `struct` 的方式来读写寄存器，使寄存器读写操作更加方便；同时将 rcc 的初始化工作分离出来到 `rcc.c` 中。