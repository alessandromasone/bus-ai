# Bus AI

Bus-AI è un'applicazione concorrente sviluppata in linguaggio C per gestire il servizio di trasporto autobus extraurbano nel rispetto delle restrizioni anti-Covid19 e delle regole di accesso sincronizzato. L'applicazione utilizza thread e semafori per implementare la programmazione concorrente.

## Descrizione del problema

L'autobus di linea per il trasporto extraurbano ha una capienza massima di MAX posti complessivi. Tuttavia, a causa delle restrizioni anti-Covid19, il trasporto non può superare MAX/2 persone alla volta. Ad ogni fermata, N persone chiedono di salire sull'autobus, mentre M persone scendono dall'automezzo. Prima che altre persone possano salire, un addetto sanifica la postazione appena liberata da chi scende.

## Soluzione proposta

Per gestire il servizio di trasporto autobus in modo sincronizzato, si è adottato uno schema di sincronizzazione basato su thread e semafori. Di seguito sono riportati i dettagli implementativi della soluzione.

1. Creazione del bus:
   - Il numero di posti e la percentuale di posti occupabili vengono forniti come input.
   - Il numero di piani viene calcolato distribuendo i posti in modo omogeneo, con un massimo di 69 posti per piano.
   - I posti effettivi vengono distribuiti su ogni piano.
   - Le dimensioni del piano base del bus vengono utilizzate come riferimento per tutti i piani.
   - Le varie zone del bus vengono tipizzate, inclusi i bordi della struttura, la posizione di ingresso e uscita, i robot, l'ascensore (se presente su più piani) e il posto dell'autista (solo al piano base).
   - I posti occupabili vengono assegnati in base alla percentuale specificata sull'intero autobus.

2. Creazione delle persone:
   - Ogni persona viene associata a un identificativo unico e a una fermata di salita e discesa.

3. Avvio del bus:
   - L'autista sale per primo sull'autobus.
   - I robot vengono avviati per pulire il bus.
   - Le persone salgono sull'autobus.
   - Il motore viene avviato e l'autobus si dirige verso la prossima fermata.
   - Le persone scendono dall'autobus.
   - I posti del bus vengono puliti.

4. Fasi delle persone:
   - Fase di entrata:
     - Viene verificata la disponibilità di ingresso sull'autobus.
     - Viene individuato il primo posto disponibile partendo dal fondo per ogni piano.
     - Se il posto si trova sullo stesso piano, la persona si dirige verso quel posto.
     - Altrimenti, la persona prende l'ascensore per raggiungere il piano in cui si trova il suo posto.
   - Fase di uscita:
     - Viene ripetuta la fase di entrata in modo inverso.
     - Il posto viene dichiarato sporco e la persona si dirige verso il piano base, dove è presente l'uscita.

## Implementazione

L'applicazione è implementata in linguaggio C utilizzando thread e semafori per la programmazione concorrente. Sono stati creati thread separati per il bus, le persone e gli addetti alla pulizia.

I thread delle persone gestiscono le fasi di entrata e uscita, accedendo sincronizzatamente alle risorse con l'aiuto dei semafori. Il numero massimo di persone che possono salire sull'autobus contemporaneamente è limitato dal semaforo.

Il thread del bus gestisce l'avvio del servizio, il movimento tra le fermate, lo scarico delle persone e la pulizia dei posti del bus. Utilizza i semafori per sincronizzare le operazioni e garantire che il numero di persone presenti sull'autobus rispetti i limiti imposti.

I thread degli addetti alla pulizia gestiscono la sanificazione dei posti appena liberati dalle persone che scendono. Utilizzano i semafori per accedere ai posti disponibili e garantire che il posto venga pulito prima che altre persone possano salire.

## Valori da configurare

Prima di eseguire l'applicazione, è necessario configurare i seguenti valori:

- Numero di fermate: deve essere un numero maggiore di 0.
- Percentuale posti: deve essere un numero maggiore di 1 e minore di 101.
- Posti totali: impostato come massimo a 138 (quindi due piani).
- Numero di persone: deve essere un numero maggiore di 0.
