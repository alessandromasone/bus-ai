//
//  casella.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef casella_h
#define casella_h

class casella {
    int _stato;
    int _tipo = -1;
    int _identificativo_utente = -1;
    
public:
    mutex_wrapper mutex;
    void sporco() { _stato = costante.stato.sporco; }
    void bordo() { _tipo = costante.tipo.bordo; }
    void occupato() { _stato = costante.stato.occupato; }
    void libero() { _stato = costante.stato.libero; }
    void sedile() { _tipo = costante.tipo.sedile; }
    void entrata() { _tipo = costante.tipo.entrata; }
    void uscita() { _tipo = costante.tipo.uscita; }
    void ascensore() { _tipo = costante.tipo.ascensore; }
    void robot() { _tipo = costante.tipo.robot; }
    void percorribile() { _tipo = costante.tipo.percorribile; }
    void disponibile(bool scelta) { _stato = (scelta) ? costante.stato.disponibile : costante.stato.nondisponibile; }
    void blocca(int identificativo_utente) { mutex.lock(); _identificativo_utente = identificativo_utente; _stato = costante.stato.occupato; }
    void sblocca() { mutex.unlock(); _identificativo_utente = -1; _stato = costante.stato.libero;}
    void utente(int identificativo_utente) { _identificativo_utente = identificativo_utente; }
    int utente() { return _identificativo_utente; }
    int tipo() { return _tipo; }
    int stato() { return _stato; }
};

#endif /* casella_h */
