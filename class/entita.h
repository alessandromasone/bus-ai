//
//  entita.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef entita_h
#define entita_h

class entita{
private:
    static inline int _entita_generate = 0;
    int _ruolo;
    int _identificativo;
    int _salita;
    int _discesa;
    int _stato;
    int _piano = -1;
    int _velocita;
public:
    thread cervello;
    coordinata posizione;
    coordinata posto;
    void genera(int ruolo, int identificativo, int salita, int discesa);
    int persone() { return _entita_generate; }
    int ruolo() { return _ruolo; }
    void ruolo(int ruolo) { _ruolo = ruolo; }
    int identificativo() { return _identificativo; }
    void identificativo(int identificativo) { _identificativo = identificativo; }
    int salita() { return _salita; }
    void salita(int salita) { _salita = salita; }
    int discesa() { return _discesa; }
    void discesa(int discesa) { _discesa = discesa; }
    int stato() { return _stato; }
    void stato(int stato) { _stato = stato; }
    void seduto() { _stato = costante.stato.seduto; }
    void alzato() { _stato = costante.stato.alzato; }
    void entrata(int &fermata, vector<livello> &piano, coordinata &ascensore, coordinata &entrata, coordinata &uscita, semaphore &semaforo, coordinata &austista) { cervello = thread(&entita::entratathread, this, ref(fermata), ref(piano), ref(ascensore), ref(entrata), ref(uscita), ref(semaforo), ref(austista)); }
    void entratathread(int &fermata, vector<livello> &piano, coordinata &ascensore, coordinata &entrata, coordinata &uscita, semaphore &semaforo, coordinata &austista);
    void seduta() { cervello.join(); }
    void alzata(int &fermata, vector<livello> &piano, coordinata &ascensore, coordinata &entrata, coordinata &uscita, semaphore &semaforo) { cervello = thread(&entita::alzatathread, this, ref(fermata), ref(piano), ref(ascensore), ref(entrata), ref(uscita), ref(semaforo)); }
    void alzatathread(int &fermata, vector<livello> &piano, coordinata &ascensore, coordinata &entrata, coordinata &uscita, semaphore &semaforo);
    void uscita() { cervello.join(); }
};

