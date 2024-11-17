# README: HttpRequestQueue Library

## Overview

This library provides an implementation for managing and processing HTTP requests using a queue data structure. It includes functions to enqueue, dequeue, and process HTTP requests using the **libcurl** library to perform actual HTTP POST operations. This implementation is ideal for scenarios where multiple HTTP requests need to be queued and processed sequentially or with retry mechanisms in case of failures.

---

## Features

- **Queue Management**: Enqueue, dequeue, and check the status of the HTTP request queue.
- **HTTP Request Handling**: Send HTTP POST requests using the `libcurl` library.
- **Retry Mechanism**: Automatically re-add failed requests back into the queue for retry.
- **Memory Management**: Dynamic allocation and deallocation of memory for HTTP requests and the queue.

---

## File Structure

1. **request_queue.h**  
   The header file defining the `HttpRequest` and `HttpRequestQueue` structures.

2. **request_queue.c**  
   Implementation of the queue operations and HTTP request processing logic.

---

## Prerequisites

- **C Compiler** (e.g., GCC)
- **libcurl**: Ensure `libcurl` is installed on your system.
  - On Debian-based systems: `sudo apt-get install libcurl4-openssl-dev`
  - On macOS: `brew install curl`

---

## Structures

### `HttpRequest`
Represents a single HTTP request.

```c
typedef struct HttpRequest {
    char *url;       // Target URL
    char *data;      // Data to be sent via POST
    struct HttpRequest *next; // Pointer to the next request
} HttpRequest;
```

### `HttpRequestQueue`
Represents the queue structure.

```c
typedef struct HttpRequestQueue {
    HttpRequest *front; // Pointer to the front of the queue
    HttpRequest *rear;  // Pointer to the rear of the queue
    int size;           // Number of requests in the queue
} HttpRequestQueue;
```

---

## Functions

### Queue Management

- **`HttpRequestQueue *create_queue()`**  
  Allocates and initializes a new queue.

- **`void destroy_queue(HttpRequestQueue *queue)`**  
  Frees all memory associated with the queue.

- **`void enqueue(HttpRequestQueue *queue, const char *url, const char *data)`**  
  Adds a new HTTP request to the queue.

- **`HttpRequest *dequeue(HttpRequestQueue *queue)`**  
  Removes and returns the front request in the queue.

- **`int is_empty(HttpRequestQueue *queue)`**  
  Checks if the queue is empty.

### HTTP Request Handling

- **`int send_request(const char *url, const char *data)`**  
  Sends an HTTP POST request using `libcurl`. Returns `1` on success and `0` on failure.

- **`void process_queue(HttpRequestQueue *queue)`**  
  Processes all requests in the queue, sending each one sequentially. Requests that fail are re-added to the queue.

---

## Example Usage

```c
#include "request_queue.h"

int main() {
    HttpRequestQueue *queue = create_queue();

    // Add requests to the queue
    enqueue(queue, "https://example.com/api", "param1=value1&param2=value2");
    enqueue(queue, "https://another.com/endpoint", "data=123");

    // Process the queue
    process_queue(queue);

    // Clean up
    destroy_queue(queue);

    return 0;
}
```

---

## Notes

1. **Thread Safety**: This implementation is not thread-safe. Use synchronization mechanisms if accessed by multiple threads.
2. **Retry Mechanism**: If a request fails, it will be re-added to the queue. Modify `process_queue` to implement alternative failure handling.
3. **libcurl Setup**: Ensure your environment supports `libcurl` and it's properly linked during compilation.

---

## Compilation

To compile a program using this library, link it with `libcurl`:

```bash
gcc -o program main.c request_queue.c -lcurl
```

---

## License

This implementation is free to use and modify. No specific license is applied.
