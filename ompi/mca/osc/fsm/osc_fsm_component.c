/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012      Sandia National Laboratories.  All rights reserved.
 * Copyright (c) 2014-2018 Los Alamos National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2014      Intel, Inc. All rights reserved.
 * Copyright (c) 2015      Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2015-2018 Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * Copyright (c) 2017      The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2016-2017 IBM Corporation. All rights reserved.
 * Copyright (c) 2018      Amazon.com, Inc. or its affiliates.  All Rights reserved.
 * Copyright (c) 2019      Hewlett Packard Enterprise. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include "ompi/mca/osc/osc.h"
#include "ompi/mca/osc/base/base.h"
#include "ompi/mca/osc/base/osc_base_obj_convert.h"
#include "ompi/request/request.h"
#include "opal/util/sys_limits.h"
#include "opal/include/opal/align.h"
#include "opal/util/info_subscriber.h"
#include "opal/util/printf.h"
#include "opal/mca/common/ofi/common_ofi.h"
#include "ompi/mca/mtl/mtl.h"
#include "ompi/mca/mtl/ofi/mtl_ofi_endpoint.h"

#include "osc_fsm.h"
#include <sys/mman.h>
#include <rdma/fabric.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_ext_zhpe.h>

static int component_open(void);
static int component_init(bool enable_progress_threads, bool enable_mpi_threads);
static int component_finalize(void);
static int component_query(struct ompi_win_t *win, void **base, size_t size, int disp_unit,
                           struct ompi_communicator_t *comm, struct opal_info_t *info,
                           int flavor);
static int component_register (void);
static int component_select(struct ompi_win_t *win, void **base, size_t size, int disp_unit,
                            struct ompi_communicator_t *comm, struct opal_info_t *info,
                            int flavor, int *model);
static char* component_set_blocking_fence_info(opal_infosubscriber_t *obj, char *key, char *val);
static char* component_set_alloc_shared_noncontig_info(opal_infosubscriber_t *obj, char *key, char *val);


ompi_osc_fsm_component_t mca_osc_fsm_component = {
    { /* ompi_osc_base_component_t */
        .osc_version = {
            OMPI_OSC_BASE_VERSION_3_0_0,
            .mca_component_name = "fsm",
            MCA_BASE_MAKE_VERSION(component, OMPI_MAJOR_VERSION, OMPI_MINOR_VERSION,
                                  OMPI_RELEASE_VERSION),
            .mca_open_component = component_open,
            .mca_register_component_params = component_register,
        },
        .osc_data = { /* mca_base_component_data */
            /* The component is not checkpoint ready */
            MCA_BASE_METADATA_PARAM_NONE
        },
        .osc_init = component_init,
        .osc_query = component_query,
        .osc_select = component_select,
        .osc_finalize = component_finalize,
    }
};


ompi_osc_fsm_module_t ompi_osc_fsm_module_template = {
    {
        .osc_win_shared_query = ompi_osc_fsm_shared_query,

        .osc_win_attach = ompi_osc_fsm_attach,
        .osc_win_detach = ompi_osc_fsm_detach,
        .osc_free = ompi_osc_fsm_free,

        .osc_put = ompi_osc_fsm_put,
        .osc_get = ompi_osc_fsm_get,
        .osc_accumulate = ompi_osc_fsm_accumulate,
        .osc_compare_and_swap = ompi_osc_fsm_compare_and_swap,
        .osc_fetch_and_op = ompi_osc_fsm_fetch_and_op,
        .osc_get_accumulate = ompi_osc_fsm_get_accumulate,

        .osc_rput = ompi_osc_fsm_rput,
        .osc_rget = ompi_osc_fsm_rget,
        .osc_raccumulate = ompi_osc_fsm_raccumulate,
        .osc_rget_accumulate = ompi_osc_fsm_rget_accumulate,

        .osc_fence = ompi_osc_fsm_fence,

        .osc_start = ompi_osc_fsm_start,
        .osc_complete = ompi_osc_fsm_complete,
        .osc_post = ompi_osc_fsm_post,
        .osc_wait = ompi_osc_fsm_wait,
        .osc_test = ompi_osc_fsm_test,

        .osc_lock = ompi_osc_fsm_lock,
        .osc_unlock = ompi_osc_fsm_unlock,
        .osc_lock_all = ompi_osc_fsm_lock_all,
        .osc_unlock_all = ompi_osc_fsm_unlock_all,

        .osc_sync = ompi_osc_fsm_sync,
        .osc_flush = ompi_osc_fsm_flush,
        .osc_flush_all = ompi_osc_fsm_flush_all,
        .osc_flush_local = ompi_osc_fsm_flush_local,
        .osc_flush_local_all = ompi_osc_fsm_flush_local_all,
    }
};

