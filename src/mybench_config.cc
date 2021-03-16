#include "mybench_config.h"
#include <getopt.h>
#include <iostream>
#include <string.h>

#define MAX_PARAM_WIDTH 512

char* DEFAULT_DB_HOST     = const_cast<char*>("localhost");
char* DEFAULT_DB_USER     = const_cast<char*>("test");
char* DEFAULT_DB_PASSWORD = const_cast<char*>("test");
char* DEFAULT_DB_SCHEMA   = const_cast<char*>("test");

Mybench_config::Mybench_config()
:m_db_type(DB_TYPE_UNKNOWN),
 m_test_case(NULL),
 m_db_port(3306),
 m_threads(1),
 m_time_sec(10),
 m_test_cmd(TEST_CMD_UNKNOWN),
 m_wait_tests_finish_interval_sec(10)
{
  m_db_host     = DEFAULT_DB_HOST;
  m_db_user     = DEFAULT_DB_USER;
  m_db_password = DEFAULT_DB_PASSWORD;
  m_db_schema   = DEFAULT_DB_SCHEMA;

  m_help_info.append("Usage:\n");
  m_help_info.append("  mysbench [options]...\n\n");
  m_help_info.append("Commands implemented by most tests: prepare run cleanup help\n\n");
  m_help_info.append("General options:\n");

  m_help_info.append("  --host     STRING    MySQL server host [localhost]\n");
  m_help_info.append("  --port     N         MySQL server port [3306]\n");
  m_help_info.append("  --user     STRING    MySQL server user [test]\n");
  m_help_info.append("  --password STRING    MySQL server password [test]\n");
  m_help_info.append("  --schema   STRING    MySQL server schema name [test]\n");
  m_help_info.append("  --threads  N         number of multi-threads to use [1]\n");
  m_help_info.append("  --time     N         total elapsed time in seconds [10]\n");
  m_help_info.append("  --test     STRING    test case name[]\n");
  m_help_info.append("  --cmd      STRING    test command {prepare,run,cleanup} []\n");
  m_help_info.append("  --help     STRING    get help info\n");
}

Mybench_config::~Mybench_config()
{
  if(m_test_case != NULL)
  {
    ::free(m_test_case);
    m_test_case= NULL;
  }
  if(m_db_host != NULL)
  {
    ::free(m_db_host);
    m_db_host= NULL;
  }
  if(m_db_user != NULL)
  {
    ::free(m_db_user);
    m_db_user= NULL;
  }
  if(m_db_password != NULL)
  {
    ::free(m_db_password);
    m_db_password= NULL;
  }
  if(m_db_schema != NULL)
  {
    ::free(m_db_schema);
    m_db_schema= NULL;
  }
}

bool Mybench_config::parse_param(int argc, char** argv)
{
  int error= 0;
  int32_t opt= 0;
  int32_t opt_index= 0;
  static constexpr char opt_char_arr[]= {'h',
                             'P',
                             'u',
                             'p',
                             's',
                             'T',
                             'e',
                             't',
                             'c',
                             'H'
  };
  int opt_char_size= strlen(opt_char_arr);
  char optstring_arr[opt_char_size * 2 + 1];
  memset(optstring_arr, 0, sizeof(optstring_arr));

  for(int i=0; i<opt_char_size; i++)
  {
    optstring_arr[2*i]= opt_char_arr[i];
    optstring_arr[2*i + 1]= ':';
  }
  const char *opt_string= optstring_arr;
  static const struct option help_long_options[] =
  {
    {"host",     required_argument, NULL, opt_char_arr[0]},
    {"port",     required_argument, NULL, opt_char_arr[1]},
    {"user",     required_argument, NULL, opt_char_arr[2]},
    {"password", required_argument, NULL, opt_char_arr[3]},
    {"schema",   required_argument, NULL, opt_char_arr[4]},
    {"threads",  required_argument, NULL, opt_char_arr[5]},
    {"time",     required_argument, NULL, opt_char_arr[6]},
    {"test",     required_argument, NULL, opt_char_arr[7]},
    {"cmd",      required_argument, NULL, opt_char_arr[8]},
    {"help",     no_argument,       NULL, opt_char_arr[9]}
  };

  opt= getopt_long(argc, argv, opt_string, help_long_options, &opt_index);

  if (opt == -1)
  {
    m_util->log_info(m_help_info.c_str());
    error= -1;
    return error;
  }

  while (-1 != opt)
  {
    switch (opt)
    {
      case opt_char_arr[0]:
        m_util->log_info("input host=[%s]", optarg);
        set_db_host(optarg);
        break;
      case opt_char_arr[1]:
        m_util->log_info("input port=[%s]", optarg);
        set_db_port(atoi(optarg));
        break;
      case opt_char_arr[2]:
        m_util->log_info("input user=[%s]", optarg);
        set_db_user(optarg);
        break;
      case opt_char_arr[3]:
        m_util->log_info("input password=[******]");
        set_db_password(optarg);
        break;
      case opt_char_arr[4]:
        m_util->log_info("input schema=[%s]", optarg);
        set_db_schema(optarg);
        break;
      case opt_char_arr[5]:
        m_util->log_info("input threads=[%s]", optarg);
        set_threads(atoi(optarg));
        break;
      case opt_char_arr[6]:
        m_util->log_info("input time=[%s]", optarg);
        set_time_sec(atoll(optarg));
        break;
      case opt_char_arr[7]:
        m_util->log_info("input test case=[%s]", optarg);
        set_test_case(optarg);
        break;
      case opt_char_arr[8]:
        m_util->log_info("input test cmd=[%s]", optarg);
        set_test_cmd(optarg);
        break;
      case opt_char_arr[9]:
        m_util->log_info(m_help_info.c_str());
        break;
      default:
        m_util->log_info("invalid input parameter!");
        error= -2;
        break;
    }
    opt= getopt_long(argc, argv, "i:d:h", help_long_options, &opt_index);
  }

  //check required params
  if(get_test_case() == NULL)
  {
    m_util->log_info("input test case is null");
    error= -3;
  }
  if(get_test_cmd() == TEST_CMD_UNKNOWN)
  {
    m_util->log_info("input test cmd is null");
    error= -4;
  }
  return error;
}

