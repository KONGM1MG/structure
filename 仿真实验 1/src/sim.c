#include <stdio.h>
#include "shell.h"
#include "sim.h"
void process_instruction()
{
    uint32_t instr = mem_read_32(CURRENT_STATE.PC);// 从内存中读取指令
    uint8_t opcode = instr >> 26;// 取出opcode I-type R-type J-type
    uint8_t rs = (instr >> 21) & 0x1f;// 取出rs I-type R-type
    uint8_t rt = (instr >> 16) & 0x1f;// 取出rt I-type R-type
    uint8_t rd = (instr >> 11) & 0x1f;// 取出rd R-type
    uint8_t shamt = (instr >> 6) & 0x1f;// 取出shamt R-type
    uint8_t funct = instr & 0x3f;// 取出funct R-type
    uint16_t imm = instr & 0xffff;// 取出imm I-type
    uint32_t addr = instr & 0x3ffffff;// 取出addr J-type

    if(instr == 0)
    {
        // NOP
    }
    else if(opcode==0)
    {
        R_type(rs, rt, rd, shamt, funct);
    }
    else if(opcode==2 || opcode==3)
    {
        J_type(opcode, addr);
    }
    else
    {
        I_type(opcode, rs, rt, imm);
    }
    NEXT_STATE.PC += 4;// PC+4

}

 // 0xC syscall

void R_type(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct)
{
    // 包括了所有的R-type指令
    if(funct == 0xc)
    {
        syscall();
        return;
    }
    switch (funct)
    {
        // 23 条
        case 0x20: add(rs, rt, rd); break;
        case 0x21: addu(rs, rt, rd); break;
        case 0x22: sub(rs, rt, rd); break;
        case 0x23: subu(rs, rt, rd); break;

        case 0x18: mult(rs, rt); break;
        case 0x19: multu(rs, rt); break;
        case 0x1a: div(rs, rt); break;
        case 0x1b: divu(rs, rt); break;

        case 0x10: mfhi(rd); break;
        case 0x12: mflo(rd); break;
        case 0x11: mthi(rs); break;
        case 0x13: mtlo(rs); break;

        case 0x24: AND(rs, rt, rd); break;
        case 0x25: OR(rs, rt, rd); break;
        case 0x26: XOR(rs, rt, rd); break;
        case 0x27: NOR(rs, rt, rd); break;

        case 0x00: sll(rt, rd, shamt); break;
        case 0x04: sllv(rs, rt, rd); break;
        case 0x02: srl(rt, rd, shamt); break;
        case 0x06: srlv(rs, rt, rd); break;
        case 0x03: sra(rt, rd, shamt); break;
        case 0x07: srav(rs, rt, rd); break;

        case 0x2a: slt(rs, rt, rd); break;
        case 0x2b: sltu(rs, rt, rd); break;

        case 0x8 : jr(rs); break;
        case 0x9 : jalr(rs, rt); break;
        default: break;
    }
}

void I_type(uint8_t opcode, uint8_t rs, uint8_t rt, int16_t imm)
{
    // 包括了所有的I-type指令
    switch (opcode)
    {
        // 18 条
        case 0x08: addi(rs, rt, imm); break;
        case 0x09: addiu(rs, rt, imm); break;

        case 0x0a: slti(rs, rt, imm); break;
        case 0x0b: sltiu(rs, rt, imm); break;

        case 0x0c: ANDi(rs, rt, imm); break;
        case 0x0d: ORi(rs, rt, imm); break;
        case 0x0e: XORi(rs, rt, imm); break;
        case 0x0f: LUI(rt, imm); break;

        case 0x20: lb(rs, rt, imm); break;
        case 0x21: lh(rs, rt, imm); break;
        case 0x23: lw(rs, rt, imm); break;
        case 0x24: lbu(rs, rt, imm); break;
        case 0x25: lhu(rs, rt, imm); break;
        case 0x28: sb(rs, rt, imm); break;
        case 0x29: sh(rs, rt, imm); break;
        case 0x2b: sw(rs, rt, imm); break;

        case 0x04: beq(rs, rt, imm); break;
        case 0x05: bne(rs, rt, imm); break;
        case 0x06: blez(rs, imm); break;
        case 0x07: bgtz(rs, imm); break;
	case 0x01:
	{
		switch (rt)
		{
			case 0x00 : bltz(rs, imm); break;
			case 0x01 : bgez(rs, imm); break;
			case 0x10: bltzal(rs, imm); break;
			case 0x11: bgezal(rs, imm); break;  
		}
	}
	break;
    }
}

