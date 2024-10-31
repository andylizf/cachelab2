# CacheLab2

```
 ________  ________  ________  ___  ___  _______   ___       ________  ________    _______     
|\   ____\|\   __  \|\   ____\|\  \|\  \|\  ___ \ |\  \     |\   __  \|\   __  \  /  ___  \    
\ \  \___|\ \  \|\  \ \  \___|\ \  \\\  \ \   __/|\ \  \    \ \  \|\  \ \  \|\ /_/__/|_/  /|   
 \ \  \    \ \   __  \ \  \    \ \   __  \ \  \_|/_\ \  \    \ \   __  \ \   __  \__|//  / /   
  \ \  \____\ \  \ \  \ \  \____\ \  \ \  \ \  \_|\ \ \  \____\ \  \ \  \ \  \|\  \  /  /_/__  
   \ \_______\ \__\ \__\ \_______\ \__\ \__\ \_______\ \_______\ \__\ \__\ \_______\|\________\
    \|_______|\|__|\|__|\|_______|\|__|\|__|\|_______|\|_______|\|__|\|__|\|_______| \|_______|
```

<!-- https://www.patorjk.com/software/taag/#p=display&f=3D-ASCII&t=cachelab -->

中国人民大学（RUC） CacheLab2（改编自 CMU CacheLab）

