#include "./User.h"

#include <string.h>

void User::echo(string data) {
  for (int i = 0; i < data.size(); ++i) {
    fileSystem.write(1, data[i], strlen(data[i]));
  }
}