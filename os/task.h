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

// unit
#define OS_UNIT_TIMER 1

// port
#define OS_PORT_1 1
#define OS_PORT_2 (1 << 1)
#define OS_PORT_3 (1 << 2)
#define OS_PORT_4 (1 << 3)
#define OS_PORT_5 (1 << 4)
#define OS_PORT_6 (1 << 5)
#define OS_PORT_7 (1 << 6)
#define OS_PORT_8 (1 << 7)

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
};

extern void os_init_ctx(struct t_task_ctx *p_ctx);
extern void os_init_task(struct t_task *p_task);

extern void os_set_idle(struct t_task_ctx *p_ctx);
extern void os_set_run(struct t_task_ctx *p_ctx);

extern t_err os_start_loop(struct t_task_ctx *p_ctx);

#endif // __OS_TASK__
