#ifndef MYBENCH_MYBENCH_DB_TYPE_H
#define MYBENCH_MYBENCH_DB_TYPE_H

#include "string"
#include "stdint.h"
#include "mybench_config.h"
#include "mybench_db_interface.h"

class Mybench_db_type
{
public:
    Mybench_db_type();
    virtual ~Mybench_db_type();

    virtual int initialize();
    virtual int terminate();
    virtual Mybench_db_interface* get_db_interface();

public:
    virtual Mybench_config* get_config();
    virtual void set_config(Mybench_config* config);

private:
    Mybench_db_interface* m_db_interface;
    Mybench_config*       m_config;
};

#endif //MYBENCH_MYBENCH_DB_TYPE_H
