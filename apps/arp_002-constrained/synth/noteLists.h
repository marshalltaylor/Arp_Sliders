#ifndef NOTELISTS_H
#define NOTELISTS_H

struct seqNote_t
{
  float time;
  float length;
  uint8_t pitch;
  uint8_t velocity;
  uint8_t accent;
};

#endif
