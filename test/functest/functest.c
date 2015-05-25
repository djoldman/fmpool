#include <time.h>
#include <stdio.h>

#include "fmpool.h"

typedef struct Point_s
{
  double x;
  double y;
} Point_t;

FMPOOL_INIT(Point_t)

#define FAILTEST(testcond,failure_msg) \
   if((testcond)) { \
       fputs(failure_msg,stderr); fputc('\n',stderr); \
       exit(EXIT_FAILURE); \
   }

static void zero_allocation_test(const char* failure_msg) {
  fmpool_t(Point_t)* p;
  FAILTEST(((p = fmpool_create(Point_t,0)) != NULL),failure_msg);
  if(p) {
    fmpool_destroy(Point_t, p);
  }
}

static void* allocate_test(const size_t num,
                             bool (*testfn)(fmpool_t(Point_t)*),
                             const char* failure_msg)
{
  fmpool_t(Point_t)* p;
  FAILTEST(((p = fmpool_create(Point_t,num)) == NULL),failure_msg);
  FAILTEST((testfn(p) == false),failure_msg);
  fmpool_destroy(Point_t, p);
  return NULL;
}

static bool one_allocation_test(fmpool_t(Point_t)* pool) {
    return(fmpool_get(Point_t,pool) != NULL &&
           fmpool_get(Point_t,pool) == NULL);
}

static bool freelist_simple_test(fmpool_t(Point_t)* pool) {
    Point_t* p = fmpool_get(Point_t,pool);
    (void)fmpool_get(Point_t,pool);
    fmpool_free(Point_t,p,pool);
    return(p == fmpool_get(Point_t,pool));
}

int main()
{
  zero_allocation_test("Allocating zero-length pool isn't allowed");
  allocate_test(1,one_allocation_test, "Failed allocating one-length pool");
  allocate_test(2,freelist_simple_test,"Failed freelist simple test");
  return 0;
}

