#pragma once

#if 0
enum {
  PRIMARY_FIBER = 0,
  READ_FIBER,
  WRITE_FIBER,
  FIBER_COUNT
};

typedef struct {
  void* data;
  uint32_t size;
} Buffer;

typedef struct {
  Buffer rwbuffer;
  void* fibers[FIBER_COUNT];
  uint32_t bytes_read;
} Shared;

typedef struct {
  Shared* shared;
  DWORD parameter;          // DWORD parameter to fiber (unused)
  DWORD fiber_result_code;  // GetLastError() result code
  HANDLE file;              // handle to operate on
  DWORD bytes_processed;    // number of bytes processed
} FiberData;

void panic(const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  fputs("Error: ", stderr);
  vfprintf(stderr, fmt, arg);
  va_end(arg);
  exit(1);
}

void* allocate(size_t bytes) {
  void* mem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
  if (!mem) {
    panic("HeapAlloc error (%d)\n", GetLastError());
  }
  return mem;
}

void deallocate(void* ptr) {
  if (ptr) {
    HeapFree(GetProcessHeap(), 0, ptr);
  }
}

void display_fiber_info() {
  FiberData* fd = (FiberData*)GetFiberData();
  void* current_fiber = GetCurrentFiber();

  // Determine which fiber is executing, based on the fiber address
  if (current_fiber == fd->shared->fibers[READ_FIBER]) {
    printf("Read fiber entered");
  } else if (current_fiber == fd->shared->fibers[WRITE_FIBER]) {
    printf("Write fiber entered");
  } else if (current_fiber == fd->shared->fibers[PRIMARY_FIBER]) {
    printf("Primary fiber entered");
  } else {
    printf("Unknown fiber entered");
  }

  printf(" (parameter is 0x%lx)\n", fd->parameter);
}

void __stdcall read_fiber_func(void* param) {
  FiberData* fd = (FiberData*)param;
  if (!fd) {
    printf("Passed NULL fiber data; exiting current thread.\n");
    return;
  }

  display_fiber_info();

  fd->bytes_processed = 0;

  while (1) {
    if (!ReadFile(fd->file, fd->shared->rwbuffer.data, fd->shared->rwbuffer.size, &fd->shared->bytes_read, NULL)) {
      break;
    }

    if (fd->shared->bytes_read == 0) break;

    fd->bytes_processed += fd->shared->bytes_read;

    SwitchToFiber(fd->shared->fibers[WRITE_FIBER]);
  }

  fd->fiber_result_code = GetLastError();
  SwitchToFiber(fd->shared->fibers[PRIMARY_FIBER]);
}

void __stdcall write_fiber_func(void* param) {
  FiberData* fd = (FiberData*)param;
  if (!fd) {
    printf("Passed NULL fiber data; exiting current thread.\n");
    return;
  }

  display_fiber_info();

  fd->bytes_processed = 0;
  fd->fiber_result_code = ERROR_SUCCESS;

  DWORD bytes_written = 0;
  while (1) {
    if (!WriteFile(fd->file, fd->shared->rwbuffer.data, fd->shared->bytes_read, &bytes_written, NULL)) {
      break;
    }

    fd->bytes_processed += bytes_written;
    SwitchToFiber(fd->shared->fibers[READ_FIBER]);
  }

  fd->fiber_result_code = GetLastError();
  SwitchToFiber(fd->shared->fibers[PRIMARY_FIBER]);
}


const uint32_t buffer_size = 32768;
  Shared shared = {
    .rwbuffer = {
      .data = allocate(buffer_size),
      .size = buffer_size,
    }
  };

  FiberData* fd = (FiberData*)allocate(sizeof(FiberData) * FIBER_COUNT);
  for (size_t i = 0; i < FIBER_COUNT; ++i) {
    fd[i].shared = &shared;
  }

  const char* source_file = "../in.txt";
  const char* dest_file = "../out.txt";

  fd[READ_FIBER].file = CreateFileA(source_file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN,NULL);
  if (fd[READ_FIBER].file == INVALID_HANDLE_VALUE) {
    panic("CreateFile error (%d)\n", GetLastError());
  }

  fd[WRITE_FIBER].file = CreateFileA(dest_file, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_FLAG_SEQUENTIAL_SCAN,NULL);
  if (fd[WRITE_FIBER].file == INVALID_HANDLE_VALUE) {
    panic("CreateFile error (%d)\n", GetLastError());
  }

  // @NOTE: Main thread must be converted into a fiber before it can
  // schedule other fibers.
  shared.fibers[PRIMARY_FIBER] = ConvertThreadToFiber(&fd[PRIMARY_FIBER]);
  if (!shared.fibers[PRIMARY_FIBER]) {
    panic("ConvertThreadToFiber error (%d)\n", GetLastError());
  }

  shared.fibers[READ_FIBER] = CreateFiber(0, read_fiber_func, &fd[READ_FIBER]);
  if (!shared.fibers[READ_FIBER]) {
    panic("CreateFiber error (%d)\n", GetLastError());
  }
  fd[READ_FIBER].parameter = 0x12345678;

  shared.fibers[WRITE_FIBER] = CreateFiber(0, write_fiber_func, &fd[WRITE_FIBER]);
  if (!shared.fibers[WRITE_FIBER]) {
    panic("CreateFiber error (%d)\n", GetLastError());
  }
  fd[WRITE_FIBER].parameter = 0x54545454;

  SwitchToFiber(shared.fibers[READ_FIBER]);

  printf("ReadFiber: result code is %lu, %lu bytes processed\n",
    fd[READ_FIBER].fiber_result_code, fd[READ_FIBER].bytes_processed);

  printf("WriteFiber: result code is %lu, %lu bytes processed\n",
    fd[WRITE_FIBER].fiber_result_code, fd[WRITE_FIBER].bytes_processed);

  DeleteFiber(shared.fibers[READ_FIBER]);
  DeleteFiber(shared.fibers[WRITE_FIBER]);
  CloseHandle(fd[READ_FIBER].file);
  CloseHandle(fd[WRITE_FIBER].file);

  deallocate(fd);
  deallocate(shared.rwbuffer.data);
#endif