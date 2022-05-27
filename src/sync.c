#include "sync.h"

void mutex_init(Mutex* mtx) {
  InitializeCriticalSection(&mtx->cs);
}

void mutex_deinit(Mutex* mtx) {
  DeleteCriticalSection(&mtx->cs);
}

void mutex_acquire(Mutex* mtx) {
   EnterCriticalSection(&mtx->cs);
}

bool mutex_try_acquire(Mutex* mtx) {
  return TryEnterCriticalSection(&mtx->cs) != 0;
}

void mutex_release(Mutex* mtx) {
  LeaveCriticalSection(&mtx->cs);
}
