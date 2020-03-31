#include "task.h"

t_err os_ctx_init(struct t_task_ctx *p_ctx) {
  p_ctx->loop_limit = 0;
  p_ctx->self = NULL;

  for (t_size i = 0; i < OS_TASK_MAX_SIZE; i++) {
    p_ctx->task_array[i] = NULL;
  }

  return 0;
}


t_err os_start_loop(struct t_task_ctx * p_ctx) {
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