模板与问题反馈仓库：[CacheLab2](https://github.com/RUCICS/cachelab2)

## 阅前须知

- 请确保你已经阅读了 [everything-you-should-know](https://github.com/RUCICS/everything-you-should-know)，尤其是其中的 [How To Ask](https://github.com/RUCICS/everything-you-should-know/blob/main/ask/README.md) 的问题。
- 本实验相关的问题请提交到**模板仓库**的 [ISSUE](https://github.com/RUCICS/cachelab2/issues) 中，请在提交问题时检查其他 issue 是否有一样的问题，并反馈到已有的 issue 下，注意 GitHub 默认不显示已经 close 的问题，请注意修改 Filters 查找所有 issue。如果确有必要，助教会在 issue 中请求你加助教的微信解决，但即使你的问题需要加微信解决，**也烦请先用 issue 和助教取得联系，以确保其他人可以确认/解决相同的问题**。
- 此实验提供的报告非常详细，除了本文档，你还需要在完成第二部分实验前仔细阅读 [`demo.cpp`](./demo.cpp)，这是第二部分的文档。依照经验，会有 50% 以上的问题是不读文档导致的，提出报告中解答了的问题可能会给助教留下不太好的印象。

> 模板仓库即 [https://github.com/RUCICS/cachelab2](https://github.com/RUCICS/cachelab2)，不要反馈到自己的作业仓库里了，我们收不到提醒

## 项目编译与环境指南

本实验对系统的要求如下：

| System | Support |
| ------ | ------- |
| Ubuntu 20.04 LTS | ✔️ |
| Ubuntu 22.04 LTS | ✔️ |
| Ubuntu 24.04 LTS | ✔️ |

典型场景是用 WSL2 进行开发，如果你不具有这个环境，比如你是 Mac 用户，请访问 [https://ics.ruc.panjd.net](https://ics.ruc.panjd.net) 获取服务器登陆信息，在服务器上完成本实验。

> 助教不负责解决其他操作系统遇到的问题。

### 排行榜提交前的准备工作

本实验的 Part B 部分，我们会采用~~紧张刺激的~~打榜的形式来计算分数，见 [https://cachelab.ruc.panjd.net](https://cachelab.ruc.panjd.net)。在你开始本实验前，请先做好如下配置：

1. 访问 [https://ics.ruc.panjd.net](https://ics.ruc.panjd.net) 获取提交密钥，由于短时间访问流量过大可能导致学生身份认证失败，请尝试等待一段时间后再试，或者重启电脑，清空 cookies，以无痕模式访问等
2. Clone 本仓库
3. 运行本仓库下的 `submit_gemm.sh` 脚本，即执行 `./submit_gemm.sh`
4. 按要求输入提交密钥（access_key）
    ```bash
    Access key file is empty. Please enter the access key:
    ```
5. 如果提交成功，你应该会看到终端输出：
    ```bash
    Name: 张三, Student ID: 2000000000, 提交成功，请刷新 https://cachelab.ruc.panjd.net 查看测试结果
    ```

这个命令的本质是把该文件夹下的 `gemm.cpp` 文件和访问密钥发送到我们的服务器上，我们会在后台运行你的代码，然后给你一个分数。

我们会在文件夹下的 `.access_key` 文件中缓存你的提交密钥，请不要误将该密钥分享给别人导致你的分数受到影响，我们的密钥不能重置。

关于排行榜的计分规则，我们会在后续的章节中详细说明。

### 如何编译

```bash
make -j
```

## 问题背景

2022年的冬天，小民在写完 CacheLab 后直呼，这作业也太简单了，网上随便抄抄就满分了。

转眼来到了 2024 年，为了尽量避免小明这样的漏网之鱼，RUC CacheLab 2 诞生了。

本实验基于以下现实问题：

CPU 在工作时不能直接用内存中的数据计算，而是通过一定的层次结构层层传递到 CPU 内部再传出，具体来说，内存中的值会先加载到 cache 里，再从 cache 加载到寄存器 register 里，CPU 用寄存器完成计算后把计算结果写回 cache，再写回内存。

为了提升这个过程的性能，所以我们有至少两种优化思路：

- 减少内存读写：即通过访问模式设计，减少 cache miss，尽量复用 cache
- 减少 cache 读写：即通过访问模式设计，减少去读写 cache，尽量复用寄存器，为了进一步让你们理解这点，你们可以仔细阅读 [缓存和寄存器性能](#缓存和寄存器性能)

## 问题简述与提示

本实验共有两个部分。在第一部分 Part A 中，你需要实现一个 cache simulator，用来统计内存缓存的命中次数等（不涉及寄存器开销）。第二部分，你需要实现一个矩阵乘法算子，并最优化其缓存和寄存器性能（寄存器的开销会由我们的判分程序替你统计）。

### 缓存和寄存器性能

缓存性能顾名思义，就是减少 cache miss 的次数，这在课上已经详细讨论过，这里不再赘述。

与以往 CacheLab 不同的是，本实验显式地加入了对寄存器性能的考量，让我们用以下例子说明：

考虑两个 $2 \times 2$ 矩阵相乘的场景，假设此时所有数据都在 cache 里，我们只关注寄存器层面的问题。

#### Solution 1

```c
for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
}
```

这是一个典型的矩阵乘法代码，如果直接执行，它的汇编指令类似这样：

```
load A[0][0] to reg1
load B[0][0] to reg2
    calculate reg3 = reg1 * reg2
load C[0][0] to reg4
    calculate reg4 = reg4 + reg3
store reg4 to C[0][0]
...
```

一共需要执行 8 次这样的操作，意味着 24 次 load 和 8 次 store。

#### Solution 2

为了优化，我们显式地声明寄存器来规定代码的行为。为了方便，我们假设 reg 就是一个寄存器类型。

```c
for (reg i = 0; i < 2; ++i) {
    for (reg j = 0; j < 2; ++j) {
        reg tmpc = 0;
        for (reg k = 0; k < 2; ++k) {
            reg tmpa = A[i][k];
            reg tmpb = B[k][j];
            tmpc += tmpa * tmpb;
        }
        C[i][j] = tmpc;
    }
}
```

此时经过优化后的汇编指令类似这样：

```c
    calculate tmpc = 0
load A[0][0] to tmpa
load B[0][0] to tmpb
    calculate tmpa = tmpa * tmpb
    calculate tmpc = tmpc + tmpa
store tmpc to C[0][0]
...
```

这样的操作执行 8 遍，最后只需要执行 16 次 load 和 4 次 store，减少了大量的内存/cache访问，减少的部分是由于我们只读和写了 `C[i][j]` 一次，而不像上一个解法那样每次都重新立刻读写。

#### Solution 3

我们还可以优化，注意到数据实际上只有 8 个 int 要读入，我们却使用了 16 次 load，一个暴力的优化方法是，全部加载到寄存器，计算后再导出：

```c
reg tmpa[2][2];
reg tmpb[2][2];
reg tmpc[2][2];
for (reg i = 0; i < 2; ++i) {
    for (reg j = 0; j < 2; ++j) {
        tmpc[i][j] = 0;
        tmpa[i][j] = A[i][j];
        tmpb[i][j] = B[i][j];
    }
}
for (reg i = 0; i < 2; ++i) {
    for (reg j = 0; j < 2; ++j) {
        for (reg k = 0; k < 2; ++k) {
            tmpc[i][j] += tmpa[i][k] * tmpb[k][j];
        }
    }
}
for (reg i = 0; i < 2; ++i) {
    for (reg j = 0; j < 2; ++j) {
        C[i][j] = tmpc[i][j];
    }
}
```

这样我们达到了理论上的最优性能，只需要 8 次 load 和 4 次 store。代价是我们需要大量的寄存器，而 CPU 上的寄存器是有限的，我们会在问题描述中详细规定硬件配置。

> 以上的二维 `reg tmpa[2][2]` 仅仅是演示方便，本实验提供的实际框架不支持这种写法，只能写成一维的。

## 问题描述

### Part A：缓存模拟器

我们提供了生成内存访问记录的框架，

比如运行朴素实现的 $2 \times 2 \times 2$ 矩阵乘法：

```
./main case0
```

它会输出 trace 文件，形如：

```
 L 0x30000000,4 8
 L 0x30000004,4 9
 L 0x30000001,4 8
 L 0x30000006,4 9
 S 0x30000008,4 6
...
 S 0x3000000b,4 6
```

其中：
- L 和 S 分别代表 load 和 store。
- 后面的数字代表地址（16进制），如 0x3000000b，但是，请兼容 3000000b 这样的输入。注意对于 `scanf` 来说，这是自动支持的，请自行查阅 `%lx`。我们保证地址空间是 32 位的。
- 逗号后的数字代表访问的内存大小（byte记），如 1，2，4，8，我们保证大小小于等于 8。
- 最后一个数字代表的是与这行操作相关的寄存器，这个数字为 -1 时代表立即数，比如 `C[0] = 0`。这个信息其实不影响你 Part A 的实现。

这一部分你需要做的是补全 `csim.c` 文件，使其能够正确地模拟 LRU 替换策略的 cache 的行为，计算 cache 的命中次数（hits）、缺失次数（misses）、替换次数（evictions）。

其中 evictions 是指当前 cache miss，且当前的 cache 已满，我们需要做一次 LRU 策略的替换，如果此时 cache 未满，只要存入 cache 即可，就不会导致 evictions 增加。

更具体地来说，程序结束后你需要调用我们提供的 `printSummary` 函数输出这三个值。作为参考，我们提供了 `csim-ref` 程序，在你正确完成本任务后，你的结果应该和它完全一致。cache 的硬件配置通过命令行参数给出，关于怎么在 C 语言中处理命令行参数，你可以询问 AI。

命令行参数规定如下：

```
Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>
Options:
  -h         Print this help message.
  -v         Optional verbose flag.
  -s <num>   Number of set index bits.
  -E <num>   Number of lines per set.
  -b <num>   Number of block offset bits.
  -t <file>  Trace file.

Examples:
  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace
  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace
```

完成后，我们会将你的 csim 和助教提供的 csim-ref 的运行结果进行比对，测试文件在 `traces` 文件夹下，你需要保证输出一致。

![cache](./imgs/cache.jpg)

#### 实现要求

- 编译器为 gcc，请用 C 语言实现
- 你需要保证你的代码支持任意合理的 s，E，b 参数
- 你需要至少支持 traces 文件夹下的各种样例，样例之外的情况我们不做考虑
- 你需要保证你的代码没有任何编译警告
- 尽量使用 `.clang-format` 提供的格式化规则来格式化你的代码
- `-v` 参数是可选的，我们不进行测试，实现了也不会加分
- 我们对实现方式不做要求，只要最后的结果正确即可

#### 测试与分数

我们假设你的系统中安装有 python3（通常如此），那么你只需要

```bash
python3 test/csim_test.py

# 非 linux 用户，你可以这样测试，我们会仅执行你的 csim，你需要对照下文来检查结果是否正确
# 或者 push 到 Github Classroom 里，由 autograding 来测试 
python3 test/csim_test.py --no_linux 
```

得到这样的输出代表你的实现是正确的：

```bash
status | trace_file         | (s, E, b) | ref: (hits, misses, evictions) | handin: (hits, misses, evictions)
-------+--------------------+-----------+--------------------------------+----------------------------------
OK     | traces/yi2.trace   | (5, 1, 5) | (15, 1, 0)                     | (15, 1, 0)                       
OK     | traces/yi.trace    | (5, 1, 5) | (3, 4, 0)                      | (3, 4, 0)                        
OK     | traces/dave.trace  | (5, 1, 5) | (2, 3, 0)                      | (2, 3, 0)                        
OK     | traces/trans.trace | (5, 1, 5) | (211, 7, 0)                    | (211, 7, 0)                      
OK     | traces/long.trace  | (5, 1, 5) | (246213, 21775, 21743)         | (246213, 21775, 21743)           
OK     | traces/yi2.trace   | (2, 4, 3) | (14, 2, 0)                     | (14, 2, 0)                       
OK     | traces/yi.trace    | (2, 4, 3) | (2, 5, 0)                      | (2, 5, 0)                        
OK     | traces/dave.trace  | (2, 4, 3) | (0, 5, 0)                      | (0, 5, 0)                        
OK     | traces/trans.trace | (2, 4, 3) | (192, 26, 10)                  | (192, 26, 10)                    
OK     | traces/long.trace  | (2, 4, 3) | (243398, 24590, 24574)         | (243398, 24590, 24574)           
OK     | traces/yi2.trace   | (4, 2, 4) | (15, 1, 0)                     | (15, 1, 0)                       
OK     | traces/yi.trace    | (4, 2, 4) | (2, 5, 2)                      | (2, 5, 2)                        
OK     | traces/dave.trace  | (4, 2, 4) | (2, 3, 0)                      | (2, 3, 0)                        
OK     | traces/trans.trace | (4, 2, 4) | (206, 12, 0)                   | (206, 12, 0)                     
OK     | traces/long.trace  | (4, 2, 4) | (247163, 20825, 20793)         | (247163, 20825, 20793)           
OK     | traces/yi2.trace   | (1, 1, 1) | (8, 8, 6)                      | (8, 8, 6)                        
OK     | traces/yi.trace    | (1, 1, 1) | (0, 7, 5)                      | (0, 7, 5)                        
OK     | traces/dave.trace  | (1, 1, 1) | (0, 5, 4)                      | (0, 5, 4)                        
OK     | traces/trans.trace | (1, 1, 1) | (25, 193, 192)                 | (25, 193, 192)                   
OK     | traces/long.trace  | (1, 1, 1) | (35393, 232595, 232594)        | (35393, 232595, 232594)          

Score: 100.00
```

我们的分数为 $100 \times 正确比例^{0.25}$

### Part B：矩阵乘法优化

我们在 [缓存和寄存器性能](#缓存和寄存器性能) 中提到了如何优化矩阵乘法的寄存器性能，现在你需要再考虑上 cache，基于我们提供的框架，将这些优化实现进 [`gemm.cpp`](./gemm.cpp) 里，目前 [`gemm.cpp`](./gemm.cpp) 只提供了最朴素的算法用作演示。

我们假设的硬件环境是：

- cache：LRU 替换策略，s = 4，E = 1，b = 5
- CPU：一共 32 个寄存器，我们的框架会在你使用超过 32 个寄存器时抛出运行时错误
- 读写一次内存需要 15 个单位时间
- 读写一次 cache 需要 1 个单位时间

你们需要优化的只有 case1，case2，case3 三种情况，分别对应

- case1: $16 \times 16 \times 16$ 矩阵乘法
- case2: $32 \times 32 \times 32$ 矩阵乘法
- case3: $31 \times 37 \times 31$ 矩阵乘法

你可以运行 `./main case1` 来查看你的算法执行 case1 的矩阵乘法时对应的 trace 文件。

运行 `make case1` 会帮你把这份 trace 文件保存到 `gemm_traces/case1.trace` 下，并调用 `csim-ref` 来计算对应的 cache 行为。

不难看出，其中 misses 就代表了你的算法读写内存的次数，记作 $miss_{cache}$。同时，trace 文件的行数就代表了你读写 cache 的次数，记作 $miss_{reg}$。

我们的实验要求你最优化内存和 cache 的访问时间，即最优化 $latency = 15miss_{cache} + miss_{reg}$。

#### 框架说明

[`demo.cpp`](./demo.cpp) 是本框架的详细文档，请在读完本文档后详细阅读 [`demo.cpp`](./demo.cpp) 了解更多用法。

寄存器通常是汇编层面的代码才可以控制的，为了避免各位同学直接手搓汇编，我们提供了一个框架，其中定义了两种新的类型：

- `dtype_ptr`：内存指针类型，你可以视作 `int*`，其会占用一个寄存器，支持绝大多数指针类型支持的操作，但是会在你们尝试直接操作内存时报错，比如你不能写 `A[0] * B[0]`，因为 CPU 不能直接访问内存，你需要先存到寄存器里。
- `reg`：寄存器类型，你可以视作 `int`，其会占用一个寄存器，支持绝大多数 int 支持的操作。

我们要求你们在 `gemm.cpp` 中只使用这两种类型，不出现任何的 `int` 之类的类型。

比如

```cpp
void gemm_case1(dtype_ptr A, dtype_ptr B, dtype_ptr C, dtype_ptr buffer) {
    for (reg i = 0; i < m; ++i) {
        for (reg j = 0; j < p; ++j) {
            reg tmpc = 0;
            for (reg k = 0; k < n; ++k) {
                reg tmpa = A[i * n + k];
                reg tmpb = B[k * p + j];
                tmpc += tmpa * tmpb;
            }
            C[i * p + j] = tmpc;
        }
    }
}
```

注意，我们提供了一段 `buffer` 内存，该段内存的大小为 256 byte。你们可以使用这段内存来存储一些中间结果，作为我们要求中不允许自己申请内存的补偿，但是请注意，这段内存是有限的，而且也会产生 cache miss，你们需要合理利用。

本框架是用 `C++` 编写的，但是 `gemm.cpp` 部分不需要掌握 `C++` 的特性，你们可以当作 `C` 代码来写，特别之处在于我们引入了两个特殊的类型，他们有一些操作限制。特别地，如果你想要调试输出我们引入的两个类型，请使用 `C++` 中的 `std::cout`，具体见 [`demo.cpp`](./demo.cpp)。

再次强调，具体哪些操作是允许的请见 [`demo.cpp`](./demo.cpp)，本框架规定了可使用操作的上限，但可能有未发现的应该禁止的行为，请各位在使用时遵从“内存和内存不能直接操作，必须经过寄存器”的原则自行判断你是否绕过了框架限制。

#### 实现要求

- 编译器为 g++，允许用 C++ 语法
- 你需要保证你的代码没有任何编译警告
- 只能修改 `gemm.cpp` 文件
- 不能使用除了 `dtype_ptr` 和 `reg` 外的任何类型
- 不能申请任何内存，如果你需要内存来暂存东西，而不是寄存器，你可以使用 `buffer` 字段
- 禁止利用任何二进制技巧在一个寄存器/内存上，存储多个数值
- 禁止使用复杂度低于 $O(n^3)$ 的矩阵乘法算法，比如 Strassen 算法，你应该专注于 cache 的优化
- 严格按照框架的假设来设计代码，不允许绕过框架或者改变框架，即不允许利用漏洞，注意，这意味着请再次仔细阅读 [`demo.cpp`](./demo.cpp)，彻底理解这个框架
- 你的代码需要通过我们提供的在线测试（比如排行榜），而不是仅仅本地环境能通过
- 尽量使用 `.clang-format` 提供的格式化规则来格式化你的代码

#### 测试与分数

> 如果你还没完成 [排行榜提交前的准备工作](#排行榜提交前的准备工作)，请立刻回头先完成这个。

运行以下命令可以在本地获取详细的性能报告，同时自动上传 `gemm.cpp` 到排行榜上，自动上传有 30 秒的间隔，如果你想强制上传，请用上传脚本 `./submit_gemm.sh` 上传。

```bash
python3 test/gemm_test.py

# 对于非 linux 用户，我们会用你的 csim 替代 csim-ref 来完成本测试
# 你需要保证你的 csim 是正确的，否则这一节的结果将没有意义
python3 test/gemm_test.py --no_linux
```

```bash
./submit_gemm.sh
./submit_gemm.sh public
# 如果你恰巧无法访问校园网，我们提供了公网访问：https://pubcachelab.ruc.panjd.net:54398/，你可以用 ./submit_gemm.sh public 来上传
```

请注意排行榜上仅保留最优提交，请在提交报告时注意提交对应的版本（分数一致即可），不一致会被当作作弊严肃处理。

预计在 ddl 前访问流量会特别大，可预见地系统可能出现崩溃或相应极慢的问题，请不要集中到最后几天上传排行榜，以免影响你的成绩，恕不解决因流量过大导致的各类问题。

性能报告会指出你实现的 gemm 算法的 $miss_{cache}$、 $miss_{reg}$ 和 $latency$，以及这个 $latency$ 和最初 naive 实现的版本相比的加速比 $speedup$，最终这三份 $speedup$ 会以加权的形式得到一个综合加速比，我们的性能打分会~~怀着人道主义关怀~~参考这个综合加速比在实验结束后确定。

其中不同 case 的加速比对应的加权权重为

| case | 权重 |
| ---- | ---- |
| case0 | 0% |
| case1 | 30% |
| case2 | 30% |
| case3 | 40% |

> 上机课上，我们会解释对各位同学实现效果的预期和对应的打分。我们可以保证得分肯定是边际效应递减的。

> 作为一个小测试，我们会在上机课上演示优化 case0 的寄存器性能以达到理论最优，所以我们的排行榜上包括 case0 的加速比，但这不会包括在最终得分里。

#### 提示

- 优化难度视你采用的方法，不一定是从 case1 到 case3 递增的。
- 分块是一个缓解 cache 冲突的好办法：https://csapp.cs.cmu.edu/public/waside/waside-blocking.pdf。
- CMU cachelab 中优化的算法是矩阵转置，我们的是矩阵乘法，但是除了分块，可能还有其他相同的优化策略可以用。
- [缓存和寄存器性能](#缓存和寄存器性能) 章节中我们已经提示了寄存器的优化方法。
- 寄存器可能不够用，这时候你可以暂存到 buffer 字段里，你可以评估一下这是否值得。
- 注意到 A，B，C，buffer 的排列是紧密的，所以 buffer 的另外一个用途是调整矩阵的 offset，当我们的访问量是 $O(n^3)$ 时，用 $O(n^2)$ 来搬迁数据可能是值得的。
- 虽然不同测试点的权重不同，但他们优化后能取得的加速比也不同，全力优化权重高的测试点不一定能在最后加权时取得最好的成绩。
- 请一定看看我们的[后记](#真实的故事背景与优化提示)。
- 注意优化 miss_cache 和 miss_reg 的平衡，例如为了减少 miss_reg 而增加一些 miss_cache 可能是值得的。
- 在实际的工业场合，对于矩阵乘法优化问题，相较于理论分析，我们常常是穷举超参数，比如分块的大小来确定最优的参数，所以当你发现问题过于复杂时不妨尝试穷举。换言之，你可以思考不同的“大致方案”，然后跑一下看看结果。而不必强求分析出某个具体方案再实施。
- 你可以使用助教写的 [`parabuild`](https://github.com/panjd123/parabuild) 项目进行性能调优，他的用法和场景需要你结合自己的需求探索。

## 提交，报告与总分

### 提交

我们使用 Github Classroom 来发布作业并收集提交，你需要及时 push 你完成后的作业。其中 Github Classroom 自动打的分数仅供助教观察完成情况，和最终分数无关。

### 报告

具体要求见 [report/report.md](./report/report.md)。在完成报告前，你可能需要复习我们的[总体要求](./report/README.md)。

### 总分

| Part | 分数 | 说明 |
| ---- | ---- | ---- |
| Part A | 40 | 分数参考实现正确性和报告 |
| Part B | 40 | 分数参考性能和报告 | 
| 代码风格 | 20 | 代码风格和可读性 |

## 后记

### 真实的故事背景与优化提示

为了方便，我们的故事背景使用 CPU 来展开，但其实主流架构的 CPU 上并没有那么多寄存器，
我们的题目其实是受到了 GPU 的全局内存和共享内存的启发，除了网络上大量的 CacheLab 资料，
想要了解的同学还可以参考以下内容来帮助你们优化实现：

- [深入浅出GPU优化系列：GEMM优化（一）](https://zhuanlan.zhihu.com/p/435908830)

还有一点，我们的寄存器数量其实是远远给多了，考虑到你可以把寄存器换出或者是循环展开，你需要的寄存器数量应该小于 24 个寄存器。

但是为了各位把重心放在 cache 的优化上，我们给了溢出的寄存器数量避免你们把精力花在无关的地方。

### 计算机系统中访问的真实延迟

作为一个有趣的补充，内存访问需要 cache 访问 15 倍并不是随手遍的，而是根据以下这个表格得出的：

```
Latency Comparison Numbers (~2012)
----------------------------------
L1 cache reference                           0.5 ns
Branch mispredict                            5   ns
L2 cache reference                           7   ns                      14x L1 cache
Mutex lock/unlock                           25   ns
Main memory reference                      100   ns                      20x L2 cache, 200x L1 cache
Compress 1K bytes with Zippy             3,000   ns        3 us
Send 1K bytes over 1 Gbps network       10,000   ns       10 us
Read 4K randomly from SSD*             150,000   ns      150 us          ~1GB/sec SSD
Read 1 MB sequentially from memory     250,000   ns      250 us
Round trip within same datacenter      500,000   ns      500 us
Read 1 MB sequentially from SSD*     1,000,000   ns    1,000 us    1 ms  ~1GB/sec SSD, 4X memory
Disk seek                           10,000,000   ns   10,000 us   10 ms  20x datacenter roundtrip
Read 1 MB sequentially from disk    20,000,000   ns   20,000 us   20 ms  80x memory, 20X SSD
Send packet CA->Netherlands->CA    150,000,000   ns  150,000 us  150 ms

Notes
-----
1 ns = 10^-9 seconds
1 us = 10^-6 seconds = 1,000 ns
1 ms = 10^-3 seconds = 1,000 us = 1,000,000 ns

Credit
------
By Jeff Dean:               http://research.google.com/people/jeff/
Originally by Peter Norvig: http://norvig.com/21-days.html#answers

Contributions
-------------
'Humanized' comparison:  https://gist.github.com/hellerbarde/2843375
Visual comparison chart: http://i.imgur.com/k0t1e.png
```

这个表格最早出自谷歌首席科学家 Jeff Dean 在 2012 年左右给 Google 全体工程人员的演讲，这个表格展示了计算机系统中各种操作的延迟，其中 L1 和 L2 大约差了 15 倍，L2 和主存又相差了 15 倍，我们的实验其实是模拟两个相邻的计算机层次，所以我们的题目也选择了 15 倍。

当然，随着计算机发展，表格中的数据也在变化，你可以搜索 "Latency Numbers Every Programmer Should Know" 来找到相关的讨论，或者访问[这个有趣的网站](https://colin-scott.github.io/personal_website/research/interactive_latency.html)。

## 附录

### `explict` 关键字

在 C++ 中，`explicit` 关键字用来修饰只有一个参数的构造函数，表示该构造函数是显式的，不能用于隐式转换。比如：

```cpp
class A {
public:
    explicit A(int a) : a(a) {}
    int value;
};

void func(A a) {
    std::cout << a.value << std::endl;
}

int main() {
    A a = 1; // error
    A a(1); // ok
    func(1); // error
    func(A(1)); // ok
}
```

而如果你删去 `explicit` 关键字，那么 `A a = 1;` 和 `func(1);` 就会编译通过，
这是因为编译器会自动调用 `A(int a)` 构造函数。

在我们的作业中，有些同学可能想要非常精细地控制寄存器，不想要编译器帮你自动调用构造函数，帮你把内存转成寄存器，我们为这些同学提供了一个宏。

在 [cachelab.h](./cachelab.h) 文件中，你可以去掉第二行的注释，这样我们会尽量使用 `explicit` 关键字。

```cpp
#pragma once
// #define EXPLICIT
#include "common.h"
#include "gemm.h"
#include "matrix.h"
#include "test_case.h"
```

注意，我们在示例的 [gemm.cpp](./gemm.cpp) 里使用了条件编译，如果你定义了 `EXPLICIT`，
我们会用 [gemm.cpp](./gemm.cpp) 下部的代码，请在使用时注意，比如你可以删掉条件编译，或者修改下半部分的代码。

我们的打分服务器依旧会使用不带 `explicit` 关键字的版本，因为如果你的代码没有使用隐式构造，那么即使我们允许隐式构造，你的代码行为也应该是确定的。
