#include "thread.hpp"

Thread::Thread() : _threadId(0) {}

Thread::~Thread()
{
}

bool Thread::Start(ThreadFunction callback, void* param)  //  creates a new stream
{

    if (pthread_create(&_threadId, NULL, *callback, param) == 0)  // checking for success of creating a stream
        return true;

    return false;
}