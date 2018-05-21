# u-boot-nju 开发记录

规则：

- `*` 修改文件
- `+` 增加文件
- `-` 删除文件

样例：

---
#### 更新时间: 
`2018-05-16`

#### 负责人员：
- rex

#### 主要更新内容：
- 控制 `gpio J6` 的 `out` 为高电平，使得连接的发光二极管被关闭，它可以作为内核早期开发的调试

#### 主要涉及的文件：

```
* /common/board_r.c
+ /common/bootvtos.c
- /common/bootm.c
```

## 以下是正文
---
#### 更新时间: 
`2018-05-16`

#### 负责人员：
- rex

#### 主要更新内容：
- 控制 `gpio J6` 的 `out` 为高电平，使得连接的发光二极管被关闭，它可以作为内核早期开发的调试
- 显示串口寄存器的地址
- 直接操作串口寄存器，实现一个比 `driver model` 更简单的 `printf` 函数，也可以用于内核早期开发的调试 

#### 主要涉及的文件：

```
* /common/board_r.c
* /common/console.c
* /include/console.h
* /include/common.h
* /drivers/serial/serial-uclass.c
```

---
#### 更新时间: 
`2018-05-19`

#### 负责人员：
- rex

#### 主要更新内容：
- 增加了一个新的 `u-boot` 命令 —— `bootvtos` ，用来直接启动 `vtos`
- `bootvtos` 主要做了以下几件事：
1. `ping` 服务器 `192.168.1.116`
2. 使用 `tftp` 从服务器加载 `vtos.bin` 到指定的内存地址（默认是 `0x80080000`）
3. 从指定的内存地址处（默认是 `0x80080000`）启动 `vtos` 内核

#### 主要涉及的文件：

```
* /cmd/Makefile
* /cmd/Kconfig
* /common/Makefile
* /arch/arm/cpu/armv8/cpu.c
* /arch/arm/include/asm/u-boot-arm.h
+ /cmd/bootvtos.c
+ /common/bootvtos.c
+ /include/bootvtos.h
```