static int component_register (void)
{
    /* TODO: Add own component vars */
    if (0 == access ("/dev/shm", W_OK)) {
        mca_osc_fsm_component.backing_directory = "/dev/shm";
    } else {
        mca_osc_fsm_component.backing_directory = ompi_process_info.proc_session_dir;
    }

    (void) mca_base_component_var_register (&mca_osc_fsm_component.super.osc_version, "backing_directory",
                                            "Directory to place backing files for shared memory windows. "
                                            "This directory should be on a local filesystem such as /tmp or "
                                            "/dev/shm (default: (linux) /dev/shm, (others) session directory)",
                                            MCA_BASE_VAR_TYPE_STRING, NULL, 0, 0, OPAL_INFO_LVL_3,
                                            MCA_BASE_VAR_SCOPE_READONLY, &mca_osc_fsm_component.backing_directory);

    return OPAL_SUCCESS;
}

static int
component_open(void)
{
    return OMPI_SUCCESS;
}


static int
component_init(bool enable_progress_threads, bool enable_mpi_threads)
{
    return OMPI_SUCCESS;
}


static int
component_finalize(void)
{
    /* clean up requests free list */

    return OMPI_SUCCESS;
}


static int
check_win_ok(ompi_communicator_t *comm, int flavor)
{
    if (! (MPI_WIN_FLAVOR_SHARED == flavor
           || MPI_WIN_FLAVOR_ALLOCATE == flavor) ) {
        return OMPI_ERR_NOT_SUPPORTED;
    }


    if (mca_common_ofi_get_ofi_info(NULL, NULL, NULL, NULL) != OPAL_SUCCESS) {
        OPAL_OUTPUT_VERBOSE((1, ompi_osc_base_framework.framework_output,
                             "No ofi endpoint found; disqualifying myself\n"));
        return OMPI_ERR_NOT_AVAILABLE;
    }

    /* FIXME: look if HW supports RMA */

    return OMPI_SUCCESS;
}


static int
component_query(struct ompi_win_t *win, void **base, size_t size, int disp_unit,
                struct ompi_communicator_t *comm, struct opal_info_t *info,
                int flavor)
{
    int ret;
    if (OMPI_SUCCESS != (ret = check_win_ok(comm, flavor))) {
        return ret;
    }

    /* FIXME: Look in opal_info_t if there are any flags that are not supported in this window */

    return 1000; //FIXME: Maybe don't return an arbitrary value that ensures this component is selected
}


