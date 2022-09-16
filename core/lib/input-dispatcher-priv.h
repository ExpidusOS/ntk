#pragma once

#include <ntk/context.h>

typedef struct _NtkInputDispatcherPrivate {
  GMutex running;
  NtkContext* ctx;
} NtkInputDispatcherPrivate;
