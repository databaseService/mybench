#ifndef MYBENCH_MYBENCH_DB_INTERFACE_H
#define MYBENCH_MYBENCH_DB_INTERFACE_H

class Mybench_config;

class Mybench_db_interface
{
public:
    Mybench_db_interface();
    ~Mybench_db_interface();
    virtual void* connect();
    virtual int close(void* conn);
    virtual int closeall();
    virtual int query(char* sql);

public:
    virtual Mybench_config* get_config();
    virtual void set_config(Mybench_config* config);

private:
    Mybench_config*    m_config;

};

#endif //MYBENCH_MYBENCH_DB_INTERFACE_H
