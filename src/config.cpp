#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"



Config* Config::instance = NULL;

Config* Config::i()
{
  if (NULL == instance) {
    instance = new Config();
  }

  return instance;
}

void Config::parse(int argc, char* argv[])
{
  int c;
  while ((c = getopt (argc, argv, "p:u:t:")) != -1) {
    switch (c) {
    case 'p':
      port = atoi(optarg);
      break;
    case 'u':
      pushover_user = optarg;
      break;
    case 't':
      pushover_token = optarg;
      break;
    }
  }

  if (NULL == pushover_user ||
      NULL == pushover_token ||
      0 == port) {
    printf("Usage: -u <pushover username> -t <pushover token> -p <api port>\n");
    exit(1);
  }
}
