#include "request_queue.h"
#include <unistd.h>

int main() {
    HttpRequestQueue *queue = create_queue();

    enqueue(queue, "http://example.com/api/data1", "field1=value1&field2=value2");
    enqueue(queue, "http://example.com/api/data2", "field1=value3&field2=value4");

    while (1) {
        process_queue(queue);

        printf("Esperando para reenviar...\n");
        sleep(10);

        if(is_empty(queue))
        {
            break;
        }
    }

    destroy_queue(queue);
    return 0;
}
