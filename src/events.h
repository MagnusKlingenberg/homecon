#ifndef EVENTS_H
#define EVENTS_H
#include <vector>
#include <stdlib.h>

struct device_st {
  char* name;
  int id;
  int state;
};

typedef std::vector<device_st*>* deviceVector;

class Action;
class Events {
public:
  Events();

  ~Events();
  void setup();
  void deviceEvent(int deviceId, int method, const char *data);
  void setActionHandler(Action *a);

  static void deviceEventCallback(int deviceId, int method, const char *data, int callbackId, void *context);
  static void deviceEventChangeCallback(int deviceId, int eventId, int changeType, int callbackId, void *context);
  static void rawDeviceEventCallback(const char *data, int controllerId, int callbackId, void *context);
  static void sensorEventCallback(const char *protocol, const char *model, int sensorId, int dataType, const char *value, int ts, int callbackId, void *context);

  deviceVector getDevices();
  void setState(int deviceId, int state);
  device_st* getDevice(int deviceId);
  void freeDevice(device_st *device);

private:
  int callbackId;
  Action *action;
};

#endif
