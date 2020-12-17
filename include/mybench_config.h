#ifndef MYBENCH_MYBENCH_CONFIG_H
#define MYBENCH_MYBENCH_CONFIG_H

#include "string"
#include "stdint.h"
#include "mybench_util.h"

enum enum_mybench_db_type
{
    DB_TYPE_UNKNOWN=0,
    DB_TYPE_MYSQL,
    DB_TYPE_PG,
    DB_TYPE_END
};
typedef enum enum_mybench_db_type DB_TYPE;

enum enum_test_case
{
    TEST_UNKNOWN=0,
    TEST_LOGIN,
    TEST_END
};
typedef enum enum_test_case TEST_CASE;

enum enum_mybench_test_cmd
{
    TEST_CMD_UNKNOWN=0,
    TEST_CMD_PREPARE,
    TEST_CMD_RUN,
    TEST_CMD_CLEANUP,
    TEST_CMD_END
};
typedef enum enum_mybench_test_cmd TEST_CMD;

class Mybench_config
{
public:
    Mybench_config();
    virtual ~Mybench_config();
    virtual bool parse_param(int argc, char** argv);
    virtual DB_TYPE get_db_type();
    virtual void set_db_type(DB_TYPE db_type);
    virtual const char* get_test_case();
    virtual int set_test_case(const char* test_case);
    virtual const char* get_db_host();
    virtual int set_db_host(const char* db_host);
    virtual uint32_t get_db_port();
    virtual void set_db_port(uint32_t db_port);
    virtual const char* get_db_user();
    virtual int set_db_user(const char* db_user);
    virtual const char* get_db_password();
    virtual int set_db_password(const char* db_password);
    virtual const char* get_db_schema();
    virtual int set_db_schema(const char* db_schema);
    virtual uint32_t get_threads();
    virtual void set_threads(uint32_t threads);
    virtual uint64_t get_time_sec();
    virtual void set_time_sec(uint64_t time_sec);
    virtual TEST_CMD get_test_cmd();
    virtual void set_test_cmd(const char* test_cmd);

public:
    virtual uint64_t get_wait_tests_finish_interval_sec();
    virtual void set_wait_tests_finish_interval_sec(
            uint64_t wait_tests_finish_interval_sec);

public:
    virtual Mybench_util* get_util();
    virtual void set_util(Mybench_util* util);

private:
    Mybench_util* m_util;
    std::string   m_help_info;
    DB_TYPE       m_db_type;
    char*         m_test_case;
    char*         m_db_host;
    uint32_t      m_db_port;
    char*         m_db_user;
    char*         m_db_password;
    char*         m_db_schema;
    uint32_t      m_threads;
    uint64_t      m_time_sec;
    TEST_CMD      m_test_cmd;

//param not for user
private:
    uint64_t      m_wait_tests_finish_interval_sec;
};


#endif //MYBENCH_MYBENCH_CONFIG_H
