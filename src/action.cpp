#include <curl/curl.h>
#include <string.h>

#include "config.h"
#include "action.h"
#include "events.h"

void Action::deviceOnEvent(int deviceId) {
  printf("ON event from device %i\n", deviceId);
  struct device_st* device;
  device = event->getDevice(deviceId);
  printf("On event from '%s'\n", device->name);
  if (0 == strncmp(device->name, "The Doorbell", 50)) {
    pushMessage("Doorbell", "It rang");
  }

  event->freeDevice(device);
}

void Action::setEventHandler(Events* e) {
  event = e;
}

void Action::deviceOffEvent(int device) {
  printf("OFF event from device %i\n", device);
}

void Action::sensorTempAction(const char* id, const char* value) {
  saveSensorData(id, value, "temp");
}

void Action::sensorHumiAction(const char* id, const char* value) {
  saveSensorData(id, value, "humi");
}

void Action::saveSensorData(const char* id, const char* value, const char* type) {
  char filename[80];
  FILE* file;

  printf("%s: %s %s\n", type, id, value);

  sprintf(filename, "%s/%s", type, id);
  //file = fopen(filename, "w");
  //fprintf(file, "%s", value);
  //fclose(file);

}


void Action::pushMessage(char* title, char* message) {
  CURL *curl;
  CURLM *multi_handle;

  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";

  curl_formadd(&formpost,
	       &lastptr,
	       CURLFORM_COPYNAME, "token",
	       CURLFORM_COPYCONTENTS, Config::i()->pushover_token,
	       CURLFORM_END);

  curl_formadd(&formpost,
	       &lastptr,
	       CURLFORM_COPYNAME, "user",
	       CURLFORM_COPYCONTENTS, Config::i()->pushover_user,
	       CURLFORM_END);

  curl_formadd(&formpost,
	       &lastptr,
	       CURLFORM_COPYNAME, "title",
	       CURLFORM_COPYCONTENTS, title,
	       CURLFORM_END);

  curl_formadd(&formpost,
	       &lastptr,
	       CURLFORM_COPYNAME, "message",
	       CURLFORM_COPYCONTENTS, message,
	       CURLFORM_END);

  curl = curl_easy_init();
  multi_handle = curl_multi_init();
  headerlist = curl_slist_append(headerlist, buf);
  curl_easy_setopt(curl, CURLOPT_URL, "https://api.pushover.net/1/messages.json");
  //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
  curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Action::curl_data_cb);
  curl_multi_add_handle(multi_handle, curl);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
}

size_t Action::curl_data_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
  return size;
}