static int
component_select(struct ompi_win_t *win, void **base, size_t size, int disp_unit,
                 struct ompi_communicator_t *comm, struct opal_info_t *info,
                 int flavor, int *model)
{
    ompi_osc_fsm_module_t *module = NULL;
    int comm_size = ompi_comm_size (comm);

    int ret = OMPI_ERROR;

    if (OMPI_SUCCESS != (ret = check_win_ok(comm, flavor))) {
        return ret;
    }

    /* create module structure */
    module = (ompi_osc_fsm_module_t*)
        calloc(1, sizeof(ompi_osc_fsm_module_t));
    if (NULL == module) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;

    win->w_osc_module = &module->super;

    OBJ_CONSTRUCT(&module->lock, opal_mutex_t);

    /* fill in the function pointer part */
    memcpy(module, &ompi_osc_fsm_module_template,
           sizeof(ompi_osc_base_module_t));

    /* need our communicator for collectives in next phase */
    ret = ompi_comm_dup(comm, &module->comm);
    if (OMPI_SUCCESS != ret) goto error;

    module->flavor = flavor;

    /* create the segment */
    if (1 == comm_size) {
        module->my_segment_base = NULL;
        module->sizes = malloc(sizeof(size_t));
        if (NULL == module->sizes) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        module->bases = malloc(sizeof(void*));
        if (NULL == module->bases) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;

        module->sizes[0] = size;
        module->bases[0] = malloc(size);
        if (NULL == module->bases[0]) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;

        module->global_state = malloc(sizeof(ompi_osc_fsm_global_state_t));
        if (NULL == module->global_state) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        module->node_states = malloc(sizeof(ompi_osc_fsm_node_state_t));
        if (NULL == module->node_states) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        module->posts = calloc (1, sizeof(module->posts[0]) + sizeof (module->posts[0][0]));
        if (NULL == module->posts) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        module->posts[0] = (osc_fsm_post_atomic_type_t *) (module->posts + 1);
    } else {
        unsigned long total, *rbuf;
        int i;
        size_t pagesize;
        size_t state_size;
        uint64_t* remote_keys;
        size_t posts_size, post_amount = (comm_size + OSC_FSM_POST_MASK) / (OSC_FSM_POST_MASK + 1);
        struct fid_domain *ofi_domain = NULL;
        struct fid_fabric *ofi_fabric = NULL;
        struct fid_av *ofi_av = NULL;
        struct fid_ep *ofi_ep = NULL;
        uint64_t access_flags = FI_REMOTE_WRITE | FI_REMOTE_READ | FI_READ | FI_WRITE;


        OPAL_OUTPUT_VERBOSE((1, ompi_osc_base_framework.framework_output,
                             "allocating shared memory region of size %ld\n", (long) size));

        pagesize = opal_getpagesize();
        mca_common_ofi_get_ofi_info(&ofi_fabric, &ofi_domain, &ofi_av, &ofi_ep);

        rbuf = malloc(sizeof(rbuf[0]) * comm_size);
        if (NULL == rbuf) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        remote_keys = malloc(sizeof(remote_keys[0]) * comm_size);
        if (NULL == remote_keys) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        module->mdesc = malloc(sizeof(module->mdesc[0]) * comm_size);
        if (NULL == module->mdesc) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;

        module->noncontig = false;

        // Expand size to page size
        total = ((size - 1) / pagesize + 1) * pagesize;

        if (0 == ompi_comm_rank (module->comm)) {
            state_size = sizeof(ompi_osc_fsm_global_state_t) + sizeof(ompi_osc_fsm_node_state_t);
        } else {
            state_size = sizeof(ompi_osc_fsm_node_state_t);
        }
        state_size += OPAL_ALIGN_PAD_AMOUNT(state_size, CACHELINE_SZ);
        /* TODO: Find out if only one post integer per cache line is needed */
        posts_size = post_amount * sizeof (module->posts[0][0]);
        posts_size += OPAL_ALIGN_PAD_AMOUNT(posts_size, CACHELINE_SZ);

        //TODO segment create and get magic number
        ret = posix_memalign(&module->my_segment_base, pagesize, total + posts_size + state_size);
        if(ret) {
            if(EINVAL == ret) {
                OPAL_OUTPUT_VERBOSE((1, ompi_osc_base_framework.framework_output,
                                     "Couldn't allocate memory aligned to %ld\n", (long) pagesize));
                ret = OMPI_ERR_OUT_OF_RESOURCE;
            } else if(ENOMEM == ret){
                ret = OMPI_ERR_TEMP_OUT_OF_RESOURCE;
            }
            goto error;
        }

        ret = fi_mr_reg(ofi_domain, module->my_segment_base, total, access_flags, 0, 0, 0, &module->mr, NULL);

        if(ret) {
            goto error;
        }

        module->mr_key = fi_mr_key(module->mr);

        // FIXME properly move every communication into one buffer (Only one all gather needed...)
        ret = module->comm->c_coll->coll_allgather(&module->mr_key, 1, MPI_UINT64_T,
                                                  remote_keys, 1, MPI_UINT64_T,
                                                  module->comm,
                                                  module->comm->c_coll->coll_allgather_module);
        if (OMPI_SUCCESS != ret) {
            goto error;
        }

        ret = module->comm->c_coll->coll_allgather(&total, 1, MPI_UNSIGNED_LONG,
                                                  rbuf, 1, MPI_UNSIGNED_LONG,
                                                  module->comm,
                                                  module->comm->c_coll->coll_allgather_module);
        if (OMPI_SUCCESS != ret) {
            goto error;
        }
        ret = fi_open_ops(&ofi_fabric->fid, FI_ZHPE_OPS_V1, 0,
                (void **)&module->ext_ops, NULL);

        for(i = 0; i < comm_size; i++) {
            if(i != ompi_comm_rank(module->comm)) {
                struct ompi_proc_t *ompi_proc;
                mca_mtl_ofi_endpoint_t *endpoint;

                ompi_proc = ompi_comm_peer_lookup(comm, i);
                //FIXME: This should be done in a common component. If is just a coincident that ompi_mtl_ofi_get_endpoint doesn't need the module
                endpoint = ompi_mtl_ofi_get_endpoint(NULL, ompi_proc);

                /*FIXME: For Scalable Endpoints, gather target receive context */
                //sep_peer_fiaddr = fi_rx_addr(endpoint->peer_fiaddr, ctxt_id, ompi_mtl_ofi.rx_ctx_bits);
                ret = module->ext_ops->mmap(NULL, rbuf[i], PROT_READ | PROT_WRITE,
                        MAP_SHARED, 0, ofi_ep, endpoint->peer_fiaddr, remote_keys[i],
                        FI_ZHPE_MMAP_CACHE_WB, module->mdesc + i);
                if(ret) {
                    //TODO cleanup
                    goto error;
                }
            } else {
                module->mdesc[i] = NULL;
            }
        }

        /* FIXME: Is this needed? Wait for all processes to attach */
        ret = module->comm->c_coll->coll_barrier (module->comm, module->comm->c_coll->coll_barrier_module);
        if (OMPI_SUCCESS != ret) {
            goto error;
        }

        module->sizes = malloc(sizeof(size_t) * comm_size);
        if (NULL == module->sizes) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        module->bases = malloc(sizeof(void*) * comm_size);
        if (NULL == module->bases) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        module->posts = calloc (comm_size, sizeof (module->posts[0]));
        if (NULL == module->posts) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        module->node_states = calloc (comm_size, sizeof (module->node_states[0]));
        if (NULL == module->node_states) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;

        for(i = 0; i < comm_size; i++) {
            void * base;
            if(i == ompi_comm_rank(module->comm)) {
                base = module->my_segment_base;
            } else {
                base = module->mdesc[i]->addr;
            }

            module->posts[i] = base;
            base = module->posts[i] + posts_size;
            if(0 == i) {
                module->global_state = base;
                base = module->global_state + 1;
            }
            module->node_states[i] = base;
            module->bases[i] = module->node_states[i] + 1;
            module->sizes[i] = rbuf[i];
        }

        free(rbuf);
        free(remote_keys);
    }

    /* initialize my state shared */
    module->my_node_state = module->node_states[ompi_comm_rank(module->comm)];
    memset (module->my_node_state, 0, sizeof(*module->my_node_state));

    *base = module->bases[ompi_comm_rank(module->comm)];

    opal_atomic_lock_init(&module->my_node_state->accumulate_lock, OPAL_ATOMIC_LOCK_UNLOCKED);

    /* share everyone's displacement units. */
    module->disp_units = malloc(sizeof(int) * comm_size);
    ret = module->comm->c_coll->coll_allgather(&disp_unit, 1, MPI_INT,
                                              module->disp_units, 1, MPI_INT,
                                              module->comm,
                                              module->comm->c_coll->coll_allgather_module);
    if (OMPI_SUCCESS != ret) goto error;

    module->start_group = NULL;
    module->post_group = NULL;

    /* initialize synchronization code */
    module->my_sense = 1;

    module->outstanding_locks = calloc(comm_size, sizeof(enum ompi_osc_fsm_locktype_t));
    if (NULL == module->outstanding_locks) {
        ret = OMPI_ERR_TEMP_OUT_OF_RESOURCE;
        goto error;
    }

    if (0 == ompi_comm_rank(module->comm)) {
#if HAVE_PTHREAD_CONDATTR_SETPSHARED && HAVE_PTHREAD_MUTEXATTR_SETPSHARED
        pthread_mutexattr_t mattr;
        pthread_condattr_t cattr;
        bool blocking_fence=false;
        int flag;

        if (OMPI_SUCCESS != opal_info_get_bool(info, "blocking_fence",
                                               &blocking_fence, &flag)) {
            goto error;
        }

        if (flag && blocking_fence) {
            ret = pthread_mutexattr_init(&mattr);
            ret = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
            if (ret != 0) {
                module->global_state->use_barrier_for_fence = 1;
            } else {
                ret = pthread_mutex_init(&module->global_state->mtx, &mattr);
                if (ret != 0) {
                    module->global_state->use_barrier_for_fence = 1;
                } else {
                    pthread_condattr_init(&cattr);
                    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
                    ret = pthread_cond_init(&module->global_state->cond, &cattr);
                    if (ret != 0) return OMPI_ERROR;
                    pthread_condattr_destroy(&cattr);
                }
            }
            module->global_state->use_barrier_for_fence = 0;
            module->global_state->sense = module->my_sense;
            module->global_state->count = comm_size;
            pthread_mutexattr_destroy(&mattr);
        } else {
            module->global_state->use_barrier_for_fence = 1;
        }
#else
        module->global_state->use_barrier_for_fence = 1;
#endif
    }

    ret = opal_infosubscribe_subscribe(&(win->super), "blocking_fence", "false",
        component_set_blocking_fence_info);

    if (OPAL_SUCCESS != ret) goto error;

    ret = module->comm->c_coll->coll_barrier(module->comm,
                                            module->comm->c_coll->coll_barrier_module);
    if (OMPI_SUCCESS != ret) goto error;

    *model = MPI_WIN_UNIFIED;

    return OMPI_SUCCESS;

 error:

    //TODO: Free; look at ompi_osc_fsm_free(struct ompi_win_t *win)
    ompi_osc_fsm_free (win);

    return ret;
}


