// SPDX-FileCopyrightText: 2025 The UniversalLoader development team
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fiobj_hash.h"
#include "fiobject.h"
#include "http.h"
#include <string.h>

#define LINUX_PAYLOAD "Linux"
#define MAC_PAYLOAD "Mac"
#define WINDOWS_PAYLOAD "Windows"
#define FREEBSD_PAYLOAD "FreeBSD"
#define OPENBSD_PAYLOAD "OpenBSD"
#define NETBSD_PAYLOAD "NetBSD"

void dispatcher(http_s *request);

int main() {
  http_listen("3000", NULL, .on_request = dispatcher, .log = 1);
  fio_start();
}

void send_payload(http_s *request);

void dispatcher(http_s *request) {
  char *method = fiobj_obj2cstr(request->method).data;
  char *path = fiobj_obj2cstr(request->path).data;

  if (strcmp(method, "GET") == 0 && strcmp(path, "/payload") == 0) {
    send_payload(request);

    return;
  }

  http_send_error(request, 404);
}

void send_payload(http_s *request) {
  http_parse_body(request);
  http_parse_query(request);

  FIOBJ operating_system = fiobj_hash_get2(
      request->params, fiobj_hash_string("operating_system", 16));

  if (operating_system == FIOBJ_INVALID) {
    http_send_error(request, 400);

    return;
  }

  char *operating_system_data = fiobj_obj2cstr(operating_system).data;

  if (strcmp(operating_system_data, "linux") == 0) {
    http_send_body(request, LINUX_PAYLOAD, strlen(LINUX_PAYLOAD));

    return;
  }

  if (strcmp(operating_system_data, "mac") == 0) {
    http_send_body(request, MAC_PAYLOAD, strlen(MAC_PAYLOAD));

    return;
  }

  if (strcmp(operating_system_data, "windows") == 0) {
    http_send_body(request, WINDOWS_PAYLOAD, strlen(WINDOWS_PAYLOAD));

    return;
  }

  if (strcmp(operating_system_data, "freebsd") == 0) {
    http_send_body(request, FREEBSD_PAYLOAD, strlen(FREEBSD_PAYLOAD));

    return;
  }

  if (strcmp(operating_system_data, "openbsd") == 0) {
    http_send_body(request, OPENBSD_PAYLOAD, strlen(OPENBSD_PAYLOAD));

    return;
  }

  if (strcmp(operating_system_data, "netbsd") == 0) {
    http_send_body(request, NETBSD_PAYLOAD, strlen(NETBSD_PAYLOAD));

    return;
  }

  http_send_error(request, 400);
}
