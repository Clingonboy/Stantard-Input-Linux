# Come fare un programma in C che legge lo standard input

In Linux, la possibilità di utilizzare pipe per comunicare tra programmi è una delle caratteristiche più potenti del sistema. Per creare un programma in C che riceve l'input tramite pipe, devi leggere dal flusso standard di input (stdin), che è proprio ciò che riceve i dati dalla pipe.

Quando esegui un comando come `ls | wc -l | mioprogramma`, l'output di `ls` viene passato a `wc -l`, e l'output di `wc -l` viene passato al tuo programma `mioprogramma`. Per gestire questa situazione nel tuo programma C, puoi leggere i dati dalla `stdin` (che è il flusso di input predefinito per i dati provenienti dalle pipe) usando funzioni come `fgets()` o `read()`.

### Esempio di programma in C che legge da stdin

Ecco un esempio base di un programma C che legge i dati dalla pipe e li elabora. In questo caso, il programma conta semplicemente il numero di righe ricevute, ma puoi fare qualsiasi altro tipo di elaborazione sui dati.

```c
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
```

### Come funziona:
1. **`fgets(buffer, sizeof(buffer), stdin)`**: Questa funzione legge una riga dalla `stdin` e la memorizza nel `buffer`. Quando esegui il comando `ls | wc -l | mioprogramma`, i dati provenienti dalla pipe vengono inviati a `stdin` del programma `mioprogramma`.
2. Il programma legge i dati dalla pipe riga per riga. Ogni volta che c'è una nuova riga, il programma la processa (nel nostro esempio, la stampa).
3. Puoi sostituire il codice all'interno del ciclo per fare qualsiasi tipo di elaborazione sui dati letti, come il conteggio delle righe, l'analisi del contenuto, o qualsiasi altro calcolo.

### Compilazione e utilizzo:
1. **Compilazione**: Per compilare il programma, utilizza `gcc`:
   ```bash
   gcc -o mioprogramma mioprogramma.c
   ```
2. **Esecuzione**: Per eseguire il programma con la pipe, usa il seguente comando:
   ```bash
   ls | wc -l | ./mioprogramma
   ```

### Considerazioni aggiuntive:
- Il programma non ha un termine esplicito per sapere quando finisce l'input; continua a leggere finché c'è qualcosa da leggere (tipicamente quando la pipe è chiusa).
- Se hai bisogno di gestire grandi quantità di dati, puoi leggere il flusso in modo più efficiente utilizzando `read()` al posto di `fgets()`, ma il principio di base rimane lo stesso: leggere da `stdin`.


## Stesso programma in GO

Hai ragione nel notare che in Go, con l'uso di un ciclo `for` insieme a `scanner.Scan()`, l'elaborazione avviene mentre il programma legge l'input riga per riga. Tuttavia, la tua osservazione riguarda un comportamento che potrebbe sembrare come se Go "rimanesse in attesa" finché non ha ricevuto tutto l'input dalla pipe prima di iniziare l'elaborazione.

Quello che accade è che Go, mentre esegue il ciclo `for`, **legge l'input riga per riga** dal flusso della pipe. La funzione `scanner.Scan()` non blocca il programma indefinitamente; invece, quando `scanner.Scan()` è chiamato, **legge i dati in arrivo** finché non trova una nuova riga e inizia a processarla immediatamente. Quindi, non aspetta che l'intera pipe venga chiusa prima di elaborare i dati, ma elabora riga per riga, man mano che viene letta.

Tuttavia, potrebbe sembrare che Go stia "aspettando" la fine della pipe prima di processare i dati, soprattutto se non vedi una reazione immediata. Questo potrebbe essere dovuto a come i dati vengono passati attraverso la pipe o se l'input è più lento di quanto ci si aspetti.

### Esempio aggiornato con elaborazione immediata:

Per chiarire, ecco un esempio che mostra come Go elabora i dati mano a mano che vengono ricevuti dalla pipe, senza aspettare la chiusura della pipe:

```go
package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)

	// Leggi ogni riga dalla pipe, una riga alla volta
	for scanner.Scan() {
		line := scanner.Text()

		// Elabora la riga non appena viene letta
		fmt.Printf("Elaborata: %s\n", line)
	}

	// Verifica eventuali errori durante la lettura
	if err := scanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "Errore durante la lettura:", err)
	}
}
```

### Come funziona:
1. **Elaborazione in tempo reale**: Ogni riga che arriva nella pipe viene letta e elaborata immediatamente (nel nostro caso, viene stampata), senza che il programma aspetti che tutta la pipe sia stata chiusa.
2. **Lettura non bloccante**: La chiamata a `scanner.Scan()` legge il flusso e poi passa alla riga successiva non appena è disponibile. Se i dati sono già nella pipe, vengono letti e processati in tempo reale.

### Perché potrebbe sembrare che Go aspetti:
- **Buffering**: Se i dati in ingresso vengono inviati troppo lentamente (ad esempio se c'è un ritardo nel comando precedente nella pipeline), Go potrebbe sembrare che stia aspettando. Questo può accadere in pipeline complesse dove i dati vengono processati lentamente.
- **Lettura parziale**: Se l'applicazione che scrive nella pipe non invia i dati tutti insieme (ad esempio, se scrive dati in blocchi o se c'è una latenza nel programma precedente), l'output di Go potrebbe non apparire immediatamente.

### Verifica l'output immediato:
Per vedere meglio l'elaborazione immediata, prova un comando semplice come:

```bash
echo -e "line1\nline2\nline3" | ./mioprogramma
```

Dovresti vedere le righe stampate immediatamente man mano che vengono lette, senza che Go aspetti che l'intero flusso sia chiuso.

