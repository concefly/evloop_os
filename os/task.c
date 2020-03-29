#include "task.h"

t_err os_task_start(struct t_task_set * p_task_set) {
  t_size task_size = p_task_set->size;

  if (task_size > OS_TASK_MAX_SIZE) return OS_ERR_TASK_CNT_OVER;

  struct t_task_ctx * p_ctx = p_task_set->p_ctx;

  // 遍历 before_all
  for (t_size i = 0; i < task_size; i++) {
    struct t_task *p_task = (p_task_set->p_list)[i];
    if (p_task->p_before_all != OS_NULL) {
      p_task->p_before_all(p_ctx, p_task);
    }
  }

  // 开始事件循环
  for (unsigned long loop_cnt = 1; p_task_set->loop_limit == 0 ? 1 : loop_cnt <= p_task_set->loop_limit; loop_cnt++) {
    struct t_task * hit_run_list[OS_TASK_MAX_SIZE];
    t_size hit_run_cnt = 0;

    p_ctx->loop_cnt = loop_cnt;

    // hit 测试
    for (t_size i = 0; i < task_size; i++) {
      struct t_task *p_task = (p_task_set->p_list)[i];

      // 只测试 IDLE 任务
      if (p_task->status == OS_TASK_STATUS_IDLE) {
        if (p_task->p_hit != OS_NULL) {
          p_task->p_hit(p_ctx, p_task);

          if (p_task->status == OS_TASK_STATUS_RUN && p_task->p_run != OS_NULL) {
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
      p_task->p_run(p_ctx, p_task);
    }
  }

  return 0;
}