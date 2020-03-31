#include <stdio.h>
#include <string.h>
#include "./lib/CuTest.h"
#include "../os/task.h"

unsigned int before_all_invoke_cnt = 0;
unsigned int hit_invoke_cnt = 0;
unsigned int run_invoke_cnt = 0;

void task_before_all(struct t_task_ctx * p_ctx) {
  before_all_invoke_cnt++;
}

void task_hit(struct t_task_ctx * p_ctx) {
  hit_invoke_cnt++;

  if (hit_invoke_cnt == 2) {
    p_ctx->self->os_status = OS_TASK_STATUS_RUN;
  }
}

void task_run(struct t_task_ctx * p_ctx) {
  run_invoke_cnt++;
}

void test_run_task(CuTest *tc) {
  struct t_task_ctx task_ctx;
  os_ctx_init(&task_ctx);

  task_ctx.loop_limit = 2;

  struct t_task task1;
  task1.id = 1;
  task1.p_before_all = &task_before_all;
  task1.p_hit = &task_hit;
  task1.p_run = &task_run;
  task1.os_status = OS_TASK_STATUS_IDLE;
  
  task_ctx.task_array[0] = &task1;

  t_err errno = os_start_loop(&task_ctx);

  CuAssertIntEquals(tc, 0, errno);
  CuAssertIntEquals(tc, 1, before_all_invoke_cnt);
  CuAssertIntEquals(tc, 2, hit_invoke_cnt);
  CuAssertIntEquals(tc, 1, run_invoke_cnt);
}

int main(void) {
  CuString *output = CuStringNew();
  CuSuite* suite = CuSuiteNew();
  
  SUITE_ADD_TEST(suite, test_run_task);
  
  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output); 
  printf("%s\n", output->buffer);
}