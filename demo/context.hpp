#ifndef CONTEXT_H
#define CONTEXT_H

#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

struct shm_context
{
    boost::interprocess::interprocess_mutex mutex;
    boost::interprocess::interprocess_condition condition;
};

#endif   // CONTEXT_H
