#if !defined(__OS_TASK__)
#define __OS_TASK__

// 任务开始前的回调
typedef void (*t_pBeforeAll)(struct TaskCtx * pTaskCtx);

// 任务 loop 的回调
typedef void (*t_pLoop)(struct TaskCtx * pTaskCtx);

// 任务结束后的回调
typedef void (*t_pAfterAll)(struct TaskCtx * pTaskCtx);

// 任务描述结构体
struct Task
{
  t_pBeforeAll *pBeforeAll;
  t_pLoop *pLoop;
  t_pAfterAll *pAfterAll;
};

#endif // __OS_TASK__
