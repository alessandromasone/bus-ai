//
//  costanti.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef costanti_h
#define costanti_h

namespace costanti {
class costanti{
private:
    class tipi {
    public:
        static const int bordo = 1;
        static const int sedile = 2;
        static const int entrata = 3;
        static const int uscita = 4;
        static const int ascensore = 5;
        static const int robot = 6;
        static const int percorribile = 7;
    };
    class stati {
    public:
        static const int occupato = 1;
        static const int libero = 2;
        static const int nondisponibile = 3;
        static const int disponibile = 4;
        static const int sporco = 5;
        static const int seduto = 6;
        static const int alzato = 7;
    };
    class ruoli {
    public:
        static const int autista = 0;
        static const int passeggero = 1;
    };
    class durate {
    public:
        static const int fermo = 1;
        static const int percorso = 3;
    };
    class max_min {
    public:
        static const int massimo = 200000;
        static const int minimo = 100000;
    };
    class speed {
    public:
        max_min persona;
        static const int stampa = 10000;
        static const int robot = 100000;
        
    };
public:
    static const int posti_massimi_per_piano = 69;
    bool debug = false;
    tipi tipo;
    stati stato;
    ruoli ruolo;
    durate durata;
    speed velocita;
};
static costanti costante;
}

using namespace costanti;

#endif /* costanti_h */
