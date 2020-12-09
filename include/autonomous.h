#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_


void runStandardAuto();       // Standard 15 seconds autonomous
void runExtendedAuto();       // Run the extended 45sec autonomous code
void runSkillAuto();          // Run the skill challenge (programming skill code)

// Set the global autonomous variables used to communicate across code modules
extern int autonomousTime;           // length of autonomous routine, may either be
                                     // be 15 sec (default) 45sec or 60 second

extern int fieldSide;                // side of field we play on - 1 == RED 2 ==  BLUE
#endif
