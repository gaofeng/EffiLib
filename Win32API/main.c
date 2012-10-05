
#define _WIN32_WINNT 0x400
#include <stdio.h>
#include <windows.h>

#define FIBER_COUNT 10
void *fiber_context[FIBER_COUNT];
VOID WINAPI fiberProc(void*);

int fiber_test()
{
    int i;
    int fibers[FIBER_COUNT];

    for (i = 0; i < FIBER_COUNT; i++)
        fibers[i] = i;

    fiber_context[0] = ConvertThreadToFiber(NULL);

    for (i = 1; i < FIBER_COUNT; ++i) {
        fiber_context[i] = CreateFiber(0, fiberProc, &fibers[i]);
        if (fiber_context[i] != NULL)
            printf("Fiber %d created\n", i);
    }

    for (i = 1; i < FIBER_COUNT; ++i) {
        if (fiber_context[i] != NULL)
            SwitchToFiber(fiber_context[i]);
    }

    printf("Over\n");
    return 0;
}

void WINAPI fiberProc(void *fiber_nbr)
{
    int nbr;
    nbr = *((int*)fiber_nbr);

    printf("Hello from fiber %d\n", nbr);
    SwitchToFiber(fiber_context[0]);
}

#include "coroutine.h"

struct args {
    int n;
};

static void foo(struct schedule * S, void *ud) {
    struct args* arg = (struct args*)ud;
    int start = arg->n;
    int i;

    for (i=0;i<5;i++)
    {
        printf("coroutine %d : %d\n",coroutine_running(S) , start + i);
        coroutine_yield(S);
    }
}

static void test(struct schedule *S)
{
    struct args arg1 = { 0 };
    struct args arg2 = { 100 };

    int co1 = coroutine_new(S, foo, &arg1);
    int co2 = coroutine_new(S, foo, &arg2);
    printf("main start\n");
    while (coroutine_status(S,co1) && coroutine_status(S,co2)) 
    {
        coroutine_resume(S,co1);
        coroutine_resume(S,co2);
    } 
    printf("main end\n");
}

typedef struct 
{
    int* data;
    int count;
    int temp;
} ArrayType;

void ArrayInit(ArrayType* array, int count)
{
    int i = 0;
    array->count = count;
    array->data = (int*)malloc(sizeof(int) * count);
    array->temp = 0;
    for (i = 0; i < count; i++)
    {
        array->data[i] = i + 1;
    }
}

void PrintArray(ArrayType* a)
{
    int i = 0;
    for (i = 0; i < a->count; i++)
    {
        printf("%4d", a->data[i]);
    }
    printf("\n");
    
}

void Permutations(struct schedule* s, void* para)
{
    ArrayType* array = (ArrayType*)para;
    int i;
    int temp = 0;

    if (array->temp <= 1)
    {
        coroutine_yield(s);
        return;
    }

    for (i = 0; i < array->count; i++)
    {
        temp = array->data[array->count - 1];
        array->data[array->count - 1] = array->data[i];
        array->data[i] = temp;
        array->temp--;
        Permutations(s, para);
        array->temp++;
        temp = array->data[array->count - 1];
        array->data[array->count - 1] = array->data[i];
        array->data[i] = temp;
    }
    
}

int main() {
    ArrayType array;
    struct schedule * S = coroutine_open();
    int id = 0;
    ArrayInit(&array, 5);

    array.temp = array.count;
    id = coroutine_new(S, Permutations, (void*)&array);

    while (coroutine_status(S, id))
    {    
        coroutine_resume(S, id);
        PrintArray(&array);
    }

    coroutine_close(S);

    return 0;
}