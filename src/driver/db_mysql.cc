#include "driver/db_mysql.h"
#include "mybench_config.h"

Mybench_db_interface_mysql::Mybench_db_interface_mysql()
{

}

Mybench_db_interface_mysql::~Mybench_db_interface_mysql()
{

}

void* Mybench_db_interface_mysql::connect()
{
  int error= 0;
  MYSQL* local_conn= mysql_init(NULL);
  if(local_conn == NULL){
    error= -1;
    m_config->get_util()->log_error("mybench connect to mysql, init mysql error!");
  }else if(!(mysql_real_connect(local_conn,
                                m_config->get_db_host(),
                                m_config->get_db_user(),
                                m_config->get_db_password(),
                                m_config->get_db_schema(),
                                m_config->get_db_port(),
                                NULL,
                                0)))
  {
    error= -2;
    ::free(local_conn);
    m_config->get_util()->log_error("mybench connect to mysql, conn mysql error!");
  }

  if(!error)
  {
    m_conns.push_back(local_conn);
  }

  return local_conn;
}

int Mybench_db_interface_mysql::close(void* conn)
{
  int error= 0;
  MYSQL* it_conn= NULL;
  MYSQL* local_conn= NULL;
  if(conn != NULL){
    local_conn= (MYSQL*) conn;

    std::vector<MYSQL*>::iterator conn_iterator;
    for (conn_iterator= m_conns.begin();
         conn_iterator!= m_conns.end();
         conn_iterator++) {
      it_conn = (*conn_iterator);
      if(it_conn == local_conn)
      {
        m_conns.erase(conn_iterator);
        break;
      }
    }

    if(conn_iterator != m_conns.end())
      m_conns.erase(conn_iterator);
    close_internel(local_conn);
    local_conn= NULL;
  }
  return error;
}

int Mybench_db_interface_mysql::closeall()
{
  MYSQL* local_conn= NULL;

  std::vector<MYSQL*>::iterator conn_iterator;
  for (conn_iterator= m_conns.begin();
       conn_iterator!= m_conns.end();
       conn_iterator++) {
    local_conn = (*conn_iterator);
    close_internel(local_conn);
    local_conn= NULL;
  }
  m_conns.clear();
}

int Mybench_db_interface_mysql::close_internel(MYSQL* conn)
{
  int error= 0;
  if(conn)
  {
    mysql_close(conn);
    ::free(conn);
  }
  return error;
}

int Mybench_db_interface_mysql::query(char* sql)
{
  int error= 0;
  return error;
}
