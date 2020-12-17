#include "mybench.h"
#include <stdio.h>
#include <vector>
#include <future>
#include "mybench_util.h"
#include "mybench_config.h"
#include "mybench_worker.h"
#include "mybench_db_type.h"
#include "mybench_result.h"
#include "mybench_test.h"
#include "concurrent/count_down_latch.h"

using namespace std;
using namespace mybench;

static Mybench_util                 *util= NULL;
static Mybench_config               *config= NULL;
static Mybench_db_type              *db_type= NULL;
static Mybench_test_manager         *test_manager= NULL;
static std::vector<Mybench_worker*> *workers;
static Mybench_result               *result= NULL;
//run conditions and locks
static std::mutex                   global_run_lock;
static std::condition_variable      global_run_cond;
static Count_down_latch             *count_down_latch= NULL;
static std::atomic<bool>            global_status(false);
//TODO need create thread by std::async(std::launch::async, fun1);?
static std::promise<void>           global_ready_promise;
static std::shared_future<void>     global_ready_future(global_ready_promise.get_future());

//callback func
void global_ready()
{
  global_ready_future.wait();
}

//test code
int load_config(int argc, char** argv)
{
  printf("begin to load config!\n");
  util = new Mybench_util();
  config= new Mybench_config();
  config->set_db_type(DB_TYPE_MYSQL);
  if(config->parse_param(argc, argv))
    return -1;
  db_type= new Mybench_db_type();
  db_type->set_config(config);
  if(db_type->initialize())
    return -1;
  printf("load config finish!\n");
  return 0;
}

int init_tests()
{
  printf("begin to init tests!\n");
  test_manager= new Mybench_test_manager();
  test_manager->set_config(config);
  //If mixed scenarios are not supported, it is not necessary to load all test cases
  test_manager->initialize();
  printf("init tests finish!\n");
}

int init_threads()
{
  printf("begin to init threads!\n");
  if(config == NULL || db_type == NULL)
    return -1;

  uint32_t thread_count= config->get_threads();
  Mybench_test* test= NULL;
  count_down_latch= new Count_down_latch();
  workers= new std::vector<Mybench_worker*>();
  Mybench_worker* worker= NULL;
  for (int i = 0; i < thread_count; i++)
  {
    test= test_manager->create_test();
    if(!test)
    {
      return -2;
    }
    count_down_latch->count_up();
    worker= new Mybench_worker();
    worker->set_config(config);
    worker->set_db_type(db_type);
    test->set_db_type(db_type);
    worker->set_test(test);
    worker->set_count_down_latch(count_down_latch);
    worker->set_global_status(&global_status);
    worker->set_global_ready_func(global_ready);
    worker->initialize();
    workers->push_back(worker);
  }

  printf("init threads finish!\n");
  return 0;
}

int run_tests()
{
  printf("begin to run tests!\n");
  //wake up all tests, begin to race
  global_status= true;
  global_ready_promise.set_value();
  printf("run tests finish!\n");
  return 0;
}

int wait_tests_finish()
{
  printf("begin to wait tests finish!\n");
  uint64_t time_sec= config->get_time_sec();
  uint64_t wait_tests_finish_interval_sec= config->get_wait_tests_finish_interval_sec();
  std::unique_lock<std::mutex> lock(global_run_lock);
  switch (config->get_test_cmd()) {
    case TEST_CMD_PREPARE:
      count_down_latch->await();
      break;
    case TEST_CMD_RUN:
      while(time_sec > 0)
      {
        global_run_cond.wait_for(lock, std::chrono::seconds(wait_tests_finish_interval_sec));
        time_sec -= wait_tests_finish_interval_sec;
      }
      //abort all tests
      global_status.store(false);
      break;
    case TEST_CMD_CLEANUP:
      count_down_latch->await();
      break;
    default:
      printf("mybench worker handle, unknown test cmd!");
  }
  printf("wait tests finish!\n");
  return 0;
}

int analyse_results()
{
  printf("begin to analyse results!\n");
  switch (config->get_test_cmd()) {
    case TEST_CMD_RUN:
      result= new Mybench_result();
      result->set_config(config);
      result->set_workers(workers);
      result->analyse_result();
      result->print_result();
      break;
    case TEST_CMD_PREPARE:
    case TEST_CMD_CLEANUP:
      break;
    default:
      printf("mybench worker handle, unknown test cmd!");
  }
  printf("analyse results finish!\n");
  return 0;
}

int stop_threads()
{
  Mybench_worker *one_worker= NULL;
  if(workers)
  {
    std::vector<Mybench_worker*>::iterator worker_iterator;
    for (worker_iterator= workers->begin();
         worker_iterator!= workers->end();
         worker_iterator++) {
      one_worker = (*worker_iterator);

      if(one_worker)
      {
        one_worker->terminate();
        delete one_worker;
        one_worker= NULL;
      }
    }
    workers->clear();
    delete workers;
    workers= NULL;
  }
  return 0;
}

int cleanup()
{
  printf("begin to cleanup mybench!\n");
  //result
  if(result)
  {
    delete result;
    result= NULL;
  }
  //workers
  stop_threads();
  //test
  if(test_manager)
  {
    delete test_manager;
    test_manager= NULL;
  }
  //db_type
  if(db_type)
  {
    db_type->terminate();
    delete db_type;
    db_type= NULL;
  }
  //config
  if(config)
  {
    delete config;
    config= NULL;
  }
  //util
  if(util)
  {
    delete util;
    util= NULL;
  }
  printf("cleanup mybench finish!\n");
  return 0;
}

int main(int argc, char** argv)
{
  int error= 0;
  printf("begin to run mybench!\n");
  //load config
  if(load_config(argc, argv))
  {
    error= -1;
  }
  //init tests
  else if(init_tests())
  {
    error= -2;
  }
  //init threads
  else if(init_threads())
  {
    error= -3;
  }
  //run tests
  else if(run_tests())
  {
    error= -4;
  }
  //wait tests finish
  else if(wait_tests_finish())
  {
    error= -5;
  }
  //collect results
  else if(analyse_results())
  {
    error= -6;
  }

  cleanup();

  printf("run mybench finish!\n");
  return error;
}
