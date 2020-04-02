#if !defined(__OS_TASK__)
#define __OS_TASK__

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef OS_TASK_MAX_SIZE
#define OS_TASK_MAX_SIZE 16
#endif

typedef unsigned char t_size;
typedef unsigned char t_err;
typedef unsigned char t_task_id;

// 错误号
#define OS_TASK_ERR_UNIT_OCCUPY 1
#define OS_TASK_ERR_PORT_OCCUPY 2

// 任务状态
#define OS_TASK_STATUS_IDLE 0
#define OS_TASK_STATUS_RUN 1

// 任务上下文
struct t_task_ctx {
  struct t_task *(task_array[OS_TASK_MAX_SIZE]);
  unsigned long loop_limit;

  struct t_task *self;
};

// 任务描述结构体
struct t_task {
  t_task_id id;

  void (*p_before_all)(struct t_task_ctx *p_ctx);
  void (*p_hit)(struct t_task_ctx *p_ctx);
  void (*p_run)(struct t_task_ctx *p_ctx);

  unsigned long unit_occupy;
  unsigned long port_occupy;

  unsigned char os_status;

  // 任务状态指针
  void *p_state;
};

extern void os_init_ctx(struct t_task_ctx *p_ctx);
extern void os_init_task(struct t_task *p_task);

extern void os_set_idle(struct t_task_ctx *p_ctx);
extern void os_set_run(struct t_task_ctx *p_ctx);

extern t_err os_start_loop(struct t_task_ctx *p_ctx);

#endif // __OS_TASK__
