#ifndef HTTP_H
#define HTTP_H


#include <stdlib.h>



// Request Portion

typedef struct RequestRoute RequestRoute;

struct RequestRoute {
  char value[100];
  size_t length; 
};

typedef struct RequestHost RequestHost;

struct RequestHost {
  char name[100];
  size_t length;
};


// Will add more later
// Just trying to serve static HTML for now
enum RequestMethod {
  GET
};

typedef enum RequestMethod RequestMethod;


typedef struct HTTPRequest HTTPRequest;

struct HTTPRequest {
  char raw[9000];
  RequestMethod method;
  RequestRoute route;
  RequestHost host;
  int current_position;
};



// Response Portion

// Will add more later
// Just trying to serve static HTML for now
enum StatusType {
  OK,
  NOT_FOUND
};

typedef enum StatusType StatusType;


enum ContentType {
  TEXT_PLAIN,
  TEXT_HTML,
  TEXT_CSS,
  TEXT_JS
};

typedef enum ContentType ContentType;



typedef struct ResponseHeader ResponseHeader;

struct ResponseHeader {
  char value[500];
  size_t length;
};


typedef struct ResponseBody ResponseBody;

struct ResponseBody {
  char* value;
  size_t length;
  StatusType status;
  size_t total_sent;
};


typedef struct HTTPResponse HTTPResponse;

struct HTTPResponse {
  ResponseHeader header;
  ResponseBody body;
};


typedef struct HTTP HTTP;

struct HTTP {
  HTTPRequest request;
  HTTPResponse response;
};



// Handler Section

typedef struct RequestHandler RequestHandler;

struct RequestHandler {
  char* request_path;
  char* file_path;
  char* alias;
  void (*builder)(HTTP* http, RequestHandler* request_handler);
  ContentType file_type; 
};


typedef struct RequestWrangler RequestWrangler;

struct RequestWrangler {
  RequestHandler* handlers;
  size_t length;
};


// Server Functions


int start(char* port, int backlog);

void multiplexer(int sockfd, RequestWrangler* request_wrangler);


// Request & Response Functions


void process_request(int newfd, RequestWrangler* request_wrangler);

void request_handler(HTTP* http, RequestWrangler* request_wrangler);

void header_builder(HTTP* http, RequestHandler request_handler);

#endif
