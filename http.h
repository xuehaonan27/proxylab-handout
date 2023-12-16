#ifndef HTTP_H
#define HTTP_H
#include <stdlib.h>
/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *connection_hdr = "close\r\n";
static const char *proxy_connection_hdr = "close\r\n";
enum Method { GET, POST, PUT, HEAD, CONNECT, UNINIT, };
enum Version { V0_9, V1_0, V1_1, V2_0, VOther, };
typedef struct {
    char *key;
    char *value;
    struct header_entry* next;
} header_entry;
/* 注意释放其中的指针! */
typedef struct {
    enum Method method;
    char *host;
    char *port;
    char *path;
    enum Version version;
    struct header_entry* headers;
    char *body;
}  HTTPRequest;
typedef struct {
    enum Version version;
    int status_code;
    char *status_text;
    struct header_entry* headers;
    char *body;
} HTTPResponse;
void display_request(HTTPRequest *request);
void request_init(HTTPRequest *request);
void request_clear(HTTPRequest *request);
void display_response(HTTPResponse *response);
void response_init(HTTPResponse *response);
void response_clear(HTTPResponse *response);
int read_request(int fd, HTTPRequest*);

/* 从request构建真正的HTTP请求, 写入real_request字符串 */
/* 返回值为real_request的长度, 如果返回-1则构建失败 */
/* 可以进一步解耦 */
int construct_real_request(HTTPRequest *request, char *real_request);
// int construct_cached_response(const HTTPResponse *response, char *response_buf);
int send_request(int facing_server_fd, const char *real_request, size_t len);
void *read_response(int facing_server_fd, char *buf, size_t *read_len);
int send_response(int connfd, const char *response, size_t len);
// int parse_response(HTTPResponse *response, char *response_line, size_t response_len, size_t *result);
size_t get_content_length(const char *response_line);
size_t get_url(const HTTPRequest *request, char *buf, size_t len);
#endif