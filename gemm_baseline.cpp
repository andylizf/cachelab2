/*
请注意，你的代码不能出现任何 int/short/char/float/double/auto 等局部变量/函数传参，我们仅允许使用 reg 定义的寄存器变量。
其中 reg 等价于一个 int。

你不能自己申请额外的内存，即不能使用 new/malloc，作为补偿我们传入了一段 buffer，大小为 BUFFER_SIZE，你可以视情况使用。

我们的数组按照 A, B, C, buffer 的顺序在内存上连续紧密排列，且 &A = 0x30000000（这是模拟的设定，不是 A 的真实地址）

具体见 trace 文件

如果你有定义常量的需求（更严谨的说法是，你想定义的是汇编层面的立即数，不应该占用寄存器），请参考下面这种方式使用宏定义来完成。
*/

#include "cachelab.h"

#define m case0_m
#define n case0_n
#define p case0_p

void gemm_case0_baseline(dtype_ptr A, dtype_ptr B, dtype_ptr C, dtype_ptr buffer) {  // allocate 0 1 2 3
    for (reg i(0); i < m; ++i) {                                                     // allocate 4
        for (reg j(0); j < p; ++j) {                                                 // allocate 5
            reg tmpc(0);                                                             // allocate 6
            for (reg k(0); k < n; ++k) {                                             // allocate 7
                reg tmpa(A[i * n + k]);                                              // allocate 8
                reg tmpb(B[k * p + j]);                                              // allocate 9
                tmpc += tmpa * tmpb;
            }  // free 9 8
            // free 7
            C[i * p + j] = tmpc;
        }  // free 6
        // free 5
    }
    // free 4
}  // free 3 2 1 0

#undef m
#undef n
#undef p

#define m case1_m
#define n case1_n
#define p case1_p

void gemm_case1_baseline(dtype_ptr A, dtype_ptr B, dtype_ptr C, dtype_ptr buffer) {
    for (reg i(0); i < m; ++i) {
        for (reg j(0); j < p; ++j) {
            reg tmpc(0);
            for (reg k(0); k < n; ++k) {
                reg tmpa(A[i * n + k]);
                reg tmpb(B[k * p + j]);
                tmpc += tmpa * tmpb;
            }
            C[i * p + j] = tmpc;
        }
    }
}

#undef m
#undef n
#undef p

#define m case2_m
#define n case2_n
#define p case2_p

void gemm_case2_baseline(dtype_ptr A, dtype_ptr B, dtype_ptr C, dtype_ptr buffer) {
    for (reg i(0); i < m; ++i) {
        for (reg j(0); j < p; ++j) {
            reg tmpc(0);
            for (reg k(0); k < n; ++k) {
                reg tmpa(A[i * n + k]);
                reg tmpb(B[k * p + j]);
                tmpc += tmpa * tmpb;
            }
            C[i * p + j] = tmpc;
        }
    }
}

#undef m
#undef n
#undef p

#define m case3_m
#define n case3_n
#define p case3_p

void gemm_case3_baseline(dtype_ptr A, dtype_ptr B, dtype_ptr C, dtype_ptr buffer) {
    for (reg i(0); i < m; ++i) {
        for (reg j(0); j < p; ++j) {
            reg tmpc(0);
            for (reg k(0); k < n; ++k) {
                reg tmpa(A[i * n + k]);
                reg tmpb(B[k * p + j]);
                tmpc += tmpa * tmpb;
            }
            C[i * p + j] = tmpc;
        }
    }
}

#undef m
#undef n
#undef p