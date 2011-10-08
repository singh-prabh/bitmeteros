#include "common.h"
#ifdef _WIN32
	#include <winsock2.h>
#endif
#ifndef BMWS_H
#define BMWS_H

#define SMALL_BUFSIZE 256
#define BUFSIZE 4096
#define SUBST_BUFSIZE 20480

#define DEFAULT_PORT 2605
#define MIN_PORT 1
#define MAX_PORT 65535

#define MIME_JSON "application/json"
#define MIME_HTML "text/html"
#define MIME_TEXT "text/plain"
#define MIME_CSV  "text/csv"
#define MIME_JPEG "image/jpeg"
#define MIME_GIF  "image/gif"
#define MIME_PNG  "image/png"
#define MIME_ICO  "image/vnd.microsoft.icon"
#define MIME_JS   "application/x-javascript"
#define MIME_CSS  "text/css"
#define MIME_BIN  "application/octet-stream"
#define MIME_XML  "application/xhtml+xml"

#define SYNC_CONTENT_TYPE   "application/vnd.codebox.bitmeter-sync"
#define HEADER_CONTENT_TYPE "Content-Type"
#define HTTP_EOL "\r\n"

struct HttpResponse{
    int   code;
    char* msg;
};

struct Request{
	char* method;
    char* path;
    struct NameValuePair* params;
    struct NameValuePair* headers;
};
struct Request* parseRequest(char* requestTxt);
void freeRequest(struct Request* request);

struct MimeType{
	char* fileExt;
	char* contentType;
	int binary;
};

#ifdef _WIN32
	void initMutex();
	void waitForMutex();
	void releaseMutex();
	void setupWeb();
	void shutdownWeb();
	void processWeb();
	#define WEB_SERVICE_NAME "BitMeterWebService"
#endif

void processMonitorRequest(SOCKET fd, struct Request* req);
void processMobileMonitorRequest(SOCKET fd, struct Request* req);
void processMobileMonitorAjaxRequest(SOCKET fd, struct Request* req);
void processSummaryRequest(SOCKET fd, struct Request* req);
void processMobileSummaryRequest(SOCKET fd, struct Request* req);
void processQueryRequest(SOCKET fd, struct Request* req);
void processSyncRequest(SOCKET fd, struct Request* req);
void processConfigRequest(SOCKET fd, struct Request* req, int allowAdmin);
void processExportRequest(SOCKET fd, struct Request* req);
void processAlertRequest(SOCKET fd, struct Request* req, int allowAdmin);
void processFileRequest(SOCKET fd, struct Request* req, struct NameValuePair* substPairs);
void processRssRequest(SOCKET fd, struct Request* req);
struct NameValuePair* makeRssRequestValues();

void writeText(SOCKET fd, char* txt);
void writeData(SOCKET fd, char* data, int len);
void writeDataToJson(SOCKET fd, struct Data* data);
void writeSingleDataToJson(SOCKET fd, struct Data* data);
void writeTextValueToJson(SOCKET fd, char* key, char* value);
void writeTextArrayToJson(SOCKET fd, char* key, char** values);
void writeNumValueToJson(SOCKET fd, char* key, BW_INT value);
void writeSyncData(SOCKET fd, struct Data* data);
void writeHeadersOk(SOCKET fd, char* contentType, int endHeaders);
void writeHeadersNotFound(SOCKET fd, char* file);
void writeHeadersForbidden(SOCKET fd, char* request);
void writeHeadersNotAllowed(SOCKET fd, char* httpMethod);
void writeHeadersServerError(SOCKET fd, char* msg, ...);
void writeHeader(SOCKET fd, char* name, char* value);
void writeEndOfHeaders(SOCKET fd);
void processRequest(SOCKET fd, char* buffer, int allowAdmin);

void getWebRoot(char* path);

struct HandleConfigCalls {
	void (*writeHeadersOk)(SOCKET fd, char* contentType, int endHeaders);
	void (*writeText)(SOCKET fd, char* txt);
	void (*writeHeadersServerError)(SOCKET fd, char* msg, ...);
	void (*writeHeadersForbidden)(SOCKET fd, char* request);
};
struct HandleConfigCalls mockHandleConfigCalls;

struct HandleSummaryCalls {
	void (*writeHeadersOk)(SOCKET fd, char* contentType, int endHeaders);
	void (*writeText)(SOCKET fd, char* txt);
	void (*writeNumValueToJson)(SOCKET fd, char* key, BW_INT value);
};
struct HandleSummaryCalls mockHandleSummaryCalls;

struct HandleFileCalls {
	int (*fread)(void*, size_t, size_t, FILE*);
	void (*writeData)(SOCKET fd, char* data, int len);
};
struct HandleFileCalls mockHandleFileCalls;

struct HandleQueryCalls {
	void (*writeHeadersServerError)(SOCKET fd, char* msg, ...);
	void (*writeHeadersOk)(SOCKET fd, char* contentType, int endHeaders);
	void (*writeHeader)(SOCKET fd, char* name, char* value);
	void (*writeEndOfHeaders)(SOCKET fd);
	void (*writeDataToJson)(SOCKET fd, struct Data* data);
	void (*writeText)(SOCKET fd, char* txt);
};
struct HandleQueryCalls mockHandleQueryCalls;

struct HandleMonitorCalls {
	void (*writeHeadersServerError)(SOCKET fd, char* msg, ...);
	void (*writeHeadersOk)(SOCKET fd, char* contentType, int endHeaders);
	void (*writeDataToJson)(SOCKET fd, struct Data* data);
	void (*writeText)(SOCKET fd, char* txt);
};
struct HandleMonitorCalls mockHandleMonitorCalls;

struct HandleAlertCalls {
	void (*writeHeadersServerError)(SOCKET fd, char* msg, ...);
	void (*writeHeadersForbidden)(SOCKET fd, char* request);
	void (*writeHeadersOk)(SOCKET fd, char* contentType, int endHeaders);
	void (*writeText)(SOCKET fd, char* txt);
	void (*writeNumValueToJson)(SOCKET fd, char* key, BW_INT value);
	void (*writeTextValueToJson)(SOCKET fd, char* key, char* value);
	void (*writeTextArrayToJson)(SOCKET fd, char* key, char** values);
};
struct HandleAlertCalls mockHandleAlertCalls;
#endif
