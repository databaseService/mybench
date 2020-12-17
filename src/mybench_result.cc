#include "mybench_result.h"
#include "mybench_worker.h"

Mybench_result::Mybench_result()
        :m_time_res_max_ms(0),
         m_time_res_min_ms(0),
         m_time_res_avg_ms(0),
         m_test_count(0),
         m_test_errors(0),
         m_test_qps(0)
{

}

Mybench_result::~Mybench_result()
{

}

int Mybench_result::analyse_result()
{
  Mybench_worker *one_worker= NULL;
  uint64_t time_res_max_ns= 0;
  uint64_t time_res_min_ns= 0;
  uint64_t time_res_total_ns= 0;
  if(m_workers)
  {
    std::vector<Mybench_worker*>::iterator worker_iterator;
    for (worker_iterator= m_workers->begin();
         worker_iterator!= m_workers->end();
         worker_iterator++) {
      one_worker = (*worker_iterator);

      if(one_worker)
      {
        if(time_res_max_ns <= 0)
        {
          time_res_max_ns= one_worker->get_time_res_max_ns();
        } else
        {
          time_res_max_ns= std::max(time_res_max_ns, one_worker->get_time_res_max_ns());
        }

        if(time_res_min_ns <= 0)
        {
          time_res_min_ns= one_worker->get_time_res_min_ns();
        } else
        {
          time_res_min_ns= std::min(time_res_min_ns, one_worker->get_time_res_min_ns());
        }
        m_test_count += one_worker->get_test_count();
        m_test_errors += one_worker->get_test_errors();
        //TODO need optimize by math lib
        time_res_total_ns += one_worker->get_time_res_total_ns();
      }
    }
    m_time_res_max_ms= time_res_max_ns/1000/1000;
    m_time_res_min_ms= time_res_min_ns/1000/1000;
    if(m_test_count && time_res_total_ns)
    {
      m_time_res_avg_ms= time_res_total_ns/m_test_count/1000/1000;
      m_test_qps= (m_test_count * 1000 * 1000 * 1000)/time_res_total_ns;
    }
  }
  return 0;
}

void Mybench_result::print_result()
{
  get_config()->get_util()->log_info("mybench test result:");
  get_config()->get_util()->log_info("  qps=%llu", m_test_qps);
  get_config()->get_util()->log_info("  total count=%llu", m_test_count);
  get_config()->get_util()->log_info("  error count=%llu", m_test_errors);
  get_config()->get_util()->log_info("  avg response time by ms=%llu", m_time_res_avg_ms);
  get_config()->get_util()->log_info("  max response time by ms=%llu", m_time_res_max_ms);
  get_config()->get_util()->log_info("  min response time by ms=%llu", m_time_res_min_ms);
}

void Mybench_result::set_workers(std::vector<Mybench_worker*>* workers)
{
  m_workers= workers;
}

Mybench_config* Mybench_result::get_config()
{
  return m_config;
}

void Mybench_result::set_config(Mybench_config* config)
{
  m_config= config;
}

uint64_t Mybench_result::get_time_res_max_ms()
{
  return m_time_res_max_ms;
}

uint64_t Mybench_result::get_time_res_min_ms()
{
  return m_time_res_min_ms;
}

uint64_t Mybench_result::get_time_res_avg_ms()
{
  return m_time_res_avg_ms;
}

uint64_t Mybench_result::get_test_count()
{
  return m_test_count;
}

uint64_t Mybench_result::get_test_errors()
{
  return m_test_errors;
}

uint64_t Mybench_result::get_test_qps()
{
  return m_test_qps;
}
