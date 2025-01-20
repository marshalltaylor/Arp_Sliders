#ifndef TASKMIDI_H
#define TASKMIDI_H

#ifdef __cplusplus
extern "C" {
#endif

void taskMidiWork(void * argument);
void taskMidiRx(void * argument);
//void taskSequencePlayerProcess(void * argument);

#ifdef __cplusplus
}
#endif

#endif
