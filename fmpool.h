/*
  The MIT License (MIT)
  Copyright © 2015-2022 David Newman <davidjndev@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy 
  of this software and associated documentation files (the “Software”), to deal 
  in the Software without restriction, including without limitation the rights 
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
  copies of the Software, and to permit persons to whom the Software is 
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
  SOFTWARE.
*/

#ifndef _FMPOOL_H_INCLUDED_
#define _FMPOOL_H_INCLUDED_

#include <stddef.h> /* size_t */
#include <stdlib.h> /* calloc, free */
#include <stdbool.h> /* bool */

#define FMPOOL_INLINE __attribute__((always_inline)) /* TODO: ? */

#define FMPOOL_INIT(TYPE)	\
  typedef union fmpool_##TYPE##_item_s fmpool_##TYPE##_item_t; \
  union fmpool_##TYPE##_item_s \
  { \
    fmpool_##TYPE##_item_t* next; \
    TYPE data; \
  }; \
  typedef struct fmpool_##TYPE##_s \
  { \
    fmpool_##TYPE##_item_t* items; \
    fmpool_##TYPE##_item_t* head; \
    size_t num; \
  } fmpool_##TYPE##_t; \
  static FMPOOL_INLINE fmpool_##TYPE##_t* \
  fmpool_##TYPE##_create(const size_t num) \
  { \
    if(num == 0) \
    { \
      return NULL; /* creating pool with zero items */ \
    } \
    fmpool_##TYPE##_t* P; \
    P = calloc(1, sizeof(fmpool_##TYPE##_t)); \
    if(P == NULL) \
    { \
      return NULL; /* calloc failed */ \
    } \
    P->items = calloc(num, sizeof(fmpool_##TYPE##_item_t)); \
    if(P->items == NULL) \
    { \
      free(P); \
      return NULL; /* calloc failed */ \
    } \
    P->head = &P->items[0]; \
    P->num = num; \
    for(size_t i = 0; i < num - 1; i++) \
    { \
      P->items[i].next = &P->items[i + 1]; \
    } \
    P->items[num - 1].next = NULL; \
    return P; \
  } \
  static FMPOOL_INLINE void fmpool_##TYPE##_destroy(fmpool_##TYPE##_t* P) \
  { \
    free(P->items); \
    free(P); \
  } \
  static FMPOOL_INLINE TYPE* fmpool_##TYPE##_get(fmpool_##TYPE##_t* P) \
  { \
    fmpool_##TYPE##_item_t* item = P->head; \
    if(item == NULL) \
    { \
      return NULL; \
    } \
    P->head = item->next; \
    return &item->data; \
  } \
  static FMPOOL_INLINE bool fmpool_##TYPE##_free(TYPE* OBJ, \
      fmpool_##TYPE##_t* P) \
  { \
    fmpool_##TYPE##_item_t* I = (fmpool_##TYPE##_item_t*)OBJ;\
    if((I < P->items) || (I >= (P->items + P->num))) \
    { \
      return false; \
    } \
    I->next = P->head; \
    P->head = I; \
    return true; \
  }

#define fmpool_t(TYPE) fmpool_##TYPE##_t
#define fmpool_create(TYPE, NUM) fmpool_##TYPE##_create(NUM)
#define fmpool_destroy(TYPE, POOL) fmpool_##TYPE##_destroy(POOL)
#define fmpool_get(TYPE, POOL) fmpool_##TYPE##_get(POOL)
#define fmpool_free(TYPE, OBJ, POOL) fmpool_##TYPE##_free(OBJ, POOL)

#endif // _FMPOOL_H_INCLUDED_

