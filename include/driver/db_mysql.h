#ifndef MYBENCH_DB_MYSQL_H
#define MYBENCH_DB_MYSQL_H

#include <string>
#include <vector>
#ifdef __APPLE__
  #include "/usr/local/mysql-8.0.22-macos10.15-x86_64/include/mysql.h"
#else
  #include <mysql/mysql.h>
#endif

#include "mybench_db_interface.h"

class Mybench_db_interface_mysql : public Mybench_db_interface
{
public:
    Mybench_db_interface_mysql();
    virtual ~Mybench_db_interface_mysql();
    virtual void* connect();
    virtual int close(void* conn);
    virtual int closeall();
    virtual int query(char* sql);

private:
    virtual int close_internel(MYSQL* conn);

public:
    virtual Mybench_config* get_config(){ return Mybench_db_interface::get_config(); }
    virtual void set_config(Mybench_config* config){ Mybench_db_interface::set_config(config); }

private:
    Mybench_config*     m_config;
    std::vector<MYSQL*> m_conns;
};

#endif //MYBENCH_DB_MYSQL_H
