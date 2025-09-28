#include "state_machine.h"

// 通用执行器
void state_machine_step(uint8_t* pState, const Transition* tbl, uint8_t tn, Evt evt) {
    for (uint8_t i = 0; i < tn; i++) {
        const Transition* t = &tbl[i];
        if ((*pState == t->curState || STATE_ANY == t->curState) && evt & t->evtMask) {
            *pState = t->nextState;
            if (t->action) {
                t->action();
            }
            break;
        }
    }
}
