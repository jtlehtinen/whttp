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
#include "sync.h"

// @TODO: remove this crap
volatile bool quit = false;
Mutex quit_mutex;

struct Server {
  SOCKET socket;
};
typedef struct Server Server;

bool server_init(Server* server) {
  server->socket = INVALID_SOCKET;

  WORD version_want = MAKEWORD(2, 2);
  WSADATA data;
  int err = WSAStartup(version_want, &data);
  if (err != NO_ERROR) {
    printf("WSAStartup failed with error: %d\n", err);
    return false;
  }

  WORD version_got = data.wVersion;
  if (version_got != version_want) {
    WORD major = LOBYTE(data.wVersion);
    WORD minor = HIBYTE(data.wVersion);
    printf("Failed to find Winsock DLL supporting version 2.2, got version %d.%d\n", major, minor);
    WSACleanup();
    return false;
  }

  return true;
}

void server_deinit(Server* server) {
  if (server->socket != INVALID_SOCKET) {
    closesocket(server->socket);
  }

  WSACleanup();
}

int server_listen_and_serve(Server* server) {
  if (!server_init(server)) return 1;

  server->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server->socket == INVALID_SOCKET) {
    printf("Failed to create server socket with error: %d\n", WSAGetLastError());
    server_deinit(server);
    return 1;
  }

  // FIONBIO sets the blocking mode for the socket.
  // value == 0 => block
  // value != 0 => non-block
  u_long value = 1;
  if (ioctlsocket(server->socket, FIONBIO, &value) == SOCKET_ERROR) {
    printf("ioctlsocket failed\n");\
    server_deinit(server);
    return 1;
  }

  //fcntl(socket, F_SETFL, O_NONBLOCK);

  const uint16_t port = 3000;
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  // @TODO: Accept localhost here...
  // Use inet_addr? https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-inet_addr
  if (InetPtonW(AF_INET, L"127.0.0.1", &addr.sin_addr) != 1) {
    printf("InetPtonW failed\n");
    server_deinit(server);
    return 1;
  }

  int bind_result = bind(server->socket, (const struct sockaddr*)&addr, sizeof(addr));
	if (bind_result == SOCKET_ERROR) {
    printf("bind failed with error: %d\n", WSAGetLastError());
    server_deinit(server);
    return 1;
	}

  int listen_result = listen(server->socket, SOMAXCONN);
  if (listen_result == SOCKET_ERROR) {
    printf("listen failed with error: %d\n", WSAGetLastError());
    server_deinit(server);
    return 1;
  }

  for (;;) {
    mutex_acquire(&quit_mutex);
    const should_quit = quit;
    mutex_release(&quit_mutex);
    if (should_quit) break;

    SOCKET client_socket = accept(server->socket, 0, 0);
    if (client_socket == INVALID_SOCKET) {
      Sleep(50); // @TODO: Something better than sleep?
      continue;
    }

    char buf[4096] = {0};
    int bytes_received = recv(client_socket, buf, sizeof(buf) - 1, 0); // @TODO: Remove MSG_WAITALL...

    buf[bytes_received] = 0;
    printf("=>\n");
    printf(buf);

    const char* response =
"HTTP/1.1 200 OK\n"
"Content-Length: 5\n"
"Content-Type: text/html; charset=utf-8\n"
"Date: Fri, 27 May 2022 15:08:37 GMT\n"
"\n"
"hello";

    int bytes_sent = send(client_socket, response, (int)strlen(response), 0);
    if (bytes_sent == SOCKET_ERROR) {
      // @TODO: ...
    }

    closesocket(client_socket);
  }

  server_deinit(server);

  return 0;
}

void print_usage() {
  printf("whttp is a simple http server\n\n");
  printf("USAGE:\n");
  printf("\twhttp [OPTIONS]\n\n");
  printf("OPTIONS:\n");
  printf("\t-port int\n");
  printf("\t\tserver port\n");
}

BOOL WINAPI interrupt_handler(DWORD ctrl_type) {
  if (ctrl_type != CTRL_C_EVENT) return FALSE;

  mutex_acquire(&quit_mutex);
  quit = true;
  mutex_release(&quit_mutex);
  return TRUE;
}

//int __cdecl _tmain(int argc, TCHAR *argv[]) {
int main() {
  mutex_init(&quit_mutex);

  // @NOTE: The Visual Studio debugger might catch CTRL+C
  // even when a handler has been set for it.
  SetConsoleCtrlHandler(interrupt_handler, TRUE);

  #if 0
  print_usage();

  printf("%d %s\n", StatusOK, status_code_to_status_text(StatusOK));
  printf("%d %s\n", StatusBadRequest, status_code_to_status_text(StatusBadRequest));
  printf("%d %s\n", StatusInternalServerError, status_code_to_status_text(StatusInternalServerError));

  printf("%d %s\n", MethodGet, method_to_method_text(MethodGet));
  printf("%d %s\n", MethodPost, method_to_method_text(MethodPost));
  printf("%d %s\n", MethodDelete, method_to_method_text(MethodDelete));
  #endif

  Server server;
  int ret = server_listen_and_serve(&server);

  mutex_deinit(&quit_mutex);
  return ret;
}
