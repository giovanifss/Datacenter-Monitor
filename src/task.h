#ifndef TASK_H
#define TASK_H

#define true 1
#define false 0

typedef struct task_t rms_task;

struct task_t {
    uint16_t deadline;
    uint16_t period;
    uint16_t cpu;
    uint16_t priority;
    // Pointer to function
};

// Update deadine

#endif /* TASK_H */
