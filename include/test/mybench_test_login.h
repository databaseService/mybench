#ifndef MYBENCH_MYBENCH_TEST_LOGIN_H
#define MYBENCH_MYBENCH_TEST_LOGIN_H

#include "mybench_test.h"

class Mybench_test_login : public Mybench_test
{
public:
    Mybench_test_login();
    virtual ~Mybench_test_login();
    virtual int initialize();
    virtual int terminate();
    virtual int prepare();
    virtual int run();
    virtual int clean_once();
    virtual int cleanup();

public:
    virtual TEST_CASE get_test_case(){ return Mybench_test::get_test_case(); }
    virtual void set_test_case(TEST_CASE test_case){ Mybench_test::set_test_case(test_case); }
    virtual bool get_status(){ return Mybench_test::get_status(); }
    virtual void set_status(bool status){ Mybench_test::set_status(status); }
    virtual Mybench_db_type* get_db_type(){ return Mybench_test::get_db_type(); }
    virtual void set_db_type(Mybench_db_type* db_type){ Mybench_test::set_db_type(db_type); }
    virtual Mybench_config* get_config(){ return Mybench_test::get_config(); }
    virtual void set_config(Mybench_config* config){ Mybench_test::set_config(config); }

private:
    void* m_conn;
};

#endif //MYBENCH_MYBENCH_TEST_LOGIN_H
