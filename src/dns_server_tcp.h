#ifndef _DNS_SERVER_TCP_H_
#define _DNS_SERVER_TCP_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <ev.h>

struct dns_server_tcp_s;

typedef void (*tcp_dns_req_received_cb)(
    struct dns_server_tcp_s *dns_server_tcp, void *data,
    struct sockaddr* addr, char *dns_req, size_t dns_req_len);

struct tcp_client_s {
  struct dns_server_tcp_s * d;

  int sock;

  struct sockaddr_storage raddr;
  socklen_t addr_len;

  char * input_buffer;
  uint32_t input_buffer_size;
  uint32_t input_buffer_used;

  ev_io read_watcher;
  ev_timer timer_watcher;

  struct tcp_client_s * next;
};

typedef struct dns_server_tcp_s {
  struct ev_loop *loop;
  void *cb_data;
  tcp_dns_req_received_cb cb;
  int sock;
  socklen_t addrlen;
  ev_io accept_watcher;
  uint16_t client_count;
  struct tcp_client_s * clients;
} dns_server_tcp_t;

void dns_server_tcp_init(dns_server_tcp_t *d,
    struct ev_loop *loop, struct addrinfo *listen_addrinfo,
    tcp_dns_req_received_cb cb, void *data);

void dns_server_tcp_respond(dns_server_tcp_t *d,
    struct sockaddr *raddr, char *resp, size_t resp_len);

void dns_server_tcp_stop(dns_server_tcp_t *d);

void dns_server_tcp_cleanup(dns_server_tcp_t *d);

#endif // _DNS_SERVER_H_
