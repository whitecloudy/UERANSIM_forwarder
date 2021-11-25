/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef OGS_CORE_H
#define OGS_CORE_H


#define OGS_CORE_INSIDE

#include "ogs-compat.h"
#include "ogs-macros.h"
#include "ogs-list.h"
#include "ogs-pool.h"
#include "ogs-abort.h"
#include "ogs-strings.h"
#include "ogs-errno.h"
#include "ogs-time.h"
#include "ogs-conv.h"
#include "ogs-log.h"
#include "ogs-pkbuf.h"
#include "ogs-memory.h"
#include "ogs-rand.h"
#include "ogs-uuid.h"
#include "ogs-rbtree.h"
#include "ogs-timer.h"
#include "ogs-thread.h"
#include "ogs-process.h"
#include "ogs-signal.h"
#include "ogs-sockaddr.h"
#include "ogs-socket.h"
#include "ogs-sockpair.h"
#include "ogs-socknode.h"
#include "ogs-udp.h"
#include "ogs-tcp.h"
#include "ogs-queue.h"
#include "ogs-poll.h"
#include "ogs-notify.h"
#include "ogs-tlv.h"
#include "ogs-tlv-msg.h"
#include "ogs-env.h"
#include "ogs-fsm.h"
#include "ogs-hash.h"
#include "ogs-misc.h"
#include "ogs-getopt.h"
#include "ogs-3gpp-types.h"

#undef OGS_CORE_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

extern int __ogs_mem_domain;
extern int __ogs_sock_domain;
extern int __ogs_event_domain;
extern int __ogs_thread_domain;
extern int __ogs_tlv_domain;

typedef struct {
    struct {
        int pool;
        int domain_pool;
        ogs_log_level_e level;
    } log;

    struct {
        int pool;
        int config_pool;
    } pkbuf;

    struct {
        int pool;
    } tlv;

} ogs_core_context_t;

void ogs_core_initialize(void);
void ogs_core_terminate(void);

ogs_core_context_t *ogs_core(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_CORE_H */