int
ompi_osc_fsm_shared_query(struct ompi_win_t *win, int rank, size_t *size, int *disp_unit, void *baseptr)
{
    ompi_osc_fsm_module_t *module =
        (ompi_osc_fsm_module_t*) win->w_osc_module;

    if (module->flavor != MPI_WIN_FLAVOR_SHARED) {
        return MPI_ERR_WIN;
    }

    if (MPI_PROC_NULL != rank) {
        *size = module->sizes[rank];
        *((void**) baseptr) = module->bases[rank];
        *disp_unit = module->disp_units[rank];
    } else {
        int i = 0;

        *size = 0;
        *((void**) baseptr) = NULL;
        *disp_unit = 0;
        for (i = 0 ; i < ompi_comm_size(module->comm) ; ++i) {
            if (0 != module->sizes[i]) {
                *size = module->sizes[i];
                *((void**) baseptr) = module->bases[i];
                *disp_unit = module->disp_units[i];
                break;
            }
        }
    }

    return OMPI_SUCCESS;
}


int
ompi_osc_fsm_attach(struct ompi_win_t *win, void *base, size_t len)
{
    ompi_osc_fsm_module_t *module =
        (ompi_osc_fsm_module_t*) win->w_osc_module;

    if (module->flavor != MPI_WIN_FLAVOR_DYNAMIC) {
        return MPI_ERR_RMA_ATTACH;
    }
    return OMPI_SUCCESS;
}


int
ompi_osc_fsm_detach(struct ompi_win_t *win, const void *base)
{
    ompi_osc_fsm_module_t *module =
        (ompi_osc_fsm_module_t*) win->w_osc_module;

    if (module->flavor != MPI_WIN_FLAVOR_DYNAMIC) {
        return MPI_ERR_RMA_ATTACH;
    }
    return OMPI_SUCCESS;
}


int
ompi_osc_fsm_free(struct ompi_win_t *win)
{
    //TODO rework
    ompi_osc_fsm_module_t *module =
        (ompi_osc_fsm_module_t*) win->w_osc_module;

    /* free memory */
    if (NULL != module->my_segment_base) {
        /* synchronize */
        module->comm->c_coll->coll_barrier(module->comm,
                                          module->comm->c_coll->coll_barrier_module);

        // TODO cleanup opal_shmem_segment_detach (&module->seg_ds);
    } else {
        free(module->node_states);
        free(module->global_state);
        if (NULL != module->bases) {
            free(module->bases[0]);
        }
    }
    free(module->disp_units);
    free(module->outstanding_locks);
    free(module->sizes);
    free(module->bases);

    free (module->posts);

    /* cleanup */
    ompi_comm_free(&module->comm);

    OBJ_DESTRUCT(&module->lock);

    free(module);

    return OMPI_SUCCESS;
}


