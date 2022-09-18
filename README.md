ESERCIZIO

Un autobus di linea per il trasporto extraurbano dispone di una capienza di MAX posti
complessivi, e consente un trasporto non superiore a MAX/2 persone alla volta a causa
di restrizioni anti-Covid19 imposte. Ad ogni fermata N persone chiedono di salire,
mentre M persone scendono dall’automezzo. Ogni volta che le persone scendono, un
addetto provvede a sanificare la postazione appena liberata da chi scende e soltanto
successivamente viene consentito ad altre persone di salire, sempre nel rispetto dei
limiti massimi di trasporto consentiti. Si vuole realizzare un’applicazione concorrente
che gestisca nel rispetto delle regole sopra citate l’accesso sincronizzato al servizio di
trasporto autobus extraurbano.
Si descriva lo schema di sincronizzazione adoperato e si implementi una soluzione in
linguaggio C mediante il ricorso a thread e semafori per la programmazione concorrente.
Infine, scrivere una relazione che spieghi la strategia risolutiva e i dettagli
implementativi.

Numero di fermate maggiore di 0
Percentuale posti maggiore di 1 e minore di 101
Posti totali impostato come massimo a 138 (quindi due piani)
Numero di persone maggiore di 0