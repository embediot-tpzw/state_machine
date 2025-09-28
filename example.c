#include "state_machine.h"

// 事件定义
typedef enum {
    EVT_PRESS = (1u << 0),
    EVT_OFF = (1u << 1),
    EVT_ON = (1u << 2),
} EvtBitMask;

// 状态定义
typedef enum {
    STATE_OFF = 1,
    STATE_ON,
    STATE_BLINK,
} LedState;

static void entry_off(void) {
    printf("LED OFF\n");
}
static void entry_on(void) {
    printf("LED ON\n");
}
static void entry_blink(void) {
    printf("LED BLINKING\n");
}

// 迁移表
static const Transition LED_TRANS[] = {
    {STATE_OFF, EVT_PRESS, STATE_ON, entry_on},
    {STATE_ON, EVT_PRESS, STATE_BLINK, entry_blink},
    {STATE_BLINK, EVT_PRESS, STATE_OFF, entry_off},
    {STATE_ANY, EVT_ON, STATE_ON, entry_on},
    {STATE_ANY, EVT_OFF, STATE_OFF, entry_off},
};

int main(void) {
    uint8_t ledState = STATE_ON;
    Evt evtSeq[] = {EVT_PRESS, EVT_PRESS, EVT_PRESS, EVT_OFF, EVT_ON, EVT_OFF};
    size_t n = sizeof(evtSeq) / sizeof(evtSeq[0]);
    for (size_t i = 0; i < n; i++) {
        state_machine_step(&ledState, LED_TRANS, sizeof(LED_TRANS) / sizeof(LED_TRANS[0]), evtSeq[i]);
    }
    return 0;
}
