#include "status.h"

const char* status_code_to_status_text(Status status_code) {
  switch (status_code) {
    // 1xx: Informational
    case StatusContinue: return "Continue";
    case StatusSwitchingProtocols: return "Switching Protocols";
    case StatusProcessing: return "Processing";
    case StatusEarlyHints: return "Early Hints";

    // 2xx: Success
    case StatusOK: return "OK";
    case StatusCreated: return "Created";
    case StatusAccepted: return "Accepted";
    case StatusNonAuthoritativeInformation: return "Non-Authoritative Information";
    case StatusNoContent: return "No Content";
    case StatusResetContent: return "Reset Content";
    case StatusPartialContent: return "Partial Content";
    case StatusMultiStatus: return "Multi-Status";
    case StatusAlreadyReported: return "Already Reported";
    case StatusIMUsed: return "IM Used";

    // 3xx: Redirection
    case StatusMultipleChoices: return "Multiple Choices";
    case StatusMovedPermanently: return "Moved Permanently";
    case StatusFound: return "Found";
    case StatusSeeOther: return "See Other";
    case StatusNotModified: return "Not Modified";
    case StatusUseProxy: return "Use Proxy";
    case StatusTemporaryRedirect: return "Temporary Redirect";
    case StatusPermanentRedirect: return "Permanent Redirect";

    // 4xx: Client Error
    case StatusBadRequest: return "Bad Request";
    case StatusUnauthorized: return "Unauthorized";
    case StatusPaymentRequired: return "Payment Required";
    case StatusForbidden: return "Forbidden";
    case StatusNotFound: return "Not Found";
    case StatusMethodNotAllowed: return "Method Not Allowed";
    case StatusNotAcceptable: return "Not Acceptable";
    case StatusProxyAuthenticationRequired: return "Proxy Authentication Required";
    case StatusRequestTimeout: return "Request Timeout";
    case StatusConflict: return "Conflict";
    case StatusGone: return "Gone";
    case StatusLengthRequired: return "Length Required";
    case StatusPreconditionFailed: return "Precondition Failed";
    case StatusContentTooLarge: return "Content Too Large";
    case StatusURITooLong: return "URI Too Long";
    case StatusUnsupportedMediaType: return "Unsupported Media Type";
    case StatusRangeNotSatisfiable: return "Range Not Satisfiable";
    case StatusExpectationFailed: return "Expectation Failed";
    case StatusMisdirectedRequest: return "Misdirected Request";
    case StatusUnprocessableContent: return "Unprocessable Content";
    case StatusLocked: return "Locked";
    case StatusFailedDependency: return "Failed Dependency";
    case StatusTooEarly: return "Too Early";
    case StatusUpgradeRequired: return "Upgrade Required";
    case StatusPreconditionRequired: return "Precondition Required";
    case StatusTooManyRequests: return "Too Many Requests";
    case StatusRequestHeaderFieldsTooLarge: return "Request Header Fields Too Large";
    case StatusUnavailableForLegalReasons: return "Unavailable For Legal Reasons";

    // 5xx: Server Error
    case StatusInternalServerError: return "Internal Server Error";
    case StatusNotImplemented: return "Not Implemented";
    case StatusBadGateway: return "Bad Gateway";
    case StatusServiceUnavailable: return "Service Unavailable";
    case StatusGatewayTimeout: return "Gateway Timeout";
    case StatusHTTPVersionNotSupported: return "HTTP Version Not Supported";
    case StatusVariantAlsoNegotiates: return "Variant Also Negotiat";
    case StatusInsufficientStorage: return "Insufficient Storage";
    case StatusLoopDetected: return "Loop Detected";
    case StatusNotExtended: return "Not Extended";
    case StatusNetworkAuthenticationRequired: return "Network Authentication Required";

    default: return "";
  }
}
