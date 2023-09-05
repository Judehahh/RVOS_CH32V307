# RVOS_ch32v307

此仓库为 RVOS 课程在 ch32v307 开发板上的移植，目前实现了启动程序以及 UART 驱动，可以通过 UART1 打印出 "Hello, RVOS!"。

## 准备
> 以下工作仅在 debian 中得到验证，仅供参考。

> 另外 linux 下的 wch 调试工具并不那么好用，可以选择在 linux 下编译后在 Windows 中再使用 wch 的相关工具进行刷入或调试。

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

调试代码：
```shell
make debug
```
目前 debug 存在在关闭 gdb 后无法关闭 openocd 的问题，需要手动 kill 掉进程

查看反汇编：
```shell
make code
```
