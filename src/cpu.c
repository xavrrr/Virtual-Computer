// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define Byte uint8_t
#define Word uint16_t

/*
    List of all 'functions' supported by VM
    Inspired by W65C02 microprocessor

    Stars on the siz are to know which func are implemented
    1 star: the func exist but is not in the exec()
    2 stars: the func is fully imlemented

    OPCODES:
        0x09: ORA
        0x18: CLC **
        0x29: AND
        0x2A: ROL
        0x38: SEC **
        0x48: PHA
        0x49: EOR
        0x4C: JMP
        0x68: PLA
        0x69: ADC
        0x6A: ROR
        0x88: DEY **
        0x8A: TXA **
        0x90: BCC *
        0x91: TYS **
        0x92: TSY **
        0x98: TYA **
        0x9A: TXS **
        0xA0: LDY **
        0xA2: LDX **
        0xA8: TAY **
        0xA9: LDA **
        0xAA: TAX **
        0xB0: BCS **
        0xBA: TSX **
        0xC0: CPY
        0xC8: INY **
        0xC9: CMP
        0xCA: DEX **
        0xD0: BNE *
        0xE0: CPX
        0xE8: INX **
        0xE8: SBC
        0xF0: BEQ **
        0xFF: NOOP **
        0x00: HALT **
*/

typedef struct
{
    Word PC;          // Program Counter
    Byte SP;          // Stack Pointer
    Byte IR;          // Instruction Register
    Byte stack[0xFF]; // CPU Stack

    Byte A; // register A
    Byte X; // register X
    Byte Y; // register Y

    bool flags[3]; // Z C B

} CPU;

//-----COMMANDS-----

// LDA, OPCODE: 0xA9
void lda(CPU *cpu, Byte data)
{
    cpu->A = data;
    cpu->PC += 2;
}

// OPCODE: 0xA2
void ldx(CPU *cpu, Byte data)
{
    cpu->X = data;
    cpu->PC += 2;
}

// OPCODE: 0xA0
void ldy(CPU *cpu, Byte data)
{
    cpu->Y = data;
    cpu->PC += 2;
}

// OPCODE: 0xAA
void tax(CPU *cpu)
{
    cpu->X = cpu->A;
}

// OPCODE: 0xA8
void tay(CPU *cpu)
{
    cpu->X = cpu->A;
}

// OPCODE: 0x98
void tya(CPU *cpu)
{
    cpu->X = cpu->A;
}

// OPCODE: 0x8A
void txa(CPU *cpu)
{
    cpu->X = cpu->A;
}

// OPCODE: 0x9A
void txs(CPU *cpu)
{
    cpu->X = cpu->A;
}

// OPCODE: 0x91
void tys(CPU *cpu)
{
    cpu->X = cpu->A;
}

// OPCODE: 0xBA
void tsx(CPU *cpu)
{
    cpu->X = cpu->A;
}

// OPCODE: 0x92
void tsy(CPU *cpu)
{
    cpu->X = cpu->A;
}

// OPCODE: 0x18
void clc(CPU *cpu)
{
    cpu->flags[1] = 0;
    cpu->PC++;
}

// OPCODE: 0x38
void sec(CPU *cpu)
{
    cpu->flags[1] = 1;
    cpu->PC++;
}

// OPCODE: 0x88
void dey(CPU *cpu)
{
    if (cpu->Y == 0)
    {
    }
    else
    {
        cpu->Y--;
    }
    cpu->PC++;
}

// OPCODE: 0xCA
void dex(CPU *cpu)
{
    if (cpu->X == 0)
    {
    }
    else
    {
        cpu->X--;
    }
    cpu->PC++;
}

// OPCODE: 0xC8
void iny(CPU *cpu)
{
    if (cpu->Y == 0xFF)
    {
        cpu->flags[1] = 1;
    }
    else
    {
        cpu->Y++;
    }
    cpu->PC++;
}

// OPCODE: 0xE8
void inx(CPU *cpu)
{
    if (cpu->X == 0xFF)
    {
        cpu->flags[1] = 1;
    }
    else
    {
        cpu->X++;
    }
    cpu->PC++;
}

// OPCODE: 0xFF
void noop(CPU *cpu)
{
    cpu->PC++;
}