void J_type(uint8_t opcode, uint32_t addr)
{
    // 包括了所有的J-type指令
    switch (opcode)
    {
        // 2 条
        case 0x02: j(addr); break;
        case 0x03: jal(addr); break;

    }
}


// R-type
// 1
void add(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
    }
}
// 2
void addu(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if(rd != 0) // 避免写入$zero
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
    }
}
// 3
void sub(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
    }
}
// 4
void subu(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
    }
}
// 5
void mult(uint8_t rs, uint8_t rt)
{
    /*
    通用寄存器rs和rt的内容被视为32位二进制补码值进行相乘。
    在任何情况下都不会发生整数溢出异常。
    在64位模式下，操作数必须是有效的32位符号扩展值。
    当操作完成时，双精度结果的低位字会加载到特殊寄存器LO中，
    双精度结果的高位字会加载到特殊寄存器HI中。
    */
    int64_t temp = (int64_t)CURRENT_STATE.REGS[rs] * (int64_t)CURRENT_STATE.REGS[rt];
    NEXT_STATE.LO = temp & 0xffffffff;
    NEXT_STATE.HI = (temp >> 32) & 0xffffffff;
}
// 6
void multu(uint8_t rs, uint8_t rt)
{
    /*
    通用寄存器rs和通用寄存器rt的内容被视为无符号值进行相乘，
    在任何情况下都不会发生溢出异常。
    在64位模式下，操作数必须是有效的32位符号扩展值。
    当操作完成时，双精度结果的低位字加载到特殊寄存器LO中，
    双精度结果的高位字加载到特殊寄存器HI中。
    */ 
    uint64_t temp = (uint64_t)CURRENT_STATE.REGS[rs] * (uint64_t)CURRENT_STATE.REGS[rt];
    NEXT_STATE.LO = temp & 0xffffffff;
    NEXT_STATE.HI = (temp >> 32) & 0xffffffff;
}
// 7
void div(uint8_t rs, uint8_t rt)
{
    /*
    在64位模式下，操作数必须是有效的符号扩展的32位值。
    通常，这个指令后面会跟随其他指令，用于检查零除数和溢出。
    当操作完成时，双精度结果的商字会加载到特殊寄存器LO中，
    余数字会加载到特殊寄存器HI中。
    */
    if(CURRENT_STATE.REGS[rt] != 0)
    {
        NEXT_STATE.LO = (int32_t) CURRENT_STATE.REGS[rs] / (int32_t)CURRENT_STATE.REGS[rt];
        NEXT_STATE.HI = (int32_t) CURRENT_STATE.REGS[rs] % (int32_t)CURRENT_STATE.REGS[rt];
    }
}
// 8
void divu(uint8_t rs, uint8_t rt)
{
    /*
    当操作完成时，双精度结果的商字会加载到特殊寄存器LO中，
    余数字会加载到特残寄存器HI中。
    */
    if(CURRENT_STATE.REGS[rt] != 0)
    {
        NEXT_STATE.LO = (uint32_t)CURRENT_STATE.REGS[rs] / (uint32_t)CURRENT_STATE.REGS[rt];
        NEXT_STATE.HI = (uint32_t)CURRENT_STATE.REGS[rs] % (uint32_t)CURRENT_STATE.REGS[rt];
    }
}
// 9
void mfhi(uint8_t rd)
{
    /*
    从特殊寄存器HI中读取双精度结果的高位字，
    并将其加载到通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
    }
}
// 10
void mflo(uint8_t rd)
{
    /*
    从特殊寄存器LO中读取双精度结果的低位字，
    并将其加载到通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
    }
}
// 11
void mthi(uint8_t rs)
{
    /*
    将通用寄存器rs的内容加载到特殊寄存器HI中。
    */
    NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
}
// 12
void mtlo(uint8_t rs)
{
    /*
    将通用寄存器rs的内容加载到特殊寄存器LO中。
    */
    NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
}
// 13
void AND(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rs和通用寄存器rt的内容视为32位二进制补码值，
    并将其逐位进行逻辑与运算。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
    }
}
// 14
void OR(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rs和通用寄存器rt的内容视为32位二进制补码值，
    并将其逐位进行逻辑或运算。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
    }
}
// 15
void XOR(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rs和通用寄存器rt的内容视为32位二进制补码值，
    并将其逐位进行逻辑异或运算。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
    }
}
// 16
void NOR(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rs和通用寄存器rt的内容视为32位二进制补码值，
    并将其逐位进行逻辑或运算，然后对结果取反。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
    }
}
// 17
void sll(uint8_t rt, uint8_t rd, uint8_t shamt)
{
    /*
    将通用寄存器rt的内容视为32位二进制补码值，
    并将其逻辑左移shamt位。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
    }
}
// 18
void sllv(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rt的内容视为32位二进制补码值，
    并将其逻辑左移通用寄存器rs的低5位。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << (CURRENT_STATE.REGS[rs] & 0x1f);
    }
}
// 19
void srl(uint8_t rt, uint8_t rd, uint8_t shamt)
{
    /*
    将通用寄存器rt的内容视为32位二进制补码值，
    并将其逻辑右移shamt位。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
    }
}
// 20
void srlv(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rt的内容视为32位二进制补码值，
    并将其逻辑右移通用寄存器rs的低5位。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x1f);
    }
}
// 21
void sra(uint8_t rt, uint8_t rd, uint8_t shamt)
{
    /*
    将通用寄存器rt的内容视为32位二进制补码值，
    并将其算术右移shamt位。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> shamt;
    }
}
// 22
void srav(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rt的内容视为32位二进制补码值，
    并将其算术右移通用寄存器rs的低5位。结果存储在通用寄存器rd中。
    */
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x1f);
    }
}
// 23
void slt(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rs和通用寄存器rt的内容视为32位二进制补码值，
    并将其进行有符号比较。如果rs < rt，则将通用寄存器rd设置为1，否则设置为0。
    */
    if(rd != 0)
    {
        if((int32_t)CURRENT_STATE.REGS[rs] < (int32_t)CURRENT_STATE.REGS[rt])
        {
            NEXT_STATE.REGS[rd] = 1;
        }
        else
        {
            NEXT_STATE.REGS[rd] = 0;
        }
    }
}
// 24
void sltu(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    将通用寄存器rs和通用寄存器rt的内容视为32位二进制补码值，
    并将其进行无符号比较。如果rs < rt，则将通用寄存器rd设置为1，否则设置为0。
    */
    if(rd != 0)
    {
        if(CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt])
        {
            NEXT_STATE.REGS[rd] = 1;
        }
        else
        {
            NEXT_STATE.REGS[rd] = 0;
        }
    }
}

