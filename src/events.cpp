#include <stdio.h>
#include <telldus-core.h>
#include <time.h>
#include <string.h>

#include "events.h"
#include "action.h"

Events::Events() {
}

Events::~Events() {
  tdUnregisterCallback(callbackId);
}

void Events::setActionHandler(Action *a) {
  action = a;
}


void Events::setup() {
  tdInit();
  //callbackId = tdRegisterRawDeviceEvent (reinterpret_cast<TDRawDeviceEvent>(&Events::rawDeviceEventCallback), this );
  callbackId = tdRegisterSensorEvent (reinterpret_cast<TDSensorEvent>(&Events::sensorEventCallback), this );
  callbackId = tdRegisterDeviceChangeEvent (reinterpret_cast<TDDeviceChangeEvent>(&Events::deviceEventChangeCallback), this );
  callbackId = tdRegisterDeviceEvent (reinterpret_cast<TDDeviceEvent>(&Events::deviceEventCallback), this );
}

void Events::deviceEventChangeCallback(int deviceId, int eventId, int changeType, int callbackId, void *context) {
  printf("deviceId: %d, eventId: %d, changeType: %d, callbackId: %d\n", deviceId, eventId, changeType, callbackId);
}

void Events::sensorEventCallback(const char *protocol, const char *model, int sensorId, int dataType, const char *value, int ts, int callbackId, void *context) {
  char id[80];

  //Print the sensor
  sprintf(id, "%s-%s-%i", protocol, model, sensorId);
  Action *a = (Action*)context;

  //Retrieve the values the sensor supports
  if (dataType == TELLSTICK_TEMPERATURE) {
    a->sensorTempAction(id, value);
  } else if (dataType == TELLSTICK_HUMIDITY) {
    a->sensorHumiAction(id, value);
  }
}

void Events::rawDeviceEventCallback(const char *data, int controllerId, int callbackId, void *context) {
  printf("Incoming event %i %i %s\n", callbackId, controllerId, data);
}

void Events::deviceEvent(int deviceId, int method, const char *data) {
  if (method == TELLSTICK_TURNON) {
    action->deviceOnEvent(deviceId);
  } else if (method == TELLSTICK_TURNOFF) {
    action->deviceOffEvent(deviceId);
  } else {
    printf("Unknown event from device %i\n", deviceId);
  }
}

void Events::deviceEventCallback(int deviceId, int method, const char *data, int callbackId, void *context) {
  Events *e = reinterpret_cast<Events *>(context);
  if (e) {
    /** Please note!
     * We are here in another thread than the main. Some measures to syncronize
     * this must be taken!
     **/
    e->deviceEvent(deviceId, method, data);
  }
}

deviceVector Events::getDevices() {
  deviceVector devices = new std::vector<device_st*>(0);

  int intNumberOfDevices = tdGetNumberOfDevices();
  struct device_st* device;
  for (int i = 0; i < intNumberOfDevices; i++) {
    //struct device_st* device = (struct device_st*) malloc(sizeof(struct device_st));; 
    int deviceId = tdGetDeviceId(i);
    device = this->getDevice(deviceId);
    devices->push_back(device);
  }

  return devices;
}

void Events::setState(int deviceId, int state) {
  if (state == TELLSTICK_TURNOFF) {
    tdTurnOff(deviceId);
  } else if (state == TELLSTICK_TURNON) {
    tdTurnOn(deviceId);
  }
}

device_st* Events::getDevice(int deviceId) {
  struct device_st* device = new device_st(); 
  device->id = deviceId;
  device->name = tdGetName(deviceId);
  device->state = tdLastSentCommand(deviceId, TELLSTICK_TURNON | TELLSTICK_TURNOFF);
  return device;
}

void Events::freeDevice(device_st* device) {
  tdReleaseString(device->name);
  delete device;
}
