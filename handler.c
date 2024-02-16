#include <stdio.h>
#include <string.h>
#include <time.h>
#include "http.h"



void status_builder(HTTP* http) {
  
  if(http->response.body.status == OK) {
    strcat(http->response.header.value," 200 OK\r\n");
  }
  else if(http->response.body.status == NOT_FOUND) {
    strcat(http->response.header.value, "404 Not Found\r\n");  
  }
}


void date_builder(HTTP* http) {
  char response_date[30];
  time_t now = time(0);
  struct tm* t = gmtime(&now);
  
  strftime(response_date,29,"Date: %a, %d %b %Y %T",t);

  strcat(http->response.header.value,response_date);
  strcat(http->response.header.value," UTC\r\n");
}


void content_length_builder(HTTP* http, size_t file_size) {
  
  if(file_size <= 0) {
    perror("header build error");
    return;
  }

  char raw_number[15];

  snprintf(raw_number,sizeof(raw_number), "%zu", file_size);

  int len = strlen(raw_number);

  raw_number[len] = '\0';

  strcat(http->response.header.value,"Accept-Ranges: bytes\r\n");

  strcat(http->response.header.value,"Content-Length: ");

  strcat(http->response.header.value,raw_number);

  strcat(http->response.header.value,"\r\n");
}


void content_type(HTTP* http, ContentType file_type) {
 
  if(file_type == TEXT_PLAIN) {
    strcat(http->response.header.value, "Content-Type: text/plain\r\n");
  }
  else if(file_type == TEXT_HTML) {
    strcat(http->response.header.value, "Content-Type: text/html\r\n");
  }
  else if(file_type == TEXT_CSS) {
    strcat(http->response.header.value, "Content-Type: text/css\r\n");
  }
  else if(file_type == TEXT_JS) {
    strcat(http->response.header.value, "Content-Type: text/javascript\r\n");
  }
}


void header_builder(HTTP *http, RequestHandler handler) {
  
  char basic_header[] = "HTTP/1.1 ";
  strcat(http->response.header.value,basic_header);
  
  status_builder(http);

  date_builder(http);

  content_length_builder(http, http->response.body.length);

  content_type(http, handler.file_type);

  http->response.header.length = strlen(http->response.header.value);
}


int find_route(HTTP*http, RequestWrangler* request_wrangler) {
 
  size_t i = 0;
  RequestHandler handler;

  while (i < request_wrangler->length) {
    handler = request_wrangler->handlers[i];

    if(strcmp(http->request.route.value,handler.request_path) == 0) {

      return i;
    }
    
    i++;
  }
  
  return -1;
}


void no_route_handler(HTTP* http) {
    
  http->response.body.status = NOT_FOUND;
  
  strcat(http->response.header.value,"HTTP/1.1 404 Not Found\r\n");
}


void request_handler(HTTP* http, RequestWrangler* request_wrangler) {

  int route = find_route(http,request_wrangler);

  if(route == -1) {
    no_route_handler(http);
    return;
  }

  RequestHandler handler = request_wrangler->handlers[route];

  handler.builder(http, &handler);

  header_builder(http, handler);
}
