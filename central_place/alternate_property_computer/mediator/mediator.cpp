/**
 * @file mediator/mediator.cpp
 * @description definition of class mediator.
 */

#include "mediator.h"
#include <assert.h>

mediator* mediator::s_instance = nullptr;

mediator& mediator::get_instance()
{
    assert(s_instance != nullptr);
    return *s_instance;
}

void mediator::instantiate()
{
    assert(s_instance == nullptr);
    s_instance = new mediator();
}

void mediator::destroy()
{
    assert(s_instance != nullptr);
    delete s_instance;
    s_instance = nullptr;
}
