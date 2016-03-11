#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "events.h"
#include "action.h"
#include "listen.h"
#include "config.h"

int main(int argc, char *argv[]) {
  Config::i()->parse(argc, argv);
  Events *event = new Events();
  Listener *listener = new Listener();
  Action *action = new Action();

  event->setActionHandler(action);
  action->setEventHandler(event);
  listener->setEventHandler(event);

  event->setup();
  listener->setup();

  //tdClose();

  return 0;
}