void entita::genera(int ruolo, int identificativo, int salita, int discesa) {
    _entita_generate++;
    _ruolo = ruolo;
    _identificativo = identificativo;
    _salita = salita;
    _discesa = discesa;
    _velocita = funzione.random(costante.velocita.persona.minimo, costante.velocita.persona.massimo);
    _piano = -1;
}
void entita::entratathread(int &fermata, vector<livello> &piano, coordinata &ascensore, coordinata &entrata, coordinata &uscita, semaphore &semaforo, coordinata &austista) {
    //prova ad entrare
    if (semaforo.try_acquire()) {
        alzato();
        funzione.debug(costante.debug, to_string(_identificativo) + ": presa della disonibilita' di posto all'interno del bus");
        piano.at(0).casella.at(entrata.dammi('x')).at(entrata.dammi('y')).blocca(_identificativo);
        funzione.debug(costante.debug, to_string(identificativo()) + ": entrata");
        posizione.assegna(entrata.dammi('x'), entrata.dammi('y'));
        funzione.debug(costante.debug, to_string(identificativo()) + ": posizione assegnato " + to_string(posizione.dammi('x')) + "-" + to_string(posizione.dammi('y')));;
        //per l'autista e non
        if (_identificativo == 0) {
            if (piano.at(0).casella.at(austista.dammi('x')).at(austista.dammi('y')).tipo() == costante.tipo.sedile && piano.at(0).casella.at(austista.dammi('x')).at(austista.dammi('y')).stato() == costante.stato.libero) {
                if (piano.at(0).casella.at(austista.dammi('x')).at(austista.dammi('y')).mutex.try_lock()) {
                    piano.at(0).casella.at(austista.dammi('x')).at(austista.dammi('y')).occupato();
                    piano.at(0).casella.at(austista.dammi('x')).at(austista.dammi('y')).utente(_identificativo);
                    posto.assegna(austista.dammi('x'), austista.dammi('y'));
                    _piano = 0;
                    funzione.debug(costante.debug, to_string(identificativo()) + ": posto preso " + to_string(posto.dammi('x')) + "-" + to_string(posto.dammi('y')));
                }
            }
        }
        else {
            for (int n_piano = 0; _piano == -1; n_piano++) {
                funzione.debug(costante.debug, to_string(identificativo()) + ": controllo piano " + to_string(n_piano));
                for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
                    for (int riga = 0; riga < piano.at(n_piano).casella.at(colonna).size(); riga++) {
                        if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.sedile && piano.at(n_piano).casella.at(colonna).at(riga).stato() == costante.stato.libero) {
                            if (piano.at(n_piano).casella.at(colonna).at(riga).mutex.try_lock()) {
                                piano.at(n_piano).casella.at(colonna).at(riga).occupato();
                                piano.at(n_piano).casella.at(colonna).at(riga).utente(_identificativo);
                                posto.assegna(colonna, riga);
                                _piano = n_piano;
                                funzione.debug(costante.debug, to_string(identificativo()) + ": posto assegnato " + to_string(posto.dammi('x')) + "-" + to_string(posto.dammi('y')));
                                break;
                            }
                        }
                        if (_piano != -1) {
                            break;
                        }
                    }
                }
            }
        }
        funzione.debug(costante.debug, to_string(identificativo()) + ": cammino verso il posto");
        //per arrivare al piano del proprio posto
        for (int piano_attuale = 0; piano_attuale < _piano; ) {
            usleep(_velocita);
            funzione.debug(costante.debug, to_string(identificativo()) + ": piano attuale" + to_string(_piano));
            if (posizione.dammi('x') != ascensore.dammi('x') || posizione.dammi('y') != ascensore.dammi('y')) {
                if (posizione.dammi('y') < ascensore.dammi('y')) {
                    piano.at(piano_attuale).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).blocca(_identificativo);
                    posizione.incrementay();
                    piano.at(piano_attuale).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).sblocca();
                }
                else if (posizione.dammi('y') > ascensore.dammi('y')) {
                    piano.at(piano_attuale).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).blocca(_identificativo);
                    posizione.decrementay();
                    piano.at(piano_attuale).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).sblocca();
                }
                else if (posizione.dammi('x') < ascensore.dammi('x')) {
                    piano.at(piano_attuale).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).blocca(_identificativo);
                    posizione.incrementax();
                    piano.at(piano_attuale).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).sblocca();
                }
                else if (posizione.dammi('x') > ascensore.dammi('x')) {
                    piano.at(piano_attuale).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).blocca(_identificativo);
                    posizione.decrementax();
                    piano.at(piano_attuale).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).sblocca();
                }
            }
            else {
                piano_attuale++;
                piano.at(piano_attuale).casella.at(posizione.dammi('x')).at(posizione.dammi('y')).blocca(_identificativo);
                piano.at(piano_attuale - 1).casella.at(posizione.dammi('x')).at(posizione.dammi('y')).sblocca();
            }
        }
        funzione.debug(costante.debug, to_string(identificativo()) + ": piano del posto raggiunto");
        //arrivo al posto
        for (bool posto_preso = true; posto_preso; ) {
            funzione.debug(costante.debug, to_string(identificativo()) + ": posizione " + to_string(posizione.dammi('x')) + "-" + to_string(posizione.dammi('y')));;
            usleep(_velocita);
            if (posizione.dammi('x') == posto.dammi('x') + 1) {
                if (posizione.dammi('y') == posto.dammi('y')) {
                    if (piano.at(_piano).casella.at(posto.dammi('x')).at(posto.dammi('y')).tipo() == costante.tipo.sedile && piano.at(_piano).casella.at(posto.dammi('x')).at(posto.dammi('y')).utente() == _identificativo) {
                        posizione.decrementax();
                        piano.at(_piano).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).sblocca();
                        posto_preso = false;
                    }
                }
                else if (posizione.dammi('y') < posto.dammi('y')) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).blocca(_identificativo);
                    posizione.incrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).sblocca();
                }
                else if (posizione.dammi('y') > posto.dammi('y')) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).blocca(_identificativo);
                    posizione.decrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).sblocca();
                }
            }
            else {
                if (posizione.dammi('y') == 3) {
                    if (posizione.dammi('x') < posto.dammi('x') + 1) {
                        piano.at(_piano).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).blocca(_identificativo);
                        posizione.incrementax();
                        piano.at(_piano).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).sblocca();
                    }
                    else if (posizione.dammi('x') > posto.dammi('x') + 1) {
                        piano.at(_piano).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).blocca(_identificativo);
                        posizione.decrementax();
                        piano.at(_piano).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).sblocca();
                    }
                }
                else if (posizione.dammi('y') < 3) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).blocca(_identificativo);
                    posizione.incrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).sblocca();
                }
                else if (posizione.dammi('y') > 3) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).blocca(_identificativo);
                    posizione.decrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).sblocca();
                }
            }
        }
        funzione.debug(costante.debug, to_string(identificativo()) + ": arrivato al posto");
        seduto();
    }
    else {
        funzione.debug(costante.debug, to_string(identificativo()) + ": bus perso");
    }
}
void entita::alzatathread(int &fermata, vector<livello> &piano, coordinata &ascensore, coordinata &entrata, coordinata &uscita, semaphore &semaforo) {
    piano.at(_piano).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).blocca(_identificativo);
    posizione.incrementax();
    piano.at(_piano).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).sporco();
    piano.at(_piano).sporco(true);
    piano.at(_piano).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).mutex.unlock();
    alzato();
    funzione.debug(costante.debug, to_string(identificativo()) + ": rilascio del posto");
    funzione.debug(costante.debug, to_string(identificativo()) + ": posizione attuale " + to_string(posizione.dammi('x')) + "-" + to_string(posizione.dammi('y')));;
    //arrivare al piano dove è possibile uscire
    for (; _piano != 0; ) {
        funzione.debug(costante.debug, to_string(identificativo()) + ": posizione attuale " + to_string(posizione.dammi('x')) + "-" + to_string(posizione.dammi('y')));
        usleep(_velocita);
        funzione.debug(costante.debug, to_string(identificativo()) + ": piano attuale" + to_string(_piano));
        if (posizione.dammi('x') == ascensore.dammi('x') && posizione.dammi('y') == ascensore.dammi('y')) {
            _piano--;
            piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y')).blocca(_identificativo);
            piano.at(_piano + 1).casella.at(posizione.dammi('x')).at(posizione.dammi('y')).sblocca();
        }
        else {
            if (posizione.dammi('x') == ascensore.dammi('x')) {
                if (posizione.dammi('y') < ascensore.dammi('y')) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).blocca(_identificativo);
                    posizione.incrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).sblocca();
                }
                else if (posizione.dammi('y') > ascensore.dammi('y')) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).blocca(_identificativo);
                    posizione.decrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).sblocca();
                }
            }
            else {
                if (posizione.dammi('y') == 3) {
                    if (posizione.dammi('x') < ascensore.dammi('x')) {
                        piano.at(_piano).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).blocca(_identificativo);
                        posizione.incrementax();
                        piano.at(_piano).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).sblocca();
                    }
                    else if (posizione.dammi('x') > ascensore.dammi('x')) {
                        piano.at(_piano).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).blocca(_identificativo);
                        posizione.decrementax();
                        piano.at(_piano).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).sblocca();
                    }
                }
                else {
                    if (posizione.dammi('y') < 3) {
                        piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).blocca(_identificativo);
                        posizione.incrementay();
                        piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).sblocca();
                    }
                    else if (posizione.dammi('y') > 3) {
                        piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).blocca(_identificativo);
                        posizione.decrementay();
                        piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).sblocca();
                    }
                }
            }
        }
    }
    funzione.debug(costante.debug, to_string(identificativo()) + ": arrivato al piano di uscita");
    //cammino verso l'uscita
    for (bool uscita_presa = true; uscita_presa; ) {
        funzione.debug(costante.debug, to_string(identificativo()) + ": posizione attuale " + to_string(posizione.dammi('x')) + "-" + to_string(posizione.dammi('y')));
        usleep(_velocita);
        if (posizione.dammi('x') == uscita.dammi('x')) {
            if (posizione.dammi('y') == uscita.dammi('y')) {
                piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y')).sblocca();
                uscita_presa = false;
            }
            else {
                if (posizione.dammi('y') < uscita.dammi('y')) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).blocca(_identificativo);
                    posizione.incrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).sblocca();
                }
                else if (posizione.dammi('y') > uscita.dammi('y')) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).blocca(_identificativo);
                    posizione.decrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).sblocca();
                }
            }
        }
        else {
            if (posizione.dammi('y') == 3) {
                if (posizione.dammi('x') < uscita.dammi('x')) {
                    piano.at(_piano).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).blocca(_identificativo);
                    posizione.incrementax();
                    piano.at(_piano).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).sblocca();
                }
                else if (posizione.dammi('x') > uscita.dammi('x')) {
                    piano.at(_piano).casella.at(posizione.dammi('x') - 1).at(posizione.dammi('y')).blocca(_identificativo);
                    posizione.decrementax();
                    piano.at(_piano).casella.at(posizione.dammi('x') + 1).at(posizione.dammi('y')).sblocca();
                }
            }
            else {
                if (posizione.dammi('y') < 3) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).blocca(_identificativo);
                    posizione.incrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).sblocca();
                }
                else if (posizione.dammi('y') > 3) {
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') - 1).blocca(_identificativo);
                    posizione.decrementay();
                    piano.at(_piano).casella.at(posizione.dammi('x')).at(posizione.dammi('y') + 1).sblocca();
                }
            }
        }
    }
    funzione.debug(costante.debug, to_string(identificativo()) + ": uscita presa e rilascio della disponibilita' di posto");
    semaforo.release();
}

#endif /* entita_h */
