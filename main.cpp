//
//  main.cpp
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//  g++ main.cpp -std=c++20 && ./a.out

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <unistd.h>
#include <cstdio>
#include <random>
#include <cstdlib>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "class.h"

int main(int argc, const char * argv[]) {
    //vettore per l'input da perte del file txt
    vector<int> vettore;
    //controllo dei valore del vettore
    if (funzione.input(ref(vettore)) > 0) { return 1; }
    //inizializzazione del bus
    veicolo bus(0, 0, 0, vettore.at(2), vettore.at(1), vettore.at(0));
    //generazione persone
    bus.persone(vettore.at(3));
    //avvio della stampa e del movimento del bus
    bus.avviostampa();
    bus.avviobus();
    //attesa della fine stampa e fine del movimento del bus
    bus.stopbus();
    bus.stopstampa();
    cout << "Hello, World!\n";
    return 0;
}
