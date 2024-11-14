#include "request_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

HttpRequestQueue *create_queue() {
    HttpRequestQueue *queue = (HttpRequestQueue *)malloc(sizeof(HttpRequestQueue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void destroy_queue(HttpRequestQueue *queue) {
    while (queue->front != NULL) {
        HttpRequest *temp = queue->front;
        queue->front = queue->front->next;
        free(temp->url);
        free(temp->data);
        free(temp);
    }
    free(queue);
}

void enqueue(HttpRequestQueue *queue, const char *url, const char *data) {
    HttpRequest *new_request = (HttpRequest *)malloc(sizeof(HttpRequest));
    new_request->url = strdup(url);
    new_request->data = strdup(data);
    new_request->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = new_request;
    } else {
        queue->rear->next = new_request;
        queue->rear = new_request;
    }

    queue->size++;
}

int is_empty(HttpRequestQueue *queue) {
    if (queue->front == NULL) {
        return 1;
    }
    return 0;
}

HttpRequest *dequeue(HttpRequestQueue *queue) {
    if (queue->front == NULL) {
        return NULL;
    }

    HttpRequest *temp = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    queue->size--;

    return temp;
}

int send_request(const char *url, const char *data) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Erro ao inicializar o CURL\n");
        return 0;
    }

    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    // Tenta realizar a requisição
    res = curl_easy_perform(curl);

    int success = 1;
    if (res != CURLE_OK) {
        fprintf(stderr, "Erro na requisição CURL: %s\n", curl_easy_strerror(res));
        success = 0;
    }

    curl_easy_cleanup(curl);
    return success;
}

void process_queue(HttpRequestQueue *queue) {
    while (queue->front != NULL) {
        HttpRequest *request = dequeue(queue);

        // Tenta enviar a requisição
        if (send_request(request->url, request->data)) {
            printf("Requisição enviada com sucesso para %s\n", request->url);
        } else {
            // Se falhar, coloca de volta na fila
            printf("Falha ao enviar requisição para %s, re-adicionando à fila\n", request->url);
            enqueue(queue, request->url, request->data);
            free(request->url);
            free(request->data);
            free(request);
            break; // Sai do loop, assumindo que a conexão ainda está indisponível
        }

        free(request->url);
        free(request->data);
        free(request);
    }
}
