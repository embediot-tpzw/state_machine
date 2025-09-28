# embeded_state_machine

A lightweight, portable, and table-driven state machine framework for embedded systems.  
一个轻量级、可移植、基于表驱动的嵌入式状态机框架。  

---

## Features
- Table-driven transitions  
- Entry actions  
- Global `STATE_ANY` transitions  
- Portable (C99, no OS dependency)  
- Suitable for MCU + RTOS  

---

## Principle
The framework is based on two core ideas:

1. **Transition Table**  
   Each row defines one rule:  
   ```
   (curState, event) → (nextState, action)
   ```

2. **Generic Executor**  
   Iterates over the table, finds a matching rule, updates the state, and runs the action.  

```c
typedef struct {
    uint8_t  curState;      // current state
    uint32_t evtMask;       // event mask
    uint8_t  nextState;     // next state
    void   (*action)(void); // entry action
} Transition;

void state_machine_step(uint8_t* pState, const Transition* tbl, uint8_t tn, uint32_t evt) {
    for (uint8_t i = 0; i < tn; i++) {
        const Transiton* t = &tbl[i];
        if ((*pState == t->curState || t->curState == STATE_ANY) && (evt & t->evtMask)) {
            *pState = t->nextState;
            if (t->action) t->action();
            break;
        }
    }
}
```

---

## Example
A simple LED controller with three states: **OFF → ON → BLINK**  

### 1. Define states and events
```c
typedef enum { STATE_ANY=0, STATE_OFF, STATE_ON, STATE_BLINK } LedState;
typedef enum { EVT_PRESS=1<<0, EVT_OFF=1<<1, EVT_ON=1<<2 } LedEvent;
```

### 2. Define actions
```c
static void entry_off(void)   { printf("LED OFF\n"); }
static void entry_on(void)    { printf("LED ON\n"); }
static void entry_blink(void) { printf("LED BLINKING\n"); }
```

### 3. Transition table
```c
static const Transiton LED_TRANS[] = {
    {STATE_OFF,   EVT_PRESS, STATE_ON,    entry_on},
    {STATE_ON,    EVT_PRESS, STATE_BLINK, entry_blink},
    {STATE_BLINK, EVT_PRESS, STATE_OFF,   entry_off},
    {STATE_ANY,   EVT_ON,    STATE_ON,    entry_on},
    {STATE_ANY,   EVT_OFF,   STATE_OFF,   entry_off},
};
```

### 4. Run example
```c
int main(void) {
    uint8_t ledState = STATE_ON;
    LedEvent evtSeq[] = {EVT_PRESS, EVT_PRESS, EVT_PRESS, EVT_OFF, EVT_ON, EVT_OFF};
    for (size_t i = 0; i < sizeof(evtSeq)/sizeof(evtSeq[0]); i++) {
        state_machine_step(&ledState, LED_TRANS, sizeof(LED_TRANS)/sizeof(LED_TRANS[0]), evtSeq[i]);
    }
    return 0;
}
```

### 5. Output
```
LED BLINKING
LED OFF
LED ON
LED OFF
LED ON
LED OFF
```

---

## State Diagram
The following diagram shows the same LED FSM as defined above:
<img width="360" height="443" alt="image" src="https://github.com/user-attachments/assets/5f491498-a54d-41bf-b4fe-fae5982da91c" />

