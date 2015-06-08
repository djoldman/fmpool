#include <time.h>
#include <stdio.h>

#include "fmpool.h"

typedef struct Point_s
{
  int x;
  int y;
} Point_t;

FMPOOL_INIT(Point_t)

#define FAILTEST(testcond,failure_msg) \
   if((testcond)) { \
       fputs(failure_msg,stderr); fputc('\n',stderr); \
       exit(EXIT_FAILURE); \
   }

#define NUMITEMS 10
static void free_under_and_overflow_test(const char* failure_msg) {
  fmpool_t(Point_t)* pool = fmpool_create(Point_t,NUMITEMS);
  Point_t* p = fmpool_get(Point_t,pool);
  /* test the underflow case */
  FAILTEST(fmpool_free(Point_t,p-1,pool) != false,failure_msg);
  /* test the overflow case */
  FAILTEST(fmpool_free(Point_t,p+NUMITEMS,pool) != false,failure_msg);
  FAILTEST(fmpool_free(Point_t,p,pool) != true,failure_msg);
  if(pool) {
    fmpool_destroy(Point_t, pool);
  }
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

static Point_t* set_point(Point_t* p) {
    p->x = 3;
    p->y = 4;
    return p;
}

static Point_t* fail_ctor(Point_t* p) {
   return NULL;
}

static bool dtor_called = false;

static Point_t* test_dtor(Point_t* p) {
    dtor_called = true;    
    return NULL;
}

static bool ctor_dtor_test(fmpool_t(Point_t)* pool) {
    pool->ctor = set_point;
    pool->dtor = test_dtor;
    Point_t* p = fmpool_get(Point_t,pool);
    Point_t point = *p;
    fmpool_free(Point_t,p,pool);
    pool->ctor = fail_ctor;
    p = fmpool_get(Point_t,pool);
    return(point.x == 3 && point.y == 4 && dtor_called == true && p == NULL);
}

int main()
{
  zero_allocation_test("Allocating zero-length pool isn't allowed");
  allocate_test(1,one_allocation_test, "Failed allocating one-length pool");
  allocate_test(2,freelist_simple_test,"Failed freelist simple test");
  allocate_test(2,ctor_dtor_test, "Failed constructor/destructor test");
  free_under_and_overflow_test("Failed under/over flow test");
  return 0;
}