DB_TYPE Mybench_config::get_db_type()
{
  return m_db_type;
}

void Mybench_config::set_db_type(DB_TYPE db_type)
{
  m_db_type= db_type;
}

const char* Mybench_config::get_test_case()
{
  return m_test_case;
}

int Mybench_config::set_test_case(const char* test_case)
{
  int error= 0;
  int param_len= 0;

  param_len= strlen(test_case);
  if(param_len > MAX_PARAM_WIDTH)
  {
    error= -1;
  } else{
    m_test_case= (char *) ::malloc(param_len + 1);
    strncpy(m_test_case, test_case, param_len + 1);
  }
  return error;
}

const char* Mybench_config::get_db_host()
{
  return m_db_host;
}

int Mybench_config::set_db_host(const char* db_host)
{
  int error= 0;
  int param_len= 0;

  param_len= strlen(db_host);
  if(param_len > MAX_PARAM_WIDTH)
  {
    error= -1;
  } else{
    m_db_host= (char *) ::malloc(param_len + 1);
    strncpy(m_db_host, db_host, param_len + 1);
  }
  return error;
}

uint32_t Mybench_config::get_db_port()
{
  return m_db_port;
}

void Mybench_config::set_db_port(uint32_t db_port)
{
  m_db_port= db_port;
}

const char* Mybench_config::get_db_user()
{
  return m_db_user;
}

int Mybench_config::set_db_user(const char* db_user)
{
  int error= 0;
  int param_len= 0;

  param_len= strlen(db_user);
  if(param_len > MAX_PARAM_WIDTH)
  {
    error= -1;
  } else{
    m_db_user= (char *) ::malloc(param_len + 1);
    strncpy(m_db_user, db_user, param_len + 1);
  }
  return error;
}

const char* Mybench_config::get_db_password()
{
  return m_db_password;
}

int Mybench_config::set_db_password(const char* db_password)
{
  int error= 0;
  int param_len= 0;

  param_len= strlen(db_password);
  if(param_len > MAX_PARAM_WIDTH)
  {
    error= -1;
  } else{
    m_db_password= (char *) ::malloc(param_len + 1);
    strncpy(m_db_password, db_password, param_len + 1);
  }
  return error;
}

const char* Mybench_config::get_db_schema()
{
  return m_db_schema;
}

int Mybench_config::set_db_schema(const char* db_schema)
{
  int error= 0;
  int param_len= 0;

  param_len= strlen(db_schema);
  if(param_len > MAX_PARAM_WIDTH)
  {
    error= -1;
  } else{
    m_db_schema= (char *) ::malloc(param_len + 1);
    strncpy(m_db_schema, db_schema, param_len + 1);
  }
  return error;
}

uint32_t Mybench_config::get_threads()
{
  return m_threads;
}

void Mybench_config::set_threads(uint32_t threads)
{
  m_threads= threads;
}

uint64_t Mybench_config::get_time_sec()
{
  return m_time_sec;
}

TEST_CMD Mybench_config::get_test_cmd()
{
  return m_test_cmd;
}

void Mybench_config::set_test_cmd(const char* test_cmd)
{
  if(!strcasecmp(test_cmd, "prepare"))
  {
    m_test_cmd= TEST_CMD_PREPARE;
  }else if(!strcasecmp(test_cmd, "run"))
  {
    m_test_cmd= TEST_CMD_RUN;
  }else if(!strcasecmp(test_cmd, "cleanup"))
  {
    m_test_cmd= TEST_CMD_CLEANUP;
  }else
  {
    m_test_cmd= TEST_CMD_UNKNOWN;
  }
}

uint64_t Mybench_config::get_wait_tests_finish_interval_sec()
{
  return m_wait_tests_finish_interval_sec;
}

void Mybench_config::set_wait_tests_finish_interval_sec(
        uint64_t wait_tests_finish_interval_sec)
{
  m_wait_tests_finish_interval_sec= wait_tests_finish_interval_sec;
}

Mybench_util* Mybench_config::get_util()
{
  return m_util;
}

void Mybench_config::set_util(Mybench_util* util)
{
  m_util= util;
}
