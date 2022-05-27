#pragma comment(lib, "ws2_32.lib")

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <tchar.h>

#include "http.h"

struct Server {
  int pad;
};
typedef struct Server Server;

bool server_init(Server* server) {
  WORD version_want = MAKEWORD(2, 2);
  WSADATA data;
  int err = WSAStartup(version_want, &data);
  if (err != 0) {
    printf("WSAStartup failed with error: %d\n", err);
    return false;
  }

  WORD version_got = data.wVersion;
  if (version_got != version_want) {
    WORD major = LOBYTE(data.wVersion);
    WORD minor = HIBYTE(data.wVersion);
    printf("Could not find Winsock DLL supporting version 2.2, got version %d.%d\n", major, minor);
    WSACleanup();
    return false;
  }

  return true;
}

void server_deinit(Server* server) {
  WSACleanup();
}

void print_usage() {
  printf("whttp is a simple http server\n\n");
  printf("USAGE:\n");
  printf("\twhttp [OPTIONS]\n\n");
  printf("OPTIONS:\n");
  printf("\t-port int\n");
  printf("\t\tserver port\n");
}

int __cdecl _tmain(int argc, TCHAR *argv[]) {
  print_usage();

  printf("%d %s\n", StatusOK, status_code_to_status_text(StatusOK));
  printf("%d %s\n", StatusBadRequest, status_code_to_status_text(StatusBadRequest));
  printf("%d %s\n", StatusInternalServerError, status_code_to_status_text(StatusInternalServerError));

  printf("%d %s\n", MethodGet, method_to_method_text(MethodGet));
  printf("%d %s\n", MethodPost, method_to_method_text(MethodPost));
  printf("%d %s\n", MethodDelete, method_to_method_text(MethodDelete));

  Server server;
  if (!server_init(&server)) {
    return 1;
  }

  server_deinit(&server);

  return 0;
}
