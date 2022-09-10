#ifndef __CONTEXT_PRIVATE_H_
#define __CONTEXT_PRIVATE_H_ 1

#include <nuklear.h>
#include <ntk/renderer.h>

typedef struct _NtkContextPrivate {
	struct nk_context nk;
	NtkRenderer* renderer;
} NtkContextPrivate;

#endif
