#include "mybench_test.h"
#include "mybench_config.h"
#include "test/mybench_test_login.h"

Mybench_test_manager::Mybench_test_manager()
{

}

Mybench_test_manager::~Mybench_test_manager()
{
  clean_tests();
}

int Mybench_test_manager::initialize()
{
  int result= 0;
  TEST_CASE test_case= TEST_UNKNOWN;
  Mybench_test* test= NULL;
  int i;
  for (i = TEST_LOGIN; i < TEST_END; i++)
  {
    test_case= TEST_CASE(i);
    switch(test_case)
    {
      case TEST_LOGIN:
        test= new Mybench_test_login();
        break;
      default:
        test= NULL;
        break;
    }
    if(test != NULL)
    {
      test->set_config(m_config);
      test->set_test_case(test_case);
      register_test(test);
    }else{
      result= -1;
      m_config->get_util()->log_error("Initializing tests, "
                            "failed to allocate memory for Mybench_test or invalid object type, "
                            "abort this action for test case=%d!", test_case);
      break;
    }
  }

  return result;
}

Mybench_test* Mybench_test_manager::create_test()
{
  Mybench_test* test= NULL;
  const char* test_case= m_config->get_test_case();

  if(!strcasecmp(test_case, "login"))
  {
    test= new Mybench_test_login();
    test->set_test_case(TEST_LOGIN);
  }else
  {
    test= NULL;
  }
  if(test != NULL)
  {
    test->set_config(m_config);
    register_test(test);
  }else{
    m_config->get_util()->log_error("Initializing tests, "
           "failed to allocate memory for Mybench_test or invalid object type, "
           "abort this action for test case=%d!", test_case);
  }

  return test;
}

Mybench_config* Mybench_test_manager::get_config()
{
  return m_config;
}

void Mybench_test_manager::set_config(Mybench_config* config)
{
  m_config= config;
}

int Mybench_test_manager::register_test(Mybench_test* test)
{
  m_tests.push_back(test);
  return 0;
}

void Mybench_test_manager::clean_tests()
{
  Mybench_test *test= NULL;

  std::vector<Mybench_test*>::iterator test_iterator;
  for (test_iterator= m_tests.begin();
       test_iterator!= m_tests.end();
       test_iterator++) {
    test = (*test_iterator);

    if(test)
    {
      delete test;
      test= NULL;
    }
  }
  m_tests.clear();
}
