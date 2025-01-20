#pragma once

typedef uint8_t DataByte;
typedef uint8_t Channel;
typedef uint8_t byte;
typedef uint8_t MidiType;


class MidiTestFixture
{
public:
     MidiTestFixture(){};
    ~MidiTestFixture(){};

    void sendNoteOn(DataByte inNoteNumber,
                           DataByte inVelocity,
                           Channel inChannel);

    void sendNoteOff(DataByte inNoteNumber,
                            DataByte inVelocity,
                            Channel inChannel);

    void sendProgramChange(DataByte inProgramNumber,
                                  Channel inChannel);

    void sendControlChange(DataByte inControlNumber,
                                  DataByte inControlValue,
                                  Channel inChannel);

    void sendPitchBend(int inPitchValue,    Channel inChannel);
    void sendPitchBend(double inPitchValue, Channel inChannel);

    void sendPolyPressure(DataByte inNoteNumber,
                                 DataByte inPressure,
                                 Channel inChannel) __attribute__ ((deprecated));

    void sendAfterTouch(DataByte inPressure,
                               Channel inChannel);
    void sendAfterTouch(DataByte inNoteNumber,
                               DataByte inPressure,
                               Channel inChannel);

    void sendSysEx(unsigned inLength,
                          const byte* inArray,
                          bool inArrayContainsBoundaries = false);

    void sendTimeCodeQuarterFrame(DataByte inTypeNibble,
                                         DataByte inValuesNibble);
    void sendTimeCodeQuarterFrame(DataByte inData);

    void sendSongPosition(unsigned inBeats);
    void sendSongSelect(DataByte inSongNumber);
    void sendTuneRequest();
    void sendRealTime(MidiType inType);

    void beginRpn(unsigned inNumber,
                         Channel inChannel);
    void sendRpnValue(unsigned inValue,
                             Channel inChannel);
    void sendRpnValue(byte inMsb,
                             byte inLsb,
                             Channel inChannel);
    void sendRpnIncrement(byte inAmount,
                                 Channel inChannel);
    void sendRpnDecrement(byte inAmount,
                                 Channel inChannel);
    void endRpn(Channel inChannel);

    void beginNrpn(unsigned inNumber,
                          Channel inChannel);
    void sendNrpnValue(unsigned inValue,
                              Channel inChannel);
    void sendNrpnValue(byte inMsb,
                              byte inLsb,
                              Channel inChannel);
    void sendNrpnIncrement(byte inAmount,
                                  Channel inChannel);
    void sendNrpnDecrement(byte inAmount,
                                  Channel inChannel);
    void endNrpn(Channel inChannel);

};

extern MidiTestFixture MIDI;
