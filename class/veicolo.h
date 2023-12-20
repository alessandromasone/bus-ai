//
//  veicolo.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef veicolo_h
#define veicolo_h

class veicolo {
private:
    int _identificativo;
    int _velocita_minima;
    int _velocita_massima;
    int _posti;
    int _percentuale_posti;
    int _fermate_totali;
    int _fermata_attuale = 0;
    bool _stato = false;
    bool _stampa = true;
public:
    thread stampa;
    semaphore semaforo;
    thread cervello;
    vector<entita> persona;
    coordinata entrata;
    coordinata uscita;
    coordinata ascensore;
    coordinata autista;
    vector<livello> piano;
    veicolo(int identificativo, int velocita_minima, int velocita_massima, int posti_totali, int percentuale_posti, int fermate_totali);
    ~veicolo();
    void startmotore() { _stato = true; }
    void stopmotore() { _stato = false; }
    void avviobus() { cervello = thread(&veicolo::avviobusthread, this); }
    void avviobusthread();
    void stopbus() { cervello.join(); }
    void piani();
    void posti();
    void dimensioni();
    void tipizzazione();
    void sedute();
    void struttura();
    void persone(int numero_persone);
    void avviostampa() { stampa = thread(&veicolo::stampathread, this); }
    void stopstampa() { _stampa = false; stampa.join(); }
    void stampathread();
    
};

