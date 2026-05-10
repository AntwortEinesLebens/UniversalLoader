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
  const char *method = fiobj_obj2cstr(request->method).data;
  const char *path = fiobj_obj2cstr(request->path).data;

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