// I-type
// 25
void addi(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    将符号扩展的16位立即数视为32位二进制补码值，
    并将其相加。结果存储在通用寄存器rt中。
    */
    if(rt != 0)
    {
        NEXT_STATE.REGS[rt] = (int32_t) CURRENT_STATE.REGS[rs] + imm;
    }
}
// 26
void addiu(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    将符号扩展的16位立即数视为32位二进制补码值，
    并将其相加。结果存储在通用寄存器rt中。
    */
    if(rt != 0)
    {
        NEXT_STATE.REGS[rt] = (int32_t) CURRENT_STATE.REGS[rs] + imm;
    }
}
// 27
void slti(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    将符号扩展的16位立即数视为32位二进制补码值，
    并将其进行有符号比较。如果rs < imm，则将通用寄存器rt设置为1，否则设置为0。
    */
    if(rt != 0)
    {
        if((int32_t)CURRENT_STATE.REGS[rs] < (int32_t)imm)
        {
            NEXT_STATE.REGS[rt] = 1;
        }
        else
        {
            NEXT_STATE.REGS[rt] = 0;
        }
    }
}
// 28
void sltiu(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    将符号扩展的16位立即数视为32位二进制补码值，
    并将其进行无符号比较。如果rs < imm，则将通用寄存器rt设置为1，否则设置为0。
    */
    if(rt != 0)
    {
        if(CURRENT_STATE.REGS[rs] < imm)
        {
            NEXT_STATE.REGS[rt] = 1;
        }
        else
        {
            NEXT_STATE.REGS[rt] = 0;
        }
    }
}
// 29
void ANDi(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    将符号扩展的16位立即数视为32位二进制补码值，
    并将其逐位进行逻辑与运算。结果存储在通用寄存器rt中。
    */
    if(rt != 0)
    {
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & (uint32_t)imm;
    }
}
// 30
void ORi(uint8_t rs, uint8_t rt, int16_t imm)
{
    if (rt != 0)
    {
        // 零扩展 16 位立即数并与 rs 寄存器内容进行逻辑 OR 操作
        int32_t result = (int32_t)CURRENT_STATE.REGS[rs] | (int32_t)(imm & 0xFFFF);
        // 存放结果到 rt 寄存器
        NEXT_STATE.REGS[rt] = result;
    }
}
// 31
void XORi(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    将符号扩展的16位立即数视为32位二进制补码值，
    并将其逐位进行逻辑异或运算。结果存储在通用寄存器rt中。
    */
    if(rt != 0)
    {
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ (uint32_t)imm;
    }
}
// 32
void LUI(uint8_t rt, int16_t imm)
{
    /*
    将符号扩展的16位立即数视为32位二进制补码值，
    并将其左移16位。结果存储在通用寄存器rt中。
    */
    if(rt != 0)
    {
        NEXT_STATE.REGS[rt] = imm << 16;
    }
}
// 33
void lb(uint8_t rs, uint8_t rt, int16_t imm)
{
    if (rt != 0)
    {
        // 符号扩展 16 位偏移量并与 rs 寄存器的内容相加，形成虚拟地址
        int32_t addr = (int32_t)CURRENT_STATE.REGS[rs] + (int32_t)imm;

        // 从内存中读取指定有效地址处的字节内容并符号扩展，然后加载到 rt 寄存器中
        int32_t data = (int32_t)mem_read_32(addr);
        int32_t sign_extended_data = (data << 24) >> 24; // 符号扩展到32位
        NEXT_STATE.REGS[rt] = sign_extended_data;
    }
}

