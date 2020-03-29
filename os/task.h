#if !defined(__OS_TASK__)
#define __OS_TASK__

#define OS_NULL ((void *)0)

typedef unsigned char t_size;

// 错误号
typedef unsigned char t_err;

// 注册任务数过多
#define OS_ERR_TASK_CNT_OVER 1

// 任务 id
typedef unsigned char t_task_id;

// 最大任务数
#define OS_TASK_MAX_SIZE 16

// 任务状态
#define OS_TASK_STATUS_IDLE 0
#define OS_TASK_STATUS_RUN 1

// 任务上下文
struct t_task_ctx
{
  unsigned long loop_cnt;
  // TODO: 任务间共享内存
};

// 任务描述结构体
struct t_task
{
  t_task_id id;

  void (*p_before_all)(struct t_task_ctx * p_ctx, struct t_task *self);
  void (*p_hit)(struct t_task_ctx * p_ctx, struct t_task *self);
  void (*p_run)(struct t_task_ctx * p_ctx, struct t_task *self);
  
  // 任务状态
  unsigned char status;
};

// 任务集合配置
struct t_task_set
{
  struct t_task_ctx * p_ctx;
  struct t_task *(*p_list);
  unsigned char size;
  unsigned long loop_limit;
};


// 任务 API
extern t_err os_task_start(struct t_task_set * p_task_set);

#endif // __OS_TASK__
