//
//  robot.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef robot_h
#define robot_h

class robot {
    int _stato;
    int _velocita;
public:
    thread cervello;
    coordinata posizione;
};

#endif /* robot_h */