// 34
void lh(uint8_t rs, uint8_t rt, int16_t imm)
{
	if(rt != 0)
	{
	// 符号扩展 16 位偏移量并与 rs 寄存器的内容相加，形成虚拟地址
        int32_t addr = (int32_t)CURRENT_STATE.REGS[rs] + (int32_t)imm;

        // 检查有效地址的最低位是否为非零，如果是则引发地址错误异常
        if (addr & 0x1)
        {
        }
        else
        {
        // 从内存中读取指定有效地址处的半字内容并符号扩展，然后加载到 rt 寄存器中
            int32_t data = (int32_t)mem_read_32(addr);
            int32_t sign_extended_data = (data << 16) >> 16; // 符号扩展到32位
            NEXT_STATE.REGS[rt] = sign_extended_data;
        }
	}
}
// 35
void lw(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为有效的32位地址，
    并将其加上符号扩展的16位立即数视为32位二进制补码值，
    以字为单位从内存中读取一个字。结果存储在通用寄存器rt中。
    */
    if(rt != 0)
    {
        uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
	NEXT_STATE.REGS[rt] = mem_read_32(addr);
    }
}
// 36
void lbu(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为有效的32位地址，
    并将其加上符号扩展的16位立即数视为32位二进制补码值，
    以字节为单位从内存中读取一个字节。结果的低8位存储在通用寄存器rt中，
    其余位设置为0。
    */
    if(rt != 0)
    {
	uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
	NEXT_STATE.REGS[rt] = mem_read_32(addr) & 0xff;
    }
}
// 37
void lhu(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为有效的32位地址，
    并将其加上符号扩展的16位立即数视为32位二进制补码值，
    以半字为单位从内存中读取一个半字。结果的低16位存储在通用寄存器rt中，
    其余位设置为0。
    */
    if(rt != 0)
    {
        uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
	    NEXT_STATE.REGS[rt] = mem_read_32(addr) & 0xffff;
    }
}
// 38
void sb(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rt的内容视为有效的32位地址，
    并将其加上符号扩展的16位立即数视为32位二进制补码值，
    将通用寄存器rs的低8位存储到内存中。
    */
    uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
    uint32_t data = mem_read_32(addr) & ~0xff;
    data = data | (CURRENT_STATE.REGS[rt] & 0xff);
    mem_write_32(addr, data);    
}
// 39
void sh(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rt的内容视为有效的32位地址，
    并将其加上符号扩展的16位立即数视为32位二进制补码值，
    将通用寄存器rs的低16位存储到内存中。
    */
    uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
    uint32_t data = mem_read_32(addr) & ~0xffff;
    data = data | (CURRENT_STATE.REGS[rt] & 0xffff);
    mem_write_32(addr, data);
}
// 40
void sw(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rt的内容视为有效的32位地址，
    并将其加上符号扩展的16位立即数视为32位二进制补码值，
    将通用寄存器rs的内容存储到内存中。
    */
    uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
    mem_write_32(addr, CURRENT_STATE.REGS[rt]);
}
// 41
void beq(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs和通用寄存器rt的内容视为32位二进制补码值，
    并将其进行有符号比较。如果rs = rt，则将PC设置为PC + 4 + (imm << 2)，
    否则PC设置为PC + 4。
    */
if(CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt])
{
NEXT_STATE.PC = CURRENT_STATE.PC + (imm<<2);
}	
}
// 42
void bne(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    将通用寄存器rs和通用寄存器rt的内容视为32位二进制补码值，
    并将其进行有符号比较。如果rs != rt，则将PC设置为PC + 4 + (imm << 2)，
    否则PC设置为PC + 4。
    */
    if(CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt])
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
    }
}
// 43
void blez(uint8_t rs, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    并将其进行有符号比较。如果rs <= 0，则将PC设置为PC + 4 + (imm << 2)，
    否则PC设置为PC + 4。
    */
    if((int32_t)CURRENT_STATE.REGS[rs] <= 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
    }

}
// 44
void bltz(uint8_t rs, int16_t imm)
{
    /*
    分支目标地址是根据延迟槽中指令的地址与16位偏移的和计算而得，
    然后左移两位并进行符号扩展。
    如果通用寄存器rs的内容的符号位设置为1，
    那么程序将跳转到目标地址，并延迟执行一条指令。
    */
    if((int32_t)CURRENT_STATE.REGS[rs] < 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
    }
}
// 45
void bgez(uint8_t rs, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    并将其进行有符号比较。如果rs >= 0，则将PC设置为PC + 4 + (imm << 2)，
    否则PC设置为PC + 4。
    */
    if((int32_t)CURRENT_STATE.REGS[rs] >= 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
    }
}

