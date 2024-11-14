#ifndef REQUEST_QUEUE_H
#define REQUEST_QUEUE_H

#include <curl/curl.h>

// Estrutura para armazenar uma requisição HTTP
typedef struct HttpRequest {
    char *url;
    char *data;
    struct HttpRequest *next;
} HttpRequest;

// Estrutura para a fila de requisições
typedef struct {
    HttpRequest *front;
    HttpRequest *rear;
    int size;
} HttpRequestQueue;

// Funções da biblioteca
HttpRequestQueue *create_queue();
void destroy_queue(HttpRequestQueue *queue);
void enqueue(HttpRequestQueue *queue, const char *url, const char *data);
int is_empty(HttpRequestQueue *queue);
HttpRequest *dequeue(HttpRequestQueue *queue);
int send_request(const char *url, const char *data);
void process_queue(HttpRequestQueue *queue);

#endif // REQUEST_QUEUE_H
