#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include "http.h"


void parse_route(HTTPRequest* http) {
  char current_character;

  int j = 0;
  while(true) {
    current_character = http->raw[http->current_position];
    
    if(isalnum(current_character) || ispunct(current_character)) {
      http->route.value[j] = current_character;
      http->route.length++;
      j++;
    }
    else {
      return;
    }
    http->current_position++;
  }
}


void parse_method(HTTPRequest* http) {
  char current_character;

  while(true) {
    current_character = http->raw[http->current_position];

    if(!isspace(current_character) && current_character == 'G') {
      http->method = GET;
      return;
    }
    else {
      perror("Sorry this method is not ready yet");
    }
  } 
}


void parse_http_request(HTTPRequest* http) {
  char current_character = ' ';
  http->current_position = 0;
  http->route.length = 0;

  while(current_character != '\0') {
    current_character = http->raw[http->current_position];

    if(http->current_position == 0) {
      parse_method(http); 
    }
    else if(current_character == '/' && http->route.length == 0) {
      parse_route(http);
    }

    http->current_position++;
  }
  http->route.value[http->route.length] = '\0';
}


int send_all(int response_socket, char* value, size_t length) {
  int send_length = 0;

  while((size_t)send_length < length) {
    send_length = send(response_socket,value,length,0);

    if(send_length == -1) {
      return -1;
    }
  }
  
  return 1;
}


void process_request(int newfd, RequestWrangler* request_wrangler) {
  HTTP http;

  while(true) {
    
    int recv_length = recv(newfd,http.request.raw,9000,0);
    if(recv_length == 0 ) {
      break;
    }
    else if( recv_length == -1) {
      perror("recv");
      break;
    }

    parse_http_request(&http.request);

    request_handler(&http, request_wrangler);

    int success;

    success = send_all(newfd,http.response.header.value,http.response.header.length);

    if(success == -1) {
      perror("send");
      break;
    }

    success = send_all(newfd,"\r\n",2);

    if(success == -1) {
      perror("send");
      break;
    }

    success = send_all(newfd,http.response.body.value,http.response.body.length);

    if(success == -1) {
      perror("send");
      break;
    }
   
    free(http.response.body.value);
    break;
  }
}
