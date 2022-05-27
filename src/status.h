#pragma once

// https://www.iana.org/assignments/http-status-codes/http-status-codes.xhtml
enum Status {
  // 1xx: Informational
  StatusContinue                      = 100,
  StatusSwitchingProtocols            = 101,
  StatusProcessing                    = 102,
  StatusEarlyHints                    = 103,

  // 2xx: Success
  StatusOK                            = 200,
  StatusCreated                       = 201,
  StatusAccepted                      = 202,
  StatusNonAuthoritativeInformation   = 203,
  StatusNoContent                     = 204,
  StatusResetContent                  = 205,
  StatusPartialContent                = 206,
  StatusMultiStatus                   = 207,
  StatusAlreadyReported               = 208,
  StatusIMUsed                        = 226,

  // 3xx: Redirection
  StatusMultipleChoices               = 300,
  StatusMovedPermanently              = 301,
  StatusFound                         = 302,
  StatusSeeOther                      = 303,
  StatusNotModified                   = 304,
  StatusUseProxy                      = 305,
  StatusTemporaryRedirect             = 307,
  StatusPermanentRedirect             = 308,

  // 4xx: Client Error
  StatusBadRequest                    = 400,
  StatusUnauthorized                  = 401,
  StatusPaymentRequired               = 402,
  StatusForbidden                     = 403,
  StatusNotFound                      = 404,
  StatusMethodNotAllowed              = 405,
  StatusNotAcceptable                 = 406,
  StatusProxyAuthenticationRequired   = 407,
  StatusRequestTimeout                = 408,
  StatusConflict                      = 409,
  StatusGone                          = 410,
  StatusLengthRequired                = 411,
  StatusPreconditionFailed            = 412,
  StatusContentTooLarge               = 413,
  StatusURITooLong                    = 414,
  StatusUnsupportedMediaType          = 415,
  StatusRangeNotSatisfiable           = 416,
  StatusExpectationFailed             = 417,
  StatusTeapot                        = 418,
  StatusMisdirectedRequest            = 421,
  StatusUnprocessableContent          = 422,
  StatusLocked                        = 423,
  StatusFailedDependency              = 424,
  StatusTooEarly                      = 425,
  StatusUpgradeRequired               = 426,
  StatusPreconditionRequired          = 428,
  StatusTooManyRequests               = 429,
  StatusRequestHeaderFieldsTooLarge   = 431,
  StatusUnavailableForLegalReasons    = 451,

  // 5xx: Server Error
  StatusInternalServerError           = 500,
  StatusNotImplemented                = 501,
  StatusBadGateway                    = 502,
  StatusServiceUnavailable            = 503,
  StatusGatewayTimeout                = 504,
  StatusHTTPVersionNotSupported       = 505,
  StatusVariantAlsoNegotiates         = 506,
  StatusInsufficientStorage           = 507,
  StatusLoopDetected                  = 508,
  StatusNotExtended                   = 510,
  StatusNetworkAuthenticationRequired = 511,
};
typedef enum Status Status;

// status_code_to_status_text returns a text for the HTTP status code.
// Returns the empty string if the status code is unknown.
const char* status_code_to_status_text(Status status_code);
