#include "test/mybench_test_login.h"
#include "mybench_db_type.h"

Mybench_test_login::Mybench_test_login()
:m_conn(NULL)
{

}

Mybench_test_login::~Mybench_test_login()
{

}

int Mybench_test_login::initialize()
{
  int error= 0;

  return error;
}

int Mybench_test_login::terminate()
{
  int error= 0;

  return error;
}

int Mybench_test_login::prepare()
{
  int error= 0;

  return error;
}

int Mybench_test_login::run()
{
  int error= 0;
  void* conn= get_db_type()->get_db_interface()->connect();
  if(conn == NULL)
  {
    error= -1;
  }else
  {
    m_conn= conn;
  }
  return error;
}

int Mybench_test_login::clean_once()
{
  int error= 0;
  get_db_type()->get_db_interface()->close(m_conn);
  m_conn= NULL;
  return error;
}

int Mybench_test_login::cleanup()
{
  int error= 0;

  return error;
}
