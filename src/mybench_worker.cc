#include "mybench_worker.h"
#include <thread>
#include <chrono>
#include <assert.h>
#include "mybench_config.h"

using SteadyClock = std::chrono::steady_clock;
using SteadyTime = SteadyClock::time_point;

static void *launch_worker_handler_thread(void* arg)
{
  Mybench_worker *worker= (Mybench_worker*) arg;
  worker->handler();
  return 0;
}

Mybench_worker::Mybench_worker()
:m_ready(false),
 m_run_flag(false),
 m_worker_status(false),
 m_time_res_max_ns(0),
 m_time_res_min_ns(0),
 m_time_res_total_ns(0),
 m_test_count(0),
 m_test_errors(0)
{
  m_global_status= NULL;
  global_ready= NULL;
}

Mybench_worker::~Mybench_worker()
{

}

int Mybench_worker::initialize()
{
  int error= 0;

  m_config->get_util()->log_info("Initializing test worker for test case=%s!", m_test->get_test_case());

  std::unique_lock<std::mutex> lock(m_run_lock);

  if(m_ready)
  {
    error= -1;
    m_config->get_util()->log_error("Initializing test worker, "
           "the worker has been initialized or initializing, skip the action for test case=%s!",
           m_test->get_test_case());
  }else{
    error= pthread_create(&m_t,
                          NULL,
                          launch_worker_handler_thread,
                          (void*)this);
    if(error)
    {
      m_config->get_util()->log_error("Initializing test worker, create thread error=%d...", error);
    } else
    {
      while (!m_run_flag)
      {
        m_run_cond.wait(lock);
      }
      m_ready= true;
      m_config->get_util()->log_info("Initialized test worker...");
    }
  }
  return error;
}

int Mybench_worker::terminate()
{
  int error= 0;
  std::unique_lock<std::mutex> lock(m_run_lock);
  if(!m_ready)
  {
    error= -1;
    m_config->get_util()->log_error("terminating test worker, "
           "the worker has been terminated or not inited, skip the action for test case=%s!",
           m_test->get_test_case());
  } else{
    m_config->get_util()->log_info("terminating test worker for test case=%s!", m_test->get_test_case());
    m_ready= false;
    m_run_flag= false;

    while(m_worker_status)
    {
      m_run_cond.wait_for(lock, std::chrono::seconds(2));
    }
    //1s
    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_config->get_util()->log_info("terminated test worker!");
  }

  return error;
}

void Mybench_worker::handler()
{
  m_run_lock.lock();
  m_run_flag= true;
  m_worker_status= true;
  m_run_cond.notify_all();
  m_run_lock.unlock();

  m_config->get_util()->log_info("begin to work for test case=%s!", m_test->get_test_case());

  //begin to race
  global_ready();

  assert(m_test);

  m_test->initialize();

  while (m_run_flag && m_global_status && m_global_status->load()){
    if(handler_loop())
    {
      break;
    }
  }
  m_test->terminate();

  m_config->get_util()->log_info("terminated worker for test case=%s!", m_test->get_test_case());

  m_run_lock.lock();
  m_worker_status= false;
  m_run_cond.notify_all();
  m_run_lock.unlock();
}

int Mybench_worker::handler_loop()
{
  int result= 0;
  int run_result= 0;
  SteadyTime started_at;
  SteadyTime updated_at;
  SteadyClock::duration duration_ns;
  uint64_t time_res_now_ns= 0;
  //test
  switch (m_config->get_test_cmd()) {
    case TEST_CMD_PREPARE:
      m_config->get_util()->log_info("mybench worker handler, prepare for test case=%s!",
                                     m_test->get_test_case());
      m_test->prepare();
      m_count_down_latch->count_down();
      result= 1;
      break;
    case TEST_CMD_RUN:
      started_at= SteadyClock::now();
      run_result= m_test->run();
      updated_at= SteadyClock::now();
      duration_ns= updated_at - started_at;

      time_res_now_ns= duration_ns.count();
      m_time_res_max_ns= std::max(m_time_res_max_ns, time_res_now_ns);
      if(m_time_res_min_ns <= 0)
      {
        m_time_res_min_ns= time_res_now_ns;
      }else
      {
        m_time_res_min_ns= std::min(m_time_res_min_ns, time_res_now_ns);
      }
      m_time_res_total_ns += time_res_now_ns;
      ++m_test_count;
      if(run_result)
      {
        ++m_test_errors;
      }
      m_test->clean_once();
      break;
    case TEST_CMD_CLEANUP:
      m_config->get_util()->log_info("mybench worker handler, cleanup for test case=%s!",
                                     m_test->get_test_case());
      m_test->cleanup();
      m_count_down_latch->count_down();
      result= 2;
      break;
    default:
      result= -1;
      m_config->get_util()->log_error("mybench worker handler, unknown test cmd!");
  }
  return result;
}

bool Mybench_worker::is_ready()
{
  return m_ready;
}

void Mybench_worker::set_is_ready(bool is_ready)
{
  m_ready= is_ready;
}

Mybench_config* Mybench_worker::get_config()
{
  return m_config;
}

void Mybench_worker::set_config(Mybench_config* config)
{
  m_config= config;
}

Mybench_db_type* Mybench_worker::get_db_type()
{
  return m_db_type;
}

void Mybench_worker::set_db_type(Mybench_db_type* db_type)
{
  m_db_type= db_type;
}

Mybench_test* Mybench_worker::get_test()
{
  return m_test;
}

void Mybench_worker::set_test(Mybench_test* test)
{
  m_test= test;
}

uint64_t Mybench_worker::get_time_res_max_ns()
{
  return m_time_res_max_ns;
}

uint64_t Mybench_worker::get_time_res_min_ns()
{
  return m_time_res_min_ns;
}

uint64_t Mybench_worker::get_time_res_total_ns()
{
  return m_time_res_total_ns;
}

uint64_t Mybench_worker::get_test_count()
{
  return m_test_count;
}

uint64_t Mybench_worker::get_test_errors()
{
  return m_test_errors;
}

Count_down_latch* Mybench_worker::get_count_down_latch()
{
  return m_count_down_latch;
}

void Mybench_worker::set_count_down_latch(Count_down_latch* count_down_latch)
{
  m_count_down_latch= count_down_latch;
}

void Mybench_worker::set_global_status(std::atomic<bool> *global_status)
{
  m_global_status->store(*global_status);
}

void Mybench_worker::set_global_ready_func(void (*global_ready_param)())
{
  global_ready= global_ready_param;
}