void bgtz(uint8_t rs, int16_t imm) 
{
	if((int32_t) CURRENT_STATE.REGS[rs] > 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm<<2);
    }
}


// 46
void bltzal(uint8_t rs, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    并将其进行有符号比较。如果rs < 0，则将PC设置为PC + 4 + (imm << 2)，
    否则PC设置为PC + 4。将PC + 8的值存储在通用寄存器31中。
    */
    if((int32_t) CURRENT_STATE.REGS[rs] < 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    }

}
// 47
void bgezal(uint8_t rs, int16_t imm)
{
    /*
    将通用寄存器rs的内容视为32位二进制补码值，
    并将其进行有符号比较。如果rs >= 0，则将PC设置为PC + 4 + (imm << 2)，
    否则PC设置为PC + 4。将PC + 8的值存储在通用寄存器31中。
    */
    if((int32_t)CURRENT_STATE.REGS[rs] >= 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    }
}


// J-Type
// 48
void j(uint32_t addr)
{
    /*
    将PC的高4位设置为PC + 4的高4位，
    将PC的低28位设置为addr的低28位。
    */
    NEXT_STATE.PC = (addr << 2) - 4;
}
// 49
void jal(uint32_t addr)
{
    /*
    将PC的高4位设置为PC + 4的高4位，
    将PC的低28位设置为addr的低28位。
    将PC + 8的值存储在通用寄存器31中。
    */
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    NEXT_STATE.PC = (addr << 2) - 4;
}
// 50
void jr(uint8_t rs)
{
    /*
    将通用寄存器rs的内容视为有效的32位地址，
    并将其设置为PC的值。
    */
    NEXT_STATE.PC = CURRENT_STATE.REGS[rs] - 4;
}
// 51
void jalr(uint8_t rs, uint8_t rd)
{
    uint8_t temp = rd == 0 ? 31 : rd;
    NEXT_STATE.REGS[temp] = CURRENT_STATE.PC + 4;
    NEXT_STATE.PC = CURRENT_STATE.REGS[rs] - 4;
}

// Syscall
// 52
void syscall() 
{
    /*当执行SYSCALL指令时，如果寄存器$v0（寄存器2）的值为0x0A（十进制10），
    那么go命令应该停止其模拟循环并返回到模拟器外壳的提示符。
    如果$v0具有任何其他值，则该指令不应产生任何效果。*/ 
    if(CURRENT_STATE.REGS[2] == 0x0a)
    {
        RUN_BIT = 0;
    }
}


