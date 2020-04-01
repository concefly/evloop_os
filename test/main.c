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
    os_set_run(p_ctx);
  }
}

void task_run(struct t_task_ctx * p_ctx) {
  run_invoke_cnt++;
}

void test_run_task(CuTest *tc) {
  struct t_task_ctx task_ctx;
  os_init_ctx(&task_ctx);

  task_ctx.loop_limit = 2;

  struct t_task task1;
  os_init_task(&task1);

  task1.id = 1;
  task1.p_before_all = &task_before_all;
  task1.p_hit = &task_hit;
  task1.p_run = &task_run;
  
  task_ctx.task_array[0] = &task1;

  t_err errno = os_start_loop(&task_ctx);

  CuAssertIntEquals(tc, 0, errno);
  CuAssertIntEquals(tc, 1, before_all_invoke_cnt);
  CuAssertIntEquals(tc, 2, hit_invoke_cnt);
  CuAssertIntEquals(tc, 1, run_invoke_cnt);
}

void test_check_occupy_flag(CuTest *tc) {
  struct t_task_ctx task_ctx;
  os_init_ctx(&task_ctx);

  task_ctx.loop_limit = 2;

  struct t_task task1;
  struct t_task task2;

  os_init_task(&task1);
  os_init_task(&task2);

  task1.id = 1;
  task1.unit_occupy = 1 | (1 << 1);

  task2.id = 2;
  task2.unit_occupy = (1 << 1);

  task_ctx.task_array[0] = &task1;
  task_ctx.task_array[1] = &task2;

  t_err errno = os_start_loop(&task_ctx);

  CuAssertIntEquals(tc, OS_TASK_ERR_UNIT_OCCUPY, errno);
}

int main(void) {
  CuString *output = CuStringNew();
  CuSuite* suite = CuSuiteNew();
  
  SUITE_ADD_TEST(suite, test_run_task);
  SUITE_ADD_TEST(suite, test_check_occupy_flag);
  
  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output); 
  printf("%s\n", output->buffer);
}