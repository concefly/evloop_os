#include "../timer.h"
#include "../../task.h"

static void before_all(struct t_task_ctx *p_ctx) {
  // TODO
}

static void hit(struct t_task_ctx *p_ctx) {
  // TODO

  // if 溢出

  // 重启定时器
}

void os_timer_init_task(struct t_task *p_task) {
  p_task->unit_occupy = OS_UNIT_TIMER;

  p_task->p_before_all = before_all;
  p_task->p_hit = hit;
}
