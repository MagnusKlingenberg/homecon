#ifndef CONFIG_H
#define CONFIG_H

class Config {
public:
  static Config* i();
  void parse(int argc, char* argv[]);

  int port;
  char* pushover_user;
  char* pushover_token;

private:
  static Config* instance;
};

#endif
