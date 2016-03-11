#include <evhttp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "listen.h"
#include "events.h"
#include "config.h"


void Listener::setup() {
  base = event_base_new();
  http = evhttp_new(base);
  handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", Config::i()->port);
  evhttp_set_cb(http, "/list", Listener::list_request_cb, this);
  evhttp_set_cb(http, "/set", Listener::set_request_cb, this);
  evhttp_set_cb(http, "/", Listener::base_request_cb, this);
  event_base_dispatch(base);
}

void Listener::setEventHandler(Events* e) {
  event = e;
}

void Listener::list_request_cb(struct evhttp_request *req, void *arg) {
  Events* event = (Events*) arg;
  struct evbuffer *out_buf = evbuffer_new();
  struct evkeyvalq *output_header = evhttp_request_get_output_headers(req);
  evhttp_add_header(output_header, "Content-Type", "application/json");
  deviceVector devices;
  std::vector<device_st*>::iterator it;
  devices = event->getDevices();

  evbuffer_add_printf(out_buf, "{\"switches\": [\n");
  int first = 1;
  for (it = devices->begin(); it != devices->end(); ++it) {
    if (first == 1) { first = 0; } else { evbuffer_add_printf(out_buf, ","); }
    evbuffer_add_printf(out_buf, "{\"id\": \"%d\", \"state\": \"%d\", \"name\": \"%s\"}\n",(*it)->id, (*it)->state, (*it)->name);
    event->freeDevice((*it));
  }
  evbuffer_add_printf(out_buf, "]}");
  evhttp_send_reply(req, 200, "OK", out_buf);
}

void Listener::set_request_cb(struct evhttp_request *req, void *arg) {
  Events* event = (Events*) arg;
  struct evbuffer  *out_buf       = evbuffer_new();
  struct evkeyvalq *output_header = evhttp_request_get_output_headers(req);
  struct evkeyvalq *input_header;
  int device_id;
  int state;

  evhttp_add_header(output_header, "Content-Type", "application/json");

  //Find the device id
  evhttp_parse_query(evhttp_request_get_uri(req),input_header);
  device_id = atoi(evhttp_find_header(input_header, "device"));
  state     = atoi(evhttp_find_header(input_header, "state"));

  evbuffer_add_printf(out_buf, "Got device: %d State: %d\n", device_id, state);
  event->setState(device_id, state);

  //Send it off
  evhttp_send_reply(req, 200, "OK", out_buf);
  evhttp_clear_headers(input_header);
  evhttp_clear_headers(output_header);
  evbuffer_free(out_buf);
}

void Listener::base_request_cb(struct evhttp_request *req, void *arg) {
  struct evbuffer  *out_buf       = evbuffer_new();
  int fd = -1;
  struct stat st;
  fd = open("front.html", O_RDONLY);
  fstat(fd, &st);
  evbuffer_add_file(out_buf, fd, 0, st.st_size);
  evhttp_send_reply(req, 200, "OK", out_buf);
  evbuffer_free(out_buf);
}
