#include <stdio.h>
#include <stdlib.h>

int main() {
    char buffer[1024];  // Buffer per leggere l'input dalla pipe

    // Leggi l'input riga per riga dalla stdin (pipe)
    while (fgets(buffer, sizeof(buffer), stdin)) {
        // Puoi elaborare i dati letti qui
        printf("Ricevuto: %s", buffer);  // Stampa l'input ricevuto
    }

    return 0;
}

