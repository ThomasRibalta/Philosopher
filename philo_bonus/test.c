#include <stdio.h>
#include <pthread.h>

// Fonction qui sera appelée par le thread
void* print_hello(void* arg) {
    printf("Hello, World!\n");
    return NULL;
}

int main() {
    pthread_t thread_id;

    // Créer le thread
    if (pthread_create(&thread_id, NULL, print_hello, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Attendre la fin du thread
    if (pthread_join(thread_id, NULL) != 0) {
        perror("Failed to join thread");
        return 1;
    }

    return 0;
}

