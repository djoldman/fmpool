#include <time.h>
#include <stdio.h>

#include "fmpool.h"

#define NOINLINE __attribute__((noinline))

typedef struct Point_s
{
  double x;
  double y;
} Point_t;

FMPOOL_INIT(Point_t)

static NOINLINE void* pool_test_func(const size_t num)
{
  void* save = NULL;
  fmpool_t(Point_t)* P;
  P = fmpool_create(Point_t, num);
  for(size_t i = 0; i < num; i++)
  {
    Point_t* item = fmpool_get(Point_t, P);
    if(item)
    {
      save = item;
    }
    else
    {
      perror("pool malloc");
      exit(1);
    }
  }
  fmpool_destroy(Point_t, P);
  return save;
}

static NOINLINE void* malloc_test_func(const size_t num)
{
  void* save = NULL;
  for(size_t i = 0; i < num; i++)
  {
    Point_t* item;
    item = malloc(sizeof(Point_t));
    if(item)
    {
      save = item;
    }
    else
    {
      perror("regular malloc");
      exit(1);
    }
    free(item);
  }
  return save;
}

/* Signature of a function that takes a size_t and returns a void pointer */
typedef void* TEST_FUNC_SIG (const size_t);

static NOINLINE void Test(TEST_FUNC_SIG* func, char* text)
{
  clock_t begin, end;
  double ms_elapsed;
  void* res = NULL;
  for(size_t i = 1000; i < 1000000; i*=10)
  {
    begin = clock();
    res = func(i);
    end = clock();

    ms_elapsed = (double)(end - begin) / CLOCKS_PER_SEC / 0.001;

    printf("%f ms:\t%s time to create and destroy %lu objects.  ", 
        ms_elapsed, text, i);
    printf("last pointer: %p", res);
    printf("\n");
  }
}

static NOINLINE void DoTests(void)
{
  TEST_FUNC_SIG* func;

  func = &pool_test_func;
  Test(func, "FMPool");

  func = &malloc_test_func;
  Test(func, "Malloc");
}

int main()
{
  DoTests();

  return 0;
}