### Conclusioni:
In Go, la lettura da una pipe e l'elaborazione avviene effettivamente man mano che i dati sono disponibili, ma può sembrare che stia aspettando a causa di buffer o latenza nei dati provenienti dalla pipe. Se la tua pipeline è lenta, Go potrebbe sembrare "in attesa", ma in realtà sta leggendo ed elaborando le righe in tempo reale.


## Versione in perl

In Perl, leggere l'input da una pipe è altrettanto semplice, grazie alla gestione dei flussi di input che Perl offre nativamente. Puoi leggere da `STDIN`, che rappresenta l'input standard, come nel caso di C e Python.

### Esempio di programma Perl che legge da `STDIN` (pipe)

Ecco un esempio di programma Perl che legge riga per riga dall'input standard (cioè dalla pipe) e stampa ogni riga ricevuta:

```perl
#!/usr/bin/perl
use strict;
use warnings;

# Leggi ogni riga da STDIN (pipe)
while (my $line = <STDIN>) {
    # Puoi fare qualsiasi elaborazione su ogni riga ricevuta
    print "Ricevuto: $line";
}
```

### Come funziona:
1. **`<STDIN>`**: In Perl, `<STDIN>` legge una riga di input da `STDIN`. Questo è equivalente a leggere da una pipe. Quando esegui un comando come `ls | wc -l | perl mioprogramma.pl`, l'output di `ls` va prima a `wc -l`, e poi l'output di `wc -l` viene passato al programma Perl tramite `STDIN`.
2. **`while (my $line = <STDIN>)`**: Questo ciclo legge ogni riga inviata tramite la pipe. La variabile `$line` contiene la riga letta.
3. **`print "Ricevuto: $line"`**: Questo stampa ogni riga ricevuta. Nota che `$line` contiene già il carattere di fine riga (`\n`), che può essere rimosso con `chomp($line)` se necessario.

### Esecuzione:
1. **Permessi di esecuzione**: Se il file Perl si chiama `mioprogramma.pl`, assicurati che sia eseguibile:
   ```bash
   chmod +x mioprogramma.pl
   ```
2. **Esecuzione**: Esegui il programma con una pipe come segue:
   ```bash
   ls | wc -l | perl mioprogramma.pl
   ```

### Considerazioni aggiuntive:
- Se vuoi rimuovere la nuova linea (`\n`) dalla fine di ogni riga, puoi usare `chomp($line)`:
  ```perl
  chomp($line);
  print "Ricevuto: $line\n";
  ```
- Come negli altri linguaggi, Perl continuerà a leggere da `STDIN` finché non ci sono più dati in arrivo.

### Altre elaborazioni
Puoi anche fare altre elaborazioni sui dati letti, ad esempio contando il numero di righe, come in questo esempio:

```perl
#!/usr/bin/perl
use strict;
use warnings;

my $count = 0;

# Leggi ogni riga da STDIN (pipe)
while (my $line = <STDIN>) {
    $count++;
}

print "Numero di righe ricevute: $count\n";
```

Questo programma legge tutte le righe dalla pipe e poi stampa il numero totale di righe ricevute.

---

Perl, come Python, è molto adatto per lavorare con flussi di input e output e rende facile gestire le pipe.
## Versione in python

In Python, leggere i dati da una pipe (o da qualsiasi flusso di input) è molto semplice grazie alla gestione automatica degli stream di input. Puoi utilizzare `sys.stdin` per leggere i dati in arrivo tramite pipe, che è simile a come funzionava in C con `stdin`.

### Esempio di programma Python che legge da `stdin` (pipe)

Ecco un esempio di programma in Python che legge dall'input standard (cioè dalla pipe) e stampa ogni riga ricevuta:

```python
import sys

# Leggi ogni riga dall'input (pipe)
for line in sys.stdin:
    # Puoi fare qualsiasi elaborazione su ogni riga ricevuta
    print(f"Ricevuto: {line.strip()}")
```

### Come funziona:
1. **`sys.stdin`**: In Python, `sys.stdin` è un oggetto file-like che rappresenta l'input standard. Quando esegui un comando come `ls | wc -l | python3 mioprogramma.py`, l'output di `ls` va prima a `wc -l`, e poi l'output di `wc -l` viene passato a `sys.stdin` del programma Python.
2. **`for line in sys.stdin`**: Questo ciclo legge ogni riga inviata attraverso la pipe. Ogni riga letta viene elaborata all'interno del ciclo.
3. **`line.strip()`**: Questo rimuove i caratteri di nuova riga (`\n`) dalla fine della stringa. Se non vuoi rimuovere la nuova linea, puoi evitare di chiamare `strip()`.

### Compilazione ed Esecuzione:
Non è necessaria la compilazione per un programma Python, puoi semplicemente eseguire il programma direttamente. Assicurati di avere il file Python salvato (per esempio, come `mioprogramma.py`).

1. **Esecuzione**: Usa il comando seguente per eseguire il programma con la pipe:
   ```bash
   ls | wc -l | python3 mioprogramma.py
   ```

### Considerazioni aggiuntive:
- Come in C, il programma Python continua a leggere dal flusso fino a quando non è finito, quindi non è necessario un meccanismo speciale per sapere quando i dati sono finiti.
- Python gestisce automaticamente l'encoding, quindi puoi essere certo che i dati in arrivo da `stdin` saranno decodificati correttamente in formato stringa (UTF-8 di default).

### Altre elaborazioni
Se vuoi fare qualcosa di più complesso con le righe, ad esempio contare quante righe sono state lette, puoi farlo facilmente con un contatore:

```python
import sys

count = 0
for line in sys.stdin:
    count += 1

print(f"Numero di righe ricevute: {count}")
```

In questo caso, il programma legge le righe dalla pipe e conteggia quante righe sono arrivate, per poi stampare il totale.



