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
