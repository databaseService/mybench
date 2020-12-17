#include "mybench_db_type.h"
#include "driver/db_mysql.h"

Mybench_db_type::Mybench_db_type()
{
}

Mybench_db_type::~Mybench_db_type()
{
  if(m_db_interface != NULL)
  {
    delete m_db_interface;
    m_db_interface= NULL;
  }
}

int Mybench_db_type::initialize()
{
  int error= 0;
  switch(m_config->get_db_type())
  {
    case DB_TYPE_MYSQL:
      m_db_interface= new Mybench_db_interface_mysql();
      m_db_interface->set_config(m_config);
      break;
    default:
      error= -1;
  }
  return error;
}

int Mybench_db_type::terminate()
{
  int error= 0;
  if(m_db_interface != NULL)
  {
    m_db_interface->closeall();
  }
  return error;
}

Mybench_db_interface* Mybench_db_type::get_db_interface()
{
  return m_db_interface;
}

Mybench_config* Mybench_db_type::get_config()
{
  return m_config;
}

void Mybench_db_type::set_config(Mybench_config* config)
{
  m_config= config;
}
