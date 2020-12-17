#ifndef MYBENCH_MYBENCH_WORKER_H
#define MYBENCH_MYBENCH_WORKER_H

#include <mutex>
#include <condition_variable>
#include <future>
#include "mybench_config.h"
#include "mybench_db_type.h"
#include "mybench_test.h"
#include "concurrent/count_down_latch.h"

using namespace mybench;

class Mybench_worker
{
public:
    Mybench_worker();
    virtual ~Mybench_worker();
    virtual int initialize();
    virtual int terminate();
    virtual void handler();
    virtual int handler_loop();

public:
    virtual bool is_ready();
    virtual void set_is_ready(bool is_ready);
    virtual Mybench_config* get_config();
    virtual void set_config(Mybench_config* config);
    virtual Mybench_db_type* get_db_type();
    virtual void set_db_type(Mybench_db_type* db_type);
    virtual Mybench_test* get_test();
    virtual void set_test(Mybench_test* test);
    virtual Count_down_latch* get_count_down_latch();
    virtual void set_count_down_latch(Count_down_latch* count_down_latch);
    virtual void set_global_status(std::atomic<bool> *global_status);
    virtual void set_global_ready_func(void (*global_ready_param)());

private:
    std::atomic<bool>         m_ready;
    std::atomic<bool>         m_run_flag;
    std::atomic<bool>         m_worker_status;
    Mybench_config*           m_config;
    Mybench_db_type*          m_db_type;
    Mybench_test*             m_test;
    pthread_t                 m_t;

private:
    //run conditions and locks
    mutable std::mutex                          m_run_lock;
    mutable std::condition_variable             m_run_cond;
    Count_down_latch                            *m_count_down_latch;
    std::atomic<bool>                           *m_global_status;
    void (*global_ready)();

public:
    virtual uint64_t get_time_res_max_ns();
    virtual uint64_t get_time_res_min_ns();
    virtual uint64_t get_time_res_total_ns();
    virtual uint64_t get_test_count();
    virtual uint64_t get_test_errors();

/**
 * worker info
*/
private:
    uint64_t	          m_time_res_max_ns;
    uint64_t	          m_time_res_min_ns;
    uint64_t	          m_time_res_total_ns;
    uint64_t	          m_test_count;
    uint64_t	          m_test_errors;
};

#endif //MYBENCH_MYBENCH_WORKER_H
