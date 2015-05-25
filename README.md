Free-List Memory Pool
-------
This is a C99 header library that provides efficient type-generic code to allocate heap memory from the OS for user-defined number of objects.  Achieved through the magic of C macros and [free lists](http://en.wikipedia.org/wiki/Free_list).

This library is built for situations where programs repeatedly call malloc/calloc/new/etc. for small data structures, followed closely by free/delete.  Once a pool is created, getting a pointer from the pool to an unused object costs a couple of machine instructions.  Boost memory pools and others are overkill sometimes.

Limitations:
   * Memory pools are statically-sized.

Inspired by [computer game particles](http://gameprogrammingpatterns.com/object-pool.html).

## Try it now
```sh
make; ./bin/perftest
```

## Example
```c
#include "fmpool.h"

/* define a "point" struct with x and y coordinates as doubles */
typedef struct Point_s
{
  double x;
  double y;
} Point_t;

/* macro that initializes fmpool functions that take Point_t structs as
   arguments - compile-time only */
FMPOOL_INIT(Point_t)

#define NUM_POINTS 16

int main()
{
  /* create pool - requests memory from OS */
  fmpool_t(Point_t)* Pool;
  Pool = fmpool_create(Point_t, NUM_POINTS);
  
  /* grab some pointers to allocated memory from the pool */
  Point_t* PointArray[NUM_POINTS] = {NULL};
  for(size_t i = 0; i < NUM_POINTS; i++)
  {
    Point_t* Point = fmpool_get(Point_t, Pool);
    if(Point)
      PointArray[i] = Point;
  }
  /* release some points back to the pool */
  for(size_t i = 0; i < NUM_POINTS - 5; i++)
    fmpool_free(Point_t, PointArray[i], Pool);

  /* destroy pool when done - releases memory to OS */
  fmpool_destroy(Point_t, Pool);
  return 0;
}
```

## Details
FMPool requests a contiguous block of memory once during creation with `fmpool_create`.  Afterwards, pointers to unused objects are provided with `fmpool_get`.  Objects can be released back to the pool with `fmpool_free` and subsequently re-requested.  When finished, the memory is released back to the OS with `fmpool_destroy`.

## Functions
#### fmpool_create(TYPE, NUM)
Creates a new memory pool.  The new pool has `NUM` objects of type `TYPE`.
>**Return Values**  
>On success: returns a pointer to the new fmpool.  
>On failure: returns a `NULL` pointer.  
  
#### fmpool_destroy(TYPE, POOL)
Frees all memory associated with `POOL`.
>**Return Values**  
>Returns no value. (mimics C99 standard).
  
#### fmpool_get(TYPE, POOL)
Grabs an unused object from the pool.
>**Return Values**  
>On success: returns a `TYPE` pointer.  
>On failure: returns a `NULL` pointer.
  
#### fmpool_free(TYPE, OBJ, POOL)
If `OBJ` is controlled by `POOL`, sets it as unused.  Does not zero memory.
>**Return Values**  
>On success: returns `true`.  
>On failure: returns `false`.
  
## License
[MIT License](http://dn.mit-license.org)