// OPCODE: 0x00
void halt(CPU *cpu)
{
    cpu->flags[2] = 1;
    printf("CPU halted.\n");
}

// OPCODE: 0xF0
void beq(CPU *cpu, Word address)
{
    if (cpu->flags[0] = 1)
    {
        cpu->PC = address;
    }
    else
    {
        cpu->PC++;
    }
}

// OPCODE: 0xD0
void bne(CPU *cpu, Word address)
{
    if (cpu->flags[0] = 0)
    {
        cpu->PC = address;
    }
    else
    {
        cpu->PC++;
    }
}

// OPCODE: 0xB0
void bcs(CPU *cpu, Word address)
{
    if (cpu->flags[1] = 1)
    {
        cpu->PC = address;
    }
    else
    {
        cpu->PC++;
    }
}

// OPCODE: 0x90
void bcc(CPU *cpu, Word address)
{
    if (cpu->flags[1] = 0)
    {
        cpu->PC = address;
    }
    else
    {
        cpu->PC++;
    }
}

//-----CPU/MEMORY FUNCTION-----
void init_cpu(CPU *cpu)
{
    // Reset Registers
    cpu->PC = 0;
    cpu->SP = 0xFF;
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;

    // Set flags
    cpu->flags[0] = 1; // Z = 1
    cpu->flags[1] = 0; // C = 0
    cpu->flags[2] = 0; // B = 0
}

void update_flags(CPU *cpu, Byte data)
{
    if (data == 0)
    {
        cpu->flags[0] = 1; // Z = 1
    }
    else if (data == 256)
    {
        cpu->flags[1] = 1; // C = 1
    }
    else
    {
        cpu->flags[0] = 0; // Z = 0
        cpu->flags[1] = 0; // C = 0
    }
}

void clear_mem(Byte *mem[])
{
    for (int i = 0; i < sizeof mem; i++)
    {
        mem[i] = 0xFF;
    }
}

Byte fetch(Word address, Byte mem[])
{
    return mem[address];
}

void compile_mem(Byte *mem[], Byte code[])
{
    for (int i = 0; i < sizeof code; i++)
    {
        mem[i] = &code[i];
    }
}

// 'COMPILER' take care of converting hex code into function
void exec(CPU *cpu, Byte *mem[])
{
    switch (cpu->IR)
    {
    case 0xFF:
        noop(cpu);
        break;

    case 0x00:
        halt(cpu);
        break;

    case 0x18:
        clc(cpu);
        break;

    case 0x38:
        sec(cpu);
        break;

    case 0x88:
        dey(cpu);
        break;

    case 0x8A:
        txa(cpu);
        break;

    case 0x91:
        tys(cpu);
        break;

    case 0x92:
        tsy(cpu);
        break;

    case 0x98:
        tya(cpu);
        break;

    case 0x9A:
        txs(cpu);
        break;

    case 0xA0:
        ldy(cpu, mem[cpu->PC + 1]);
        break;

    case 0xA2:
        ldx(cpu, mem[cpu->PC + 1]);
        break;

    case 0xA8:
        tay(cpu);
        break;

    case 0xA9:
        lda(cpu, mem[cpu->PC + 1]);
        break;

    case 0xAA:
        tax(cpu);
        break;

    case 0xBA:
        tsx(cpu);
        break;

    case 0xC8:
        iny(cpu);
        break;

    case 0xCA:
        dex(cpu);
        break;

    case 0xE8:
        inx(cpu);
        break;

        //case 0x

    case 0xF0:
        beq(cpu, cpu->PC + 1);
        break;
    }
}

//-----MAIN CODE-----
int main()
{
    Byte memory[65536];
    CPU cpu;

    Byte code[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF};

    // Initialization
    init_cpu(&cpu);
    clear_mem(&memory);
    compile_mem(&memory, code);
    clear_mem(&cpu.stack);

    while (1)
    {
        cpu.IR = fetch(cpu.PC, &code);
        exec(&cpu, &memory);
        printf("%x  ", cpu.flags[2]);
        printf("%x  ", cpu.IR);
        printf("%x\n", cpu.PC);
        if (cpu.flags[2] == 1)
        {
            break;
        }
    }

    printf("Exit code 0\n");

    return 0;
}