void veicolo::stampathread() {
    while (_stampa && !costante.debug) {
        usleep(costante.velocita.stampa);
        system("clear");
        if (!_stato) {
            cout << "Fermata attuale " << _fermata_attuale << "/" << _fermate_totali << endl;
        }
        else {
            cout << "Verso la prossimo fermata" << endl;
        }
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            funzione.debug(costante.debug, "Tipo piano " + to_string(n_piano));
            for (int riga = 0; riga < piano.at(n_piano).casella.at(riga).size(); riga++) {
                for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
                    if (colonna == piano.at(n_piano).dyson.posizione.dammi('x') && riga == piano.at(n_piano).dyson.posizione.dammi('y')) {
                        cout << "R";
                    }
                    else if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.percorribile) {
                        if (piano.at(n_piano).casella.at(colonna).at(riga).utente() == -1) {
                            cout << " ";
                        }
                        else {
                            cout << "P";
                        }
                    }
                    else if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.bordo) {
                        cout <<"0";
                    }
                    else if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.uscita) {
                        cout << "U";
                    }
                    else if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.robot) {
                        cout << " ";
                    }
                    else if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.entrata) {
                        cout << "E";
                    }
                    else if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.sedile) {
                        if (piano.at(n_piano).casella.at(colonna).at(riga).stato() == costante.stato.occupato) {
                            if (colonna == autista.dammi('x') && riga == autista.dammi('y')) {
                                cout << "W";
                            }
                            else {
                                cout << "O";
                            }
                        }
                        else if (piano.at(n_piano).casella.at(colonna).at(riga).stato() == costante.stato.sporco) {
                            cout << "S";
                        }
                        else if (piano.at(n_piano).casella.at(colonna).at(riga).stato() == costante.stato.nondisponibile) {
                            cout << "X";
                        }
                        else if (piano.at(n_piano).casella.at(colonna).at(riga).stato() == costante.stato.libero) {
                            cout << "D";
                        }
                    }
                    else if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.ascensore) {
                        if (piano.at(n_piano).casella.at(colonna).at(riga).utente() == -1) {
                            cout << "A";
                        }
                        else {
                            cout << "P";
                        }
                    }
                }
                cout << endl;
            }
            cout << endl;
        }
        cout << "W -> Posto dell' autista" << endl;
        cout << "R -> Robot" << endl;
        cout << "P -> Persona" << endl;
        cout << "0 -> Bordo del bus" << endl;
        cout << "U -> Uscita bus" << endl;
        cout << "E -> Entrata bus" << endl;
        cout << "O -> Posto occupato" << endl;
        cout << "S -> Posto sporco" << endl;
        cout << "X -> Posto non disponibile" <<  endl;
        cout << "D -> Posto disponibile" << endl;
        if (piano.size() > 0) {
            cout << "A -> Ascensore" << endl;
        }
    }
}
veicolo::veicolo(int identificativo, int velocita_minima, int velocita_massima, int posti_totali, int percentuale_posti, int fermate_totali) {
    _identificativo = identificativo;
    _velocita_minima = velocita_minima;
    _velocita_massima = velocita_massima;
    _posti = posti_totali;
    _percentuale_posti = percentuale_posti;
    _fermate_totali = fermate_totali;
    struttura();
}
veicolo::~veicolo() {
    _identificativo = -1;
    _velocita_minima = -1;
    _velocita_massima = -1;
    _posti = -1;
    _percentuale_posti = -1;
    _fermate_totali = -1;
    _fermata_attuale = -1;
    _stato = false;
}
void veicolo::piani() {
    //calcolo e assegnazione del numero di piani
    int numero_piani = 1;
    if ((float)_posti/costante.posti_massimi_per_piano == (int)_posti/costante.posti_massimi_per_piano) { //se il risultato è float si arrotonda per eccesso
        numero_piani = (int)_posti/costante.posti_massimi_per_piano;
    }
    else if ((float)_posti/costante.posti_massimi_per_piano > (int)_posti/costante.posti_massimi_per_piano){
        numero_piani = (int)_posti/costante.posti_massimi_per_piano + 1;
    }
    piano.resize(numero_piani);
    funzione.debug(costante.debug, "Numero di piani " + to_string(numero_piani));
}
void veicolo::posti() {
    //assegnazione numero di posti per i vari piani compreso l'autista al primo piano soltanto
    int posti_rimanenti = _posti;
    int posti_assegnati = 0;
    piano.at(0).posti(1); //autista al piano 0
    int n_piano = 0;
    while (posti_rimanenti != 0) {
        if (posti_assegnati <  costante.posti_massimi_per_piano) {
            piano.at(n_piano).posti(piano.at(n_piano).posti() + 1);
            posti_assegnati++;
            posti_rimanenti--;
        }
        else {
            n_piano++;
            posti_assegnati = 0;
        }
    }
    //debug
    if (costante.debug) {
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            funzione.debug(costante.debug, to_string(piano.at(n_piano).posti()) + " posti al piano " + to_string(n_piano));
        }
    }
}
void veicolo::dimensioni() {
    //calcolo dimensioni del primo piano per poi inizializzare ogni piano
    int numero_righe;
    int numero_colonne;
    //calcolo numero righe
    {
        numero_righe = 5 + 2; //righe posti + righe bordi
        funzione.debug(costante.debug, "Numero di righe calcolate " + to_string(numero_righe));
    }
    //calcolo numero colonne
    {
        int resto_posti; //sempre per caire se esiste una fila con meno di quattro posti
        numero_colonne = (piano.at(0).posti() - 5) / 4;
        resto_posti = (piano.at(0).posti() - 5 - 1) - (numero_colonne * 4);
        numero_colonne += (resto_posti != 0) ? 1 : 0; //fila di posti con meno di 4 posti
        numero_colonne += 2 + 2; //bordo destra e sinistra + autista
        numero_colonne = (numero_colonne * 2) - 2;
        numero_colonne += 1; //file per l'ascensore e robot
        funzione.debug(costante.debug, "Numero di colonne calcolate " + to_string(numero_colonne));
    }
    for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
        piano.at(n_piano).casella.resize(numero_colonne);
        for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
            piano.at(n_piano).casella.at(colonna).resize(numero_righe);
        }
    }
    funzione.debug(costante.debug, "Dimensioni bus " + to_string(piano.at(0).casella.size()) + "x" + to_string(piano.at(0).casella.at(0).size()));
}
void veicolo::tipizzazione() {
    //tipizzazione della caselle
    for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
        int posti_da_assegnare = piano.at(n_piano).posti();
        for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
            for (int riga = 0; riga < piano.at(n_piano).casella.at(colonna).size(); riga++) {
                //perimetro
                if (colonna == 0 || riga == 0 || colonna == piano.at(n_piano).casella.size() - 1 || riga == piano.at(n_piano).casella.at(riga).size() - 1) {
                    //assegnazione bordi
                    if (n_piano == 0) {
                        if (colonna == piano.at(n_piano).casella.size() - 4 && riga == piano.at(n_piano).casella.at(riga).size() - 1) {
                            //assegnazione entrata
                            piano.at(n_piano).casella.at(colonna).at(riga).entrata();
                            piano.at(n_piano).casella.at(colonna).at(riga).libero();
                            entrata.assegna(colonna, riga);
                        }
                        else if (colonna == 2 && riga == piano.at(n_piano).casella.at(riga).size() - 1) {
                            //assegnazione uscita
                            piano.at(n_piano).casella.at(colonna).at(riga).uscita();
                            piano.at(n_piano).casella.at(colonna).at(riga).libero();
                            uscita.assegna(colonna, riga);
                        }
                        else {
                            //tutto il bordo rimanente
                            piano.at(n_piano).casella.at(colonna).at(riga).bordo();
                            piano.at(n_piano).casella.at(colonna).at(riga).occupato();
                        }
                    }
                    else {
                        //bordo degli altri piani non ha ne entrate ne uscite
                        piano.at(n_piano).casella.at(colonna).at(riga).bordo();
                        piano.at(n_piano).casella.at(colonna).at(riga).occupato();
                    }
                }
                else { //interno del bus
                    if (posti_da_assegnare > 0) {
                        if (colonna == 1) {
                            //assegnazione ultima fila da 5 posti
                            piano.at(n_piano).casella.at(colonna).at(riga).sedile();
                            piano.at(n_piano).casella.at(colonna).at(riga).libero();
                            posti_da_assegnare--;
                        }
                        else {
                            //assegnazione posti a sedere file centrali
                            if (colonna%2 != 0) {
                                if (riga != 3) {
                                    piano.at(n_piano).casella.at(colonna).at(riga).sedile();
                                    piano.at(n_piano).casella.at(colonna).at(riga).libero();
                                    posti_da_assegnare--;
                                }
                            }
                        }
                        //posto del guidatore
                        if (posti_da_assegnare == 1 && n_piano == 0) {
                            piano.at(n_piano).casella.at(piano.at(n_piano).casella.size() - 3).at(1).sedile();
                            piano.at(n_piano).casella.at(piano.at(n_piano).casella.size() - 3).at(1).libero();
                            autista.assegna((int)piano.at(n_piano).casella.size() - 3, 1);
                            posti_da_assegnare--;
                        }
                    }
                    //robot
                    else if (colonna == piano.at(n_piano).casella.size() - 2 && riga == piano.at(n_piano).casella.at(colonna).size() - 2) {
                        piano.at(n_piano).casella.at(colonna).at(riga).robot();
                        piano.at(n_piano).casella.at(colonna).at(riga).libero();
                        piano.at(n_piano).dyson.posizione.assegna(colonna, riga);
                    }
                    //ascensore
                    else if (piano.size() > 1) {
                        if (colonna == piano.at(n_piano).casella.size() - 2 && riga == piano.at(n_piano).casella.at(colonna).size() - 3) {
                            piano.at(n_piano).casella.at(colonna).at(riga).ascensore();
                            piano.at(n_piano).casella.at(colonna).at(riga).libero();
                            ascensore.assegna(colonna, riga);
                        }
                    }
                }
                if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == -1) {
                    piano.at(n_piano).casella.at(colonna).at(riga).percorribile();
                    piano.at(n_piano).casella.at(colonna).at(riga).libero();
                }
            }
        }
    }
    //debug
    if (costante.debug) {
        //stampa a video bus con valori dei tipi
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            funzione.debug(costante.debug, "Tipo piano " + to_string(n_piano));
            for (int riga = 0; riga < piano.at(n_piano).casella.at(riga).size(); riga++) {
                for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
                    cout << "[" << piano.at(n_piano).casella.at(colonna).at(riga).tipo() << "]";
                }
                cout << endl;
            }
            cout << endl;
        }
        //stampa a video bus con valori degli stati
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            funzione.debug(costante.debug, "Stato piano " + to_string(n_piano));
            for (int riga = 0; riga < piano.at(n_piano).casella.at(riga).size(); riga++) {
                for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
                    cout << "[" << piano.at(n_piano).casella.at(colonna).at(riga).stato() << "]";
                }
                cout << endl;
            }
            cout << endl;
        }
    }
}
void veicolo::sedute() {
    //assegnazione dei posti dove ci si può sedere
    int posti_utilizzabili = (_posti * _percentuale_posti) / 100;
    funzione.debug(costante.debug, "Posti utilizzabili " + to_string(posti_utilizzabili));
    semaforo.set(posti_utilizzabili + 1);
    int intervallo_posti = (_posti / posti_utilizzabili) - 1;
    funzione.debug(costante.debug, "Intervallo posti " + to_string(intervallo_posti));
    int count = 0;
    for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
        for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
            for (int riga = 0; riga < piano.at(n_piano).casella.at(colonna).size(); riga++) {
                if (piano.at(n_piano).casella.at(colonna).at(riga).tipo() == costante.tipo.sedile) {
                    if (count == 0 && intervallo_posti != 0 && colonna != piano.at(n_piano).casella.size() - 3 && riga != 1) {
                        piano.at(n_piano).casella.at(colonna).at(riga).disponibile(false);
                        count++;
                    }
                    else {
                        if (count == intervallo_posti) {
                            count = 0;
                        }
                        else {
                            count ++;
                        }
                    }
                }
            }
        }
    }
    //debug
    if (costante.debug) {
        //stampa a video bus con valori dei tipi
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            funzione.debug(costante.debug, "Tipo piano " + to_string(n_piano));
            for (int riga = 0; riga < piano.at(n_piano).casella.at(riga).size(); riga++) {
                for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
                    cout << "[" << piano.at(n_piano).casella.at(colonna).at(riga).tipo() << "]";
                }
                cout << endl;
            }
            cout << endl;
        }
        //stampa a video bus con valori degli stati
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            funzione.debug(costante.debug, "Stato piano " + to_string(n_piano));
            for (int riga = 0; riga < piano.at(n_piano).casella.at(riga).size(); riga++) {
                for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
                    cout << "[" << piano.at(n_piano).casella.at(colonna).at(riga).stato() << "]";
                }
                cout << endl;
            }
            cout << endl;
        }
    }
}
void veicolo::struttura() {
    piani();
    posti();
    dimensioni();
    tipizzazione();
    sedute();
}
void veicolo::persone(int numero_persone) {
    persona.resize(numero_persone + 1);
    persona.at(0).genera(costante.ruolo.autista, persona.at(0).persone(), 0, _fermate_totali);
    for (int n_persona = 1; n_persona < persona.size(); n_persona++) {
        int salita = funzione.random(0, _fermate_totali - 1);
        int discesa = funzione.random(salita + 1, _fermate_totali);
        persona.at(n_persona).genera(costante.ruolo.autista, persona.at(n_persona).persone(), salita, discesa);
        funzione.debug(costante.debug, "Persona " + to_string(n_persona) + " generata con salita " + to_string(salita) + " e discesa " + to_string(discesa));
    }
}
void veicolo::avviobusthread() {
    vector<int> attesa_persona;
    for (int fermata = 0; fermata <= _fermate_totali; fermata++) {
        _fermata_attuale = fermata;
        funzione.debug(costante.debug, "Fermata attuale " + to_string(_fermata_attuale));
        //salita dell'autista prima fermata
        if (fermata == 0) {
            persona.at(0).entrata(ref(_fermata_attuale), ref(piano), ref(ascensore), ref(entrata), ref(uscita), ref(semaforo), ref(autista));
            persona.at(0).seduta();
        }
        //pulizia dei posti
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            piano.at(n_piano).avviorobot();
            funzione.debug(costante.debug, "Robot " + to_string(n_piano) + " avviato");
        }
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            piano.at(n_piano).attendirobot();
            funzione.debug(costante.debug, "Robot " + to_string(n_piano) + " fermato");
        }
        //salita persone
        empty_swap(attesa_persona);
        for (int n_persona = 1; n_persona < persona.size(); n_persona++) {
            if (persona.at(n_persona).salita() == _fermata_attuale) {
                funzione.debug(costante.debug, "Persona " + to_string(n_persona) + " alla fermata");
                persona.at(n_persona).entrata(ref(_fermata_attuale), ref(piano), ref(ascensore), ref(entrata), ref(uscita), ref(semaforo), ref(autista));
                attesa_persona.push_back(n_persona);
            }
        }
        for (int n_persona = 0; n_persona < attesa_persona.size(); n_persona++) {
            persona.at(attesa_persona.at(n_persona)).seduta();
        }
        //verso la prossima fermata
        startmotore();
        sleep(costante.durata.percorso);
        stopmotore();
        //arrivo alla fermata
        empty_swap(attesa_persona);
        //discesa delle persone
        for (int n_persona = 1; n_persona < persona.size(); n_persona++) {
            if (persona.at(n_persona).discesa() == _fermata_attuale  && persona.at(n_persona).stato() == costante.stato.seduto) {
                funzione.debug(costante.debug, "Persona " + to_string(n_persona) + " uscita");
                persona.at(n_persona).alzata(ref(_fermata_attuale), ref(piano), ref(ascensore), ref(entrata), ref(uscita), ref(semaforo));
                attesa_persona.push_back(n_persona);
            }
        }
        for (int n_persona = 0; n_persona < attesa_persona.size(); n_persona++) {
            persona.at(attesa_persona.at(n_persona)).uscita();
        }
        //discesa autista
        if (persona.at(0).discesa() == _fermata_attuale) {
            funzione.debug(costante.debug, "Persona " + to_string(0) + " alla fermata");
            persona.at(0).alzata(ref(_fermata_attuale), ref(piano), ref(ascensore), ref(entrata), ref(uscita), ref(semaforo));
            persona.at(0).uscita();
        }
        //pulizia dei posti
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            piano.at(n_piano).avviorobot();
            funzione.debug(costante.debug, "Robot " + to_string(n_piano) + " avviato");
        }
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            piano.at(n_piano).attendirobot();
            funzione.debug(costante.debug, "Robot " + to_string(n_piano) + " fermato");
        }
    }
    if (costante.debug) {
        //stampa a video bus con valori dei tipi
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            funzione.debug(costante.debug, "Tipo piano " + to_string(n_piano));
            for (int riga = 0; riga < piano.at(n_piano).casella.at(riga).size(); riga++) {
                for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
                    cout << "[" << piano.at(n_piano).casella.at(colonna).at(riga).utente() << "]";
                }
                cout << endl;
            }
            cout << endl;
        }
        //stampa a video bus con valori degli stati
        for (int n_piano = 0; n_piano < piano.size(); n_piano++) {
            funzione.debug(costante.debug, "Stato piano " + to_string(n_piano));
            for (int riga = 0; riga < piano.at(n_piano).casella.at(riga).size(); riga++) {
                for (int colonna = 0; colonna < piano.at(n_piano).casella.size(); colonna++) {
                    cout << "[" << piano.at(n_piano).casella.at(colonna).at(riga).stato() << "]";
                }
                cout << endl;
            }
            cout << endl;
        }
    }
}

#endif /* veicolo_h */
