//
//  livello.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef livello_h
#define livello_h

class livello {
    int _identificativo;
    int _posti = 0;
    static inline int _posti_sporchi = 0;
    mutex_wrapper _sporco;
public:
    robot dyson;
    vector<vector<casella>> casella;
    void posti(int posti) { _posti = posti; }
    int posti() { return _posti; }
    void avviorobot() { dyson.cervello = thread(&livello::avviorobotthread, this); }
    void avviorobotthread();
    void attendirobot() { dyson.cervello.join(); }
    void sporco(bool value) { _sporco.lock(); _posti_sporchi = (value) ? _posti_sporchi + 1 : _posti_sporchi - 1; _sporco.unlock(); }
};
void livello::avviorobotthread() {
    if (_posti_sporchi > 0) {
        for (int riga = dyson.posizione.dammi('y'); riga > 0; riga--) {
            if (riga%2 != 0) {
                for (int colonna = dyson.posizione.dammi('x'); colonna > 0; colonna--) {
                    funzione.debug(costante.debug, "Posizione robot " + to_string(dyson.posizione.dammi('x')) + "-" + to_string(dyson.posizione.dammi('y')));
                    if (casella.at(colonna).at(riga).tipo() == costante.tipo.sedile && casella.at(colonna).at(riga).stato() == costante.stato.sporco) {
                        casella.at(colonna).at(riga).mutex.lock();
                        casella.at(colonna).at(riga).libero();
                        casella.at(colonna).at(riga).mutex.unlock();
                        funzione.debug(costante.debug, "Posto pulito");                        sporco(false);
                    }
                    usleep(costante.velocita.robot);
                    if (colonna > 1) {
                        dyson.posizione.decrementax();
                    }
                }
            }
            else {
                for (int colonna = dyson.posizione.dammi('x'); colonna < casella.size() - 1; colonna++) {
                    funzione.debug(costante.debug, "Posizione robot " + to_string(dyson.posizione.dammi('x')) + "-" + to_string(dyson.posizione.dammi('y')));
                    if (casella.at(colonna).at(riga).tipo() == costante.tipo.sedile && casella.at(colonna).at(riga).stato() == costante.stato.sporco) {
                        casella.at(colonna).at(riga).mutex.lock();
                        casella.at(colonna).at(riga).libero();
                        casella.at(colonna).at(riga).mutex.unlock();
                        funzione.debug(costante.debug, "Posto pulito");
                        sporco(false);
                    }
                    usleep(costante.velocita.robot);
                    if (colonna < casella.size() - 2) {
                        dyson.posizione.incrementax();
                    }
                }
            }
            usleep(costante.velocita.robot);
            if (riga > 1) {
                dyson.posizione.decrementay();
            }
        }
        for (int colonna = dyson.posizione.dammi('x'); colonna < casella.size() - 2; colonna++) {
            funzione.debug(costante.debug, "Posizione robot " + to_string(dyson.posizione.dammi('x')) + "-" + to_string(dyson.posizione.dammi('y')));
            usleep(costante.velocita.robot);
            dyson.posizione.incrementax();
        }
        for (int riga = dyson.posizione.dammi('y'); riga < casella.at(0).size() - 2; riga++) {
            funzione.debug(costante.debug, "Posizione robot " + to_string(dyson.posizione.dammi('x')) + "-" + to_string(dyson.posizione.dammi('y')));
            usleep(costante.velocita.robot);
            dyson.posizione.incrementay();
        }
    }
}

    
#endif /* livello_h */
