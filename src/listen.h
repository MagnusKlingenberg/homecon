#include <event2/event.h>
#include <event2/http.h>

#ifndef LISTENER_H
#define LISTENER_H

class Events;
class Listener {
public: 
  void setup();

  static void list_request_cb(struct evhttp_request *req, void *arg);
  static void set_request_cb(struct evhttp_request *req, void *arg);
  static void base_request_cb(struct evhttp_request *req, void *arg);

  void setEventHandler(Events* e);

private:
  struct event_base* base;
  struct evhttp *http;
  struct evhttp_bound_socket *handle;

  Events* event;
};

#endif
