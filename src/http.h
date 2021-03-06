#pragma once

#include "status.h"

// HTTP methods.
enum Method {
  MethodGet = 0,
  MethodPut,
  MethodPost,
  MethodPatch,
  MethodDelete,
  MethodHead,
  MethodOptions,
  MethodConnect,
  MethodTrace,
};
typedef enum Method Method;

// method_to_method_text returns a text for the HTTP method.
// Returns the empty string if the HTTP method is unknown.
const char* method_to_method_text(Method http_method);

struct Request {
  int dummy;
};
typedef struct Request Request;

struct Response {
  int dummy;
};
typedef struct Response Response;

typedef void (*RequestHandler)(const Request* request, Response* response);
