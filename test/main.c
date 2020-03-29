#include <stdio.h>
#include <string.h>
#include "./lib/CuTest.h"
#include "../os/task.h"

unsigned int before_all_invoke_cnt = 0;
unsigned int hit_invoke_cnt = 0;
unsigned int run_invoke_cnt = 0;

void task_before_all(struct t_task_ctx * p_ctx, struct t_task *self) {
  before_all_invoke_cnt++;
}

void task_hit(struct t_task_ctx * p_ctx, struct t_task *self) {
  hit_invoke_cnt++;

  if (hit_invoke_cnt == 2) {
    self->status = OS_TASK_STATUS_RUN;
  }
}

void task_run(struct t_task_ctx * p_ctx, struct t_task *self) {
  run_invoke_cnt++;
}

void test_run_task(CuTest *tc) {
  struct t_task_ctx task_ctx;
  struct t_task_set task_set;

  task_set.p_ctx = &task_ctx;

  struct t_task task1;
  task1.id = 1;
  task1.p_before_all = &task_before_all;
  task1.p_hit = &task_hit;
  task1.p_run = &task_run;
  task1.status = OS_TASK_STATUS_IDLE;
  
  struct t_task * task_list[1];
  task_list[0] = &task1;
  
  task_set.p_list = task_list;
  task_set.size = 1;
  task_set.loop_limit = 2;

  t_err errno = os_task_start(&task_set);

  CuAssertIntEquals(tc, 0, errno);
  CuAssertIntEquals(tc, task_set.p_ctx->loop_cnt, 2);

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