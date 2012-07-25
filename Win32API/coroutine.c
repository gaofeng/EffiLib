#include "coroutine.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include <windows.h>

#define DEFAULT_COROUTINE 16

struct coroutine;

struct schedule {
    void* main;
    int nco;
    int cap;
    int running;
    struct coroutine **co;
};

struct coroutine {
    coroutine_func func;
    void *ud;
    void* ctx;
    struct schedule * sch;
    ptrdiff_t cap;
    ptrdiff_t size;
    int status;
    char *stack;
};

struct coroutine * _co_new(struct schedule *S , coroutine_func func, void *ud) {
    struct coroutine * co = (struct coroutine*)malloc(sizeof(*co));
    co->func = func;
    co->ud = ud;
    co->sch = S;
    co->cap = 0;
    co->size = 0;
    co->status = COROUTINE_READY;
    co->stack = NULL;
    return co;
}

void _co_delete(struct coroutine *co) {
    DeleteFiber(co->ctx);
    free(co);
}

struct schedule * coroutine_open(void) {
    struct schedule *S = (struct schedule*)malloc(sizeof(*S));
    S->nco = 0;
    S->cap = DEFAULT_COROUTINE;
    S->running = -1;
    S->co = (struct coroutine**)malloc(sizeof(struct coroutine *) * S->cap);
    memset(S->co, 0, sizeof(struct coroutine *) * S->cap);
    S->main = ConvertThreadToFiber(NULL);
    return S;
}

void coroutine_close(struct schedule *S) {
    int i;
    for (i=0;i<S->cap;i++) {
        struct coroutine * co = S->co[i];
        if (co) {
            _co_delete(co);
        }
    }
    free(S->co);
    S->co = NULL;
    free(S);
}

int coroutine_new(struct schedule *S, coroutine_func func, void *ud) {
    struct coroutine *co = _co_new(S, func , ud);
    /*超过协程允许个数，2倍增加*/
    if (S->nco >= S->cap) {
        int id = S->cap;
        S->cap *= 2;
        S->co = (struct coroutine**)realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));
        memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);
        S->co[S->cap] = co;
        S->cap *= 2;
        ++S->nco;
        return id;
    } else {
        int i;
        for (i=0;i<S->cap;i++) {
            int id = (i+S->nco) % S->cap;
            if (S->co[id] == NULL) {
                S->co[id] = co;
                ++S->nco;
                return id;
            }
        }
    }
    assert(0);
    return -1;
}

static void WINAPI fiberProc(void *fiber_nbr) 
{
    struct schedule *S = (struct schedule *)fiber_nbr;
    int id = S->running;
    struct coroutine *C = S->co[id];
    C->func(S,C->ud);
    _co_delete(C);
    S->co[id] = NULL;
    --S->nco;
    S->running = -1;
    SwitchToFiber(S->main);
}

void coroutine_resume(struct schedule * S, int id) 
{
    //         assert(S->running == -1);
    //         assert(id >=0 && id < S->cap);
    struct coroutine *C = S->co[id];
    int status;
    if (C == NULL)
        return;
    status = C->status;
    switch(status) {
    case COROUTINE_READY:
        C->ctx = CreateFiber(0, fiberProc, (void*)S);
    case COROUTINE_SUSPEND:
        S->running = id;
        C->status = COROUTINE_RUNNING;
        SwitchToFiber(C->ctx);
        break;
    default:
        assert(0);
    }
}

void coroutine_yield(struct schedule * S) 
{
    int id = S->running;
    struct coroutine* C;
    assert(id >= 0);
    C = S->co[id];
    C->status = COROUTINE_SUSPEND;
    S->running = -1;
    SwitchToFiber(S->main);
}

int coroutine_status(struct schedule * S, int id) 
{
    assert(id>=0 && id < S->cap);
    if (S->co[id] == NULL) {
        return COROUTINE_DEAD;
    }
    return S->co[id]->status;
}

int coroutine_running(struct schedule * S) {
    return S->running;
}

