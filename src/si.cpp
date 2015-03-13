/*
 * Signal
 *
 * Copyright (C) 2014-2015 Alexander Boettcher, Genode Labs GmbH.
 *
 * This file is part of the NOVA microhypervisor.
 *
 * NOVA is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NOVA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License version 2 for more details.
 */

#include "si.hpp"
#include "sm.hpp"
#include "assert.hpp"

#include "stdio.hpp"

Si::Si (Sm * s, mword v) : sm(s), prev(nullptr), next(nullptr), value(v)
{
    trace (TRACE_SYSCALL, "SI:%p created (SM:%p signal:%#lx)", this, s, v);

    if (sm)
        sm->add_ref();
}

Si::~Si()
{
    if (sm && sm->del_ref())
        delete sm;
}

void Si::submit()
{
    Sm * i = static_cast<Sm *>(this);
    assert(i);

    i->up();

    /* if !sm than it is just a semaphore */
    if (!sm) return;

    /* signal mode - send up() to chained sm */
    sm->up(nullptr, i);
}
