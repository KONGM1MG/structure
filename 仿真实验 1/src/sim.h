#pragma once
#include<stdio.h>
#include<stdint.h>

void process_instruction();

void R_type(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct);
void I_type(uint8_t opcode, uint8_t rs, uint8_t rt, int16_t imm);
void J_type(uint8_t opcode, uint32_t addr);

// R-type
void add(uint8_t rs, uint8_t rt, uint8_t rd);   // 有符号加法
void addu(uint8_t rs, uint8_t rt, uint8_t rd);  // 无符号加法
void sub(uint8_t rs, uint8_t rt, uint8_t rd);   // 有符号减法
void subu(uint8_t rs, uint8_t rt, uint8_t rd);  // 无符号减法
void mult(uint8_t rs, uint8_t rt);            // 有符号乘法
void multu(uint8_t rs, uint8_t rt);          // 无符号乘法
void div(uint8_t rs, uint8_t rt);          // 有符号除法
void divu(uint8_t rs, uint8_t rt);      // 无符号除法
void mfhi(uint8_t rd);                  // 读取HI寄存器
void mflo(uint8_t rd);                  // 读取LO寄存器
void mthi(uint8_t rs);                  // 写入HI寄存器
void mtlo(uint8_t rs);                  // 写入LO寄存器
void AND(uint8_t rs, uint8_t rt, uint8_t rd);   // 与
void OR(uint8_t rs, uint8_t rt, uint8_t rd);    // 或
void XOR(uint8_t rs, uint8_t rt, uint8_t rd);   // 异或
void NOR(uint8_t rs, uint8_t rt, uint8_t rd);   // 非或
void sll(uint8_t rt, uint8_t rd, uint8_t shamt); // 逻辑左移
void sllv(uint8_t rs, uint8_t rt, uint8_t rd);  // 逻辑左移变量
void srl(uint8_t rt, uint8_t rd, uint8_t shamt); // 逻辑右移
void srlv(uint8_t rs, uint8_t rt, uint8_t rd);  // 逻辑右移变量
void sra(uint8_t rt, uint8_t rd, uint8_t shamt); // 算术右移
void srav(uint8_t rs, uint8_t rt, uint8_t rd);  // 算术右移变量
void slt(uint8_t rs, uint8_t rt, uint8_t rd);   // 有符号比较小于
void sltu(uint8_t rs, uint8_t rt, uint8_t rd);  // 无符号比较小于

// I-type
void addi(uint8_t rs, uint8_t rt, int16_t imm);   // 有符号立即数加法
void addiu(uint8_t rs, uint8_t rt, int16_t imm);  // 无符号立即数加法
void slti(uint8_t rs, uint8_t rt, int16_t imm);   // 有符号立即数比较小于
void sltiu(uint8_t rs, uint8_t rt, int16_t imm);  // 无符号立即数比较小于
void ANDi(uint8_t rs, uint8_t rt, int16_t imm);   // 与立即数
void ORi(uint8_t rs, uint8_t rt, int16_t imm);    // 或立即数
void XORi(uint8_t rs, uint8_t rt, int16_t imm);   // 异或立即数
void LUI(uint8_t rt, int16_t imm);                // 加载高位立即数
void lb(uint8_t rs, uint8_t rt, int16_t imm);     // 有符号字节加载
void lh(uint8_t rs, uint8_t rt, int16_t imm);     // 有符号半字加载
void lw(uint8_t rs, uint8_t rt, int16_t imm);     // 有符号字加载
void lbu(uint8_t rs, uint8_t rt, int16_t imm);    // 无符号字节加载
void lhu(uint8_t rs, uint8_t rt, int16_t imm);    // 无符号半字加载
void sb(uint8_t rs, uint8_t rt, int16_t imm);     // 字节存储
void sh(uint8_t rs, uint8_t rt, int16_t imm);     // 半字存储
void sw(uint8_t rs, uint8_t rt, int16_t imm);     // 字存储
void bltz(uint8_t rs, int16_t imm);               // 分支小于零
void bgez(uint8_t rs, int16_t imm);               // 分支大于等于零
void bltzal(uint8_t rs, int16_t imm);             // 分支小于零并链接
void bgezal(uint8_t rs, int16_t imm);             // 分支大于等于零并链接
void beq(uint8_t rs, uint8_t rt, int16_t imm);    // 分支相等
void bne(uint8_t rs, uint8_t rt, int16_t imm);    // 分支不相等
void blez(uint8_t rs, int16_t imm);               // 分支小于等于零
void bgtz(uint8_t rs, int16_t imm);               // 分支大于零

// J-type
void j(uint32_t addr);      // 跳转
void jal(uint32_t addr);    // 跳转并链接
void jr(uint8_t rs);        // 跳转寄存器
void jalr(uint8_t rs, uint8_t rd);  // 跳转寄存器并链接

void syscall();             // 系统调用






