// SPDX-FileCopyrightText: 2025 The UniversalLoader development team
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fiobj_hash.h"
#include "fiobject.h"
#include "http.h"
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int port_is_valid(const char *port);
static void dispatcher(http_s *request);

int main() {
  const char *environment_variables[] = {"PORT",
                                         "LINUX_PAYLOAD",
                                         "MAC_PAYLOAD",
                                         "WINDOWS_PAYLOAD",
                                         "FREEBSD_PAYLOAD",
                                         "OPENBSD_PAYLOAD",
                                         "NETBSD_PAYLOAD"};

  size_t count =
      sizeof(environment_variables) / sizeof(environment_variables[0]);

  for (size_t index = 0; index < count; index++) {
    const char *name = environment_variables[index];
    const char *value = getenv(name);

    if (!value || value[0] == '\0') {
      fprintf(stderr, "%s environment variable must be set and non-empty\n",
              name);

      return -1;
    }
  }

  const char *port = getenv("PORT");

  if (!port_is_valid(port)) {
    fprintf(stderr, "PORT must be a number between 1 and 65535\n");

    return -1;
  }

  intptr_t listener = (http_listen)(port, NULL,
                                    (struct http_settings_s){
                                        .on_request = dispatcher, .log = 1});

  if (listener == -1) {
    fprintf(stderr, "failed to listen on port %s\n", port);

    return -1;
  }

  fio_start();

  return 0;
}

static int port_is_valid(const char *port) {
  if (!port || !isdigit((unsigned char)port[0])) {
    return 0;
  }

  errno = 0;
  char *end = NULL;
  long value = strtol(port, &end, 10);

  return errno == 0 && *end == '\0' && value >= 1 && value <= 65535;
}

static void send_payload(http_s *request);

static void dispatcher(http_s *request) {
  const char *method = fiobj_obj2cstr(request->method).data;
  const char *path = fiobj_obj2cstr(request->path).data;

  if (strcmp(method, "GET") == 0 && strcmp(path, "/payload") == 0) {
    send_payload(request);

    return;
  }

  http_send_error(request, 404);
}

static void send_payload(http_s *request) {
  http_parse_body(request);
  http_parse_query(request);

  FIOBJ operating_system = fiobj_hash_get2(
      request->params, fiobj_hash_string("operating_system", 16));

  if (operating_system == FIOBJ_INVALID) {
    http_send_error(request, 400);

    return;
  }

  const char *operating_system_data = fiobj_obj2cstr(operating_system).data;

  if (strcmp(operating_system_data, "linux") == 0) {
    char *linux_payload = getenv("LINUX_PAYLOAD");
    http_send_body(request, linux_payload, strlen(linux_payload));

    return;
  }

  if (strcmp(operating_system_data, "mac") == 0) {
    char *mac_payload = getenv("MAC_PAYLOAD");
    http_send_body(request, mac_payload, strlen(mac_payload));

    return;
  }

  if (strcmp(operating_system_data, "windows") == 0) {
    char *windows_payload = getenv("WINDOWS_PAYLOAD");
    http_send_body(request, windows_payload, strlen(windows_payload));

    return;
  }

  if (strcmp(operating_system_data, "freebsd") == 0) {
    char *freebsd_payload = getenv("FREEBSD_PAYLOAD");
    http_send_body(request, freebsd_payload, strlen(freebsd_payload));

    return;
  }

  if (strcmp(operating_system_data, "openbsd") == 0) {
    char *openbsd_payload = getenv("OPENBSD_PAYLOAD");
    http_send_body(request, openbsd_payload, strlen(openbsd_payload));

    return;
  }

  if (strcmp(operating_system_data, "netbsd") == 0) {
    char *netbsd_payload = getenv("NETBSD_PAYLOAD");
    http_send_body(request, netbsd_payload, strlen(netbsd_payload));

    return;
  }

  http_send_error(request, 400);
}
