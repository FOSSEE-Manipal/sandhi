// Copyright (C) by Josh Blum. See LICENSE.txt for licensing information.

#include <gras/thread_pool.hpp>
#include <gras_impl/block_actor.hpp>
#include <Theron/Framework.h>
#include <iostream>

using namespace gras;

/***********************************************************************
 * Active framework implementation
 **********************************************************************/
static boost::weak_ptr<Theron::Framework> weak_framework;

void ThreadPool::set_active(void)
{
    weak_framework = *this;
}

//! this routine used by the query interface only for stats
ThreadPool get_active_thread_pool(void)
{
    return ThreadPool(weak_framework);
}

/***********************************************************************
 * Block actor factory - gets active framework
 **********************************************************************/
BlockActor *BlockActor::make(const ThreadPool &tp)
{
    //thread pool provided, use it
    if (tp) return new BlockActor(tp);

    //was the thread per block env specified
    if (getenv("GRAS_TPP"))
    {
        ThreadPoolConfig config;
        config.thread_count = 1;
        return new BlockActor(ThreadPool(config));
    }

    //otherwise, create/use active pool
    else
    {
        ThreadPool active(weak_framework);
        if (not active)
        {
            active = ThreadPool(ThreadPoolConfig());
            active.set_active();
            std::cout << "Created default thread pool with " << active->GetNumThreads() << " threads." << std::endl;
        }
        return new BlockActor(active);
    }
}

BlockActor::BlockActor(const ThreadPool &tp):
    Theron::Actor(*tp)
{
    this->thread_pool = tp;
    this->register_handlers();
    this->prio_token = Token::make();
}

BlockActor::~BlockActor(void)
{
    //NOP
}
