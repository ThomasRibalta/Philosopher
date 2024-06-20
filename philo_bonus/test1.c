#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Fonction qui sera appelée par le thread
void* print_hello(void* arg) {
    printf("Hello, World!\n");
    return NULL;
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Failed to fork");
        return 1;
    } else if (pid == 0) {
        // Code exécuté par le processus enfant
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

        // Terminer le processus enfant
        return 0;
    } else {
        // Code exécuté par le processus parent
        // Attendre la fin du processus enfant
        if (wait(NULL) < 0) {
            perror("Failed to wait for child");
            return 1;
        }
    }

    return 0;
}

