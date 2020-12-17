#ifndef MYBENCH_MYBENCH_TEST_H
#define MYBENCH_MYBENCH_TEST_H

#include <vector>
#include "mybench_config.h"

class Mybench_db_type;

class Mybench_test
{
public:
    Mybench_test()
    :m_test_case(TEST_UNKNOWN),
    m_status(false)
    {

    }

    virtual ~Mybench_test();
    virtual int initialize();
    virtual int terminate();
    virtual int prepare();
    virtual int run();
    virtual int clean_once();
    virtual int cleanup();

public:
    virtual TEST_CASE get_test_case();
    virtual void set_test_case(TEST_CASE test_case);
    virtual bool get_status();
    virtual void set_status(bool status);
    virtual Mybench_db_type* get_db_type();
    virtual void set_db_type(Mybench_db_type* db_type);
    virtual Mybench_config* get_config();
    virtual void set_config(Mybench_config* config);

private:
    TEST_CASE        m_test_case;
    bool             m_status;
    Mybench_db_type  *m_db_type;
    Mybench_config   *m_config;
};

class Mybench_test_manager
{
public:
    Mybench_test_manager();
    virtual ~Mybench_test_manager();
    virtual int initialize();
    virtual Mybench_test* create_test();

    virtual Mybench_config* get_config();
    virtual void set_config(Mybench_config* config);

private:
    virtual int register_test(Mybench_test* test);
    virtual void clean_tests();

private:
    std::vector<Mybench_test*> m_tests;
    Mybench_config* m_config;
};

#endif //MYBENCH_MYBENCH_TEST_H
