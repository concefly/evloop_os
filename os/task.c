#include "task.h"

// 检查资源占用位
static t_err check_occupy_flag(struct t_task_ctx *p_ctx) {
  unsigned long unit_occupy = ~0;
  unsigned long port_occupy = ~0;

  for (t_size i = 0; i < OS_TASK_MAX_SIZE; i++) {
    struct t_task *p_task = (p_ctx->task_array)[i];
    if (p_task == NULL) continue;

    unit_occupy &= p_task->unit_occupy;
    if (unit_occupy > 0) return OS_TASK_ERR_UNIT_OCCUPY;

    port_occupy &= p_task->port_occupy;
    if (port_occupy > 0) return OS_TASK_ERR_PORT_OCCUPY;
  }

  return 0;
}

void os_init_ctx(struct t_task_ctx *p_ctx) {
  p_ctx->loop_limit = 0;
  p_ctx->self = NULL;

  for (t_size i = 0; i < OS_TASK_MAX_SIZE; i++) {
    p_ctx->task_array[i] = NULL;
  }
}

void os_init_task(struct t_task *p_task) {
  p_task->id = 0;
  p_task->p_hit = NULL;
  p_task->p_run = NULL;
  p_task->os_status = OS_TASK_STATUS_IDLE;
  p_task->state = 0;
  p_task->unit_occupy = 0;
  p_task->port_occupy = 0;
}

void os_set_idle(struct t_task_ctx *p_ctx) {
  p_ctx->self->os_status = OS_TASK_STATUS_IDLE;
}

void os_set_run(struct t_task_ctx *p_ctx) {
  p_ctx->self->os_status = OS_TASK_STATUS_RUN;
}

t_err os_start_loop(struct t_task_ctx * p_ctx) {
  t_err errno = check_occupy_flag(p_ctx);
  if (errno) return errno;

  // 遍历 before_all
  for (t_size i = 0; i < OS_TASK_MAX_SIZE; i++) {
    struct t_task *p_task = (p_ctx->task_array)[i];
    if (p_task == NULL) continue;

    p_ctx->self = p_task;

    if (p_task->p_before_all != NULL) {
      p_task->p_before_all(p_ctx);
    }
  }

  // 开始事件循环
  for (unsigned long loop_cnt = 1; p_ctx->loop_limit == 0 ? 1 : loop_cnt <= p_ctx->loop_limit; loop_cnt++) {
    struct t_task * hit_run_list[OS_TASK_MAX_SIZE];
    t_size hit_run_cnt = 0;

    // hit 测试
    for (t_size i = 0; i < OS_TASK_MAX_SIZE; i++) {
      struct t_task *p_task = (p_ctx->task_array)[i];
      if (p_task == NULL) continue;

      p_ctx->self = p_task;

      // 只测试 IDLE 任务
      if (p_task->os_status == OS_TASK_STATUS_IDLE) {
        if (p_task->p_hit != NULL) {
          p_task->p_hit(p_ctx);

          if (p_task->os_status == OS_TASK_STATUS_RUN && p_task->p_run != NULL) {
            hit_run_list[hit_run_cnt] = p_task;
            hit_run_cnt++;
          }
        }
      }
    }

    // 没有需要 run 的任务，则继续 hit 测试
    if (hit_run_cnt <= 0) continue;

    // 执行 hit_run_list 记录下来的任务
    for (t_size i = 0; i < hit_run_cnt; i++) {
      struct t_task *p_task = hit_run_list[i];
      if (p_task == NULL) continue;

      p_ctx->self = p_task;
      p_task->p_run(p_ctx);
    }
  }

  return 0;
}

