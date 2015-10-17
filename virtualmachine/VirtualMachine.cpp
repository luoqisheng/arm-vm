//
//  VirtualMachine.cpp
//  virtualmachine
//
//  Created by Jem Tucker on 10/10/2015.
//  Copyright (c) 2015 Jem Tucker. All rights reserved.
//

#include "VirtualMachine.h"

#include "Decoder.h"
#include "operation.h"


uint32_t VirtualMachine::run(std::vector<uint32_t> instructions) {
    m_running = true;
    while (m_running) {
        show_registers();
        uint32_t instr = fetch(instructions);
        decode(instr);
        execute();
    }
    show_registers();
    return 0;
}

uint32_t VirtualMachine::fetch(std::vector<uint32_t> instructions) {
    return instructions[ *m_reg_sp++ ];
}

void VirtualMachine::decode(uint32_t instruction) {
    DecodedOperation op = decoder.decode(instruction);

    m_i = op.i;
    m_s = op.s;
    
    m_cond = op.cond;
    
    m_op_code  = op.code;
    m_op_reg_d = op.reg_d;
    m_op_reg_n = op.reg_n;
    m_op_reg_m = op.reg_m;
    m_op_reg_s = op.reg_s;
    m_imm_val  = op.imm_val;
    m_imm_rot  = op.imm_rot;
}

void VirtualMachine::execute() {
    uint32_t *op_dest = &m_registers[ m_op_reg_d ];
    
    uint32_t op_a = m_registers[ m_op_reg_n ];
    uint32_t op_b = m_registers[ m_op_reg_m ];
    
    if (m_i) {
        op_b = m_imm_val;
    }
    
    switch (m_op_code) {
        case AND:
            *op_dest = op_a & op_b;
            break;
        case EOR:
            *op_dest = op_a ^ op_b;
            break;
        case SUB:
            *op_dest = op_a - op_b;
            break;
        case RSB:
            *op_dest = op_b - op_a;
            break;
        case ADD:
            *op_dest = op_a + op_b;
            break;
        case ADC:
            *op_dest = op_a + op_b + m_carry;
            break;
        case SBC:
            *op_dest = op_a - op_b + m_carry - 1;
            break;
        case RSC:
            *op_dest = op_b - op_a + m_carry - 1;
            break;
        case TST:
            m_reg_cpsr = op_a & op_b;
            break;
        case TEQ:
            m_reg_cpsr = op_a ^ op_b;
            break;
        case CMP:
            m_reg_cpsr = op_a - op_b;
            break;
        case CMN:
            m_reg_cpsr = op_a + op_b;
            break;
        case ORR:
            *op_dest = op_a | op_b;
            break;
        case MOV:
            *op_dest = op_b;
            break;
        case BIC:
            *op_dest = op_a & (~ op_b);
            break;
        case MVN:
            *op_dest = ~ op_b;
            break;
        default:
            break;
    }
}

void VirtualMachine::show_registers() {
    printf("Registers: ");
    for (int i = 0; i < m_registers.size(); i++) {
        printf("%04x ", m_registers[i]);
    }
    printf("\n");
}



