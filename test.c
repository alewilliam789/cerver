#include <string.h>
#include "http.h"
#include "builder.h"

#define PORT "3490"
#define BACKLOG 10


RequestHandler home_handler = {
  .request_path = "/",
  .file_path = "./index.html",
  .builder = serve_static,
  .file_type = TEXT_HTML
};


int main(void) {

  RequestWrangler wrangler;
  RequestHandler handlers[1];

  handlers[0] = home_handler;

  wrangler.handlers = handlers;
  wrangler.length = 1;
  
  int sockfd = start(PORT,BACKLOG);

  multiplexer(sockfd, &wrangler);

  return 1;
}
