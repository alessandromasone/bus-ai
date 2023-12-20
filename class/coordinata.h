//
//  coordinata.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef coordinata_h
#define coordinata_h

class coordinata {
private:
    int _x;
    int _y;
public:
    coordinata() { _x = 0; _y = 0; }
    coordinata(int x, int y) { _x = x; _y = y; }
    ~coordinata() { _x = 0; _y = 0; }
    void assegna(int x, int y) { _x = x; _y = y; }
    int dammi(char c) { if (c == 'x')  return _x; else if (c == 'y') return _y; return 0; }
    void incrementax() { _x++; }
    void incrementay() { _y++; }
    void decrementax() { _x--; }
    void decrementay() { _y--; }
};

#endif /* coordinata_h */
