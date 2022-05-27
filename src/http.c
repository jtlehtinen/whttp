#include "http.h"

const char* method_to_method_text(Method http_method) {
  switch (http_method) {
    case MethodGet:     return "GET";
    case MethodPut:     return "PUT";
    case MethodPost:    return "POST";
    case MethodPatch:   return "PATCH";
    case MethodDelete:  return "DELETE";
    case MethodHead:    return "HEAD";
    case MethodConnect: return "CONNECT";
    case MethodOptions: return "OPTIONS";
    case MethodTrace:   return "TRACE";
    default:            return "";
  }
}
