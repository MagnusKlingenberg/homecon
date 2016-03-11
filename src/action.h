#ifndef ACTION_H
#define ACTION_H

#include <stdio.h>

class Events;
class Action {
public:
  void sensorTempAction(const char* id, const char* value);
  void sensorHumiAction(const char* id, const char* value);
  void deviceOnEvent(int device);
  void deviceOffEvent(int device);
  void setEventHandler(Events* e);

  static size_t curl_data_cb(char *ptr, size_t size, size_t nmemb, void *userdata);

private:
  void pushMessage(char* title, char* message);
  void saveSensorData(const char* id, const char* value, const char* type);

  Events* event;
};

#endif
