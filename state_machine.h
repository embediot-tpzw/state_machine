#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__
#include <stdint.h>
#include <stdio.h>

typedef uint32_t Evt;
#define STATE_ANY 0

// 状态迁移表
typedef struct {
    uint8_t curState;
    uint32_t evtMask;
    uint8_t nextState;
    void (*action)(void);
} Transition;

extern void state_machine_step(uint8_t* pState, const Transition* tbl, uint8_t tn, Evt evt);

#endif
