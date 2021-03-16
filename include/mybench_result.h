#ifndef MYBENCH_MYBENCH_RESULT_H
#define MYBENCH_MYBENCH_RESULT_H

#include <stdint.h>
#include <vector>
#include "mybench_config.h"

class Mybench_worker;

class Mybench_result
{
public:
    Mybench_result();
    virtual ~Mybench_result();
    virtual int analyse_result();
    virtual void print_result();

    virtual void set_workers(std::vector<Mybench_worker*>* workers);
    virtual Mybench_config* get_config();
    virtual void set_config(Mybench_config* config);

private:
    std::vector<Mybench_worker*>* m_workers;
    Mybench_config*               m_config;

public:
    virtual uint64_t get_time_res_max_ms();
    virtual uint64_t get_time_res_min_ms();
    virtual uint64_t get_time_res_avg_ms();
    virtual uint64_t get_test_count();
    virtual uint64_t get_test_errors();
    virtual uint64_t get_test_qps();

private:
    uint64_t	          m_time_res_max_ms;
    uint64_t	          m_time_res_min_ms;
    uint64_t	          m_time_res_avg_ms;
    uint64_t	          m_test_count;
    uint64_t	          m_test_errors;
    uint64_t            m_test_qps;
};

#endif //MYBENCH_MYBENCH_RESULT_H
