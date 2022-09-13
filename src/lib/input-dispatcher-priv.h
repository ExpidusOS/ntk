#ifndef __INPUT_DISPATCHER_PRIV_H_
#define __INPUT_DISPATCHER_PRIV_H_ 1

#include <ntk/context.h>

typedef struct _NtkInputDispatcherPrivate {
  GMutex running;
  NtkContext* ctx;
} NtkInputDispatcherPrivate;

#endif
