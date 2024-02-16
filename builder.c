#include <stdio.h>
#include <sys/stat.h>
#include "builder.h"


void serve_static(HTTP* http, RequestHandler* request_handler) { 
  
  FILE* serve_file;

  serve_file = fopen(request_handler->file_path,"r");

  if(serve_file == NULL) {
    perror("file open");
    return;
  }

  struct stat st;
  stat(request_handler->file_path,&st);

  char* buffer = malloc(sizeof(char)*st.st_size);
 
  size_t success = fread(buffer,st.st_size,1,serve_file);
  
  if(success != 1){
    perror("file read");
    return;
  };

  fclose(serve_file);

  http->response.body.value = buffer;
  http->response.body.length = st.st_size;
  http->response.body.status = OK;
}