int
ompi_osc_fsm_set_info(struct ompi_win_t *win, struct opal_info_t *info)
{
    ompi_osc_fsm_module_t *module =
        (ompi_osc_fsm_module_t*) win->w_osc_module;

    /* enforce collectiveness... */
    return module->comm->c_coll->coll_barrier(module->comm,
                                             module->comm->c_coll->coll_barrier_module);
}


static char*
component_set_blocking_fence_info(opal_infosubscriber_t *obj, char *key, char *val)
{
    ompi_osc_fsm_module_t *module = (ompi_osc_fsm_module_t*) ((struct ompi_win_t*) obj)->w_osc_module;
/*
 * Assuming that you can't change the default.
 */
    return module->global_state->use_barrier_for_fence ? "true" : "false";
}


static char*
component_set_alloc_shared_noncontig_info(opal_infosubscriber_t *obj, char *key, char *val)
{

    ompi_osc_fsm_module_t *module = (ompi_osc_fsm_module_t*) ((struct ompi_win_t*) obj)->w_osc_module;
/*
 * Assuming that you can't change the default.
 */
    return module->noncontig ? "true" : "false";
}


int
ompi_osc_fsm_get_info(struct ompi_win_t *win, struct opal_info_t **info_used)
{
    ompi_osc_fsm_module_t *module =
        (ompi_osc_fsm_module_t*) win->w_osc_module;

    opal_info_t *info = OBJ_NEW(opal_info_t);
    if (NULL == info) return OMPI_ERR_TEMP_OUT_OF_RESOURCE;

    if (module->flavor == MPI_WIN_FLAVOR_SHARED) {
        opal_info_set(info, "blocking_fence",
                      (1 == module->global_state->use_barrier_for_fence) ? "true" : "false");
        opal_info_set(info, "alloc_shared_noncontig",
                      (module->noncontig) ? "true" : "false");
    }

    *info_used = info;

    return OMPI_SUCCESS;
}
