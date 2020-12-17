#ifndef MYBENCH_MYBENCH_H
#define MYBENCH_MYBENCH_H

int load_config(int argc, char** argv);

int init_tests();

int init_threads();

int run_tests();

int wait_tests_finish();

int analyse_results();

int stop_threads();

int cleanup();

#endif //MYBENCH_MYBENCH_H
