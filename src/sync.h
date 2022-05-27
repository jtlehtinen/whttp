#pragma once

#include <Windows.h>
#include <stdbool.h>

// MSVC, where is the atomics and threads support for C?
struct Mutex {
  CRITICAL_SECTION cs;
};
typedef struct Mutex Mutex;

void mutex_init(Mutex* mtx);
void mutex_deinit(Mutex* mtx);
void mutex_acquire(Mutex* mtx);
bool mutex_try_acquire(Mutex* mtx);
void mutex_release(Mutex* mtx);
