#include <Arduino.h>

#include <MIDI.h>
#include <USBHost_t36.h> // access to USB MIDI devices (plugged into 2nd USB port)
#include <usb_midi.h>

#include <uClock.h>

#pragma once

// Set up a list of physical midi ports, message archetypes, and a boolean filter table for input and output

#define CHKVAL 22448238

enum MidiPair
{
  MIDI_0 = 0,
  MIDI_1 = 1,
  MIDI_2 = 2,
  MIDI_3 = 3,
  MIDI_4 = 4,
  MIDI_5 = 5,
  MIDI_6 = 6,
  MIDI_7 = 7
};

enum MidiFilters
{
  NOTES = 0,
  CLOCK = 1,
  TRANSPORT = 2,
  PCHANGE = 3
};

typedef struct
{
  byte midiInputFilters[8] = {0b0010, 0b1101, 0b1100, 0b1100, 0b1100, 0b1101, 0b1100, 0b1100};
  byte midiOutputFilters[8] = {0b0101, 0b0001, 0b0111, 0b0111, 0b0001, 0b0011, 0b0110, 0b1110};
  byte midiOutputRoutes[8] = {0b11111110, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11011111, 0b11111111, 0b11111111};
  uint16_t midiOutputChannels[8] = {0b1111111000000000, 0b1111111111111111, 0b1111111111111111, 0b1111111111111111, 0b0000000000000100, 0b1111111111111111, 0b1111111111111111, 0b1111111111111111};

  uint32_t chkval = CHKVAL;
} MidiFilterData;

bool runInputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiPair pair);
bool runOutputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiPair pair);
void handleOnPPQN(uint32_t tick);
void handleOnSync24(uint32_t tick);


#define CREATE_DO_HANDLE_MESSAGE_MIDI(Name, Index)                   \
  void doHandleMessage##Name(const midi::Message<128> &message)      \
  {                                                                  \
    byte outputRoutes = midiFilterData.midiOutputRoutes[Index];      \
    midi::MidiType type = message.type;                              \
    midi::DataByte data1 = message.data1;                            \
    midi::DataByte data2 = message.data2;                            \
    midi::Channel channel = message.channel;                         \
    usbMIDI.send(type, data1, data2, channel, Index);                \
    if (runInputFilter(type, data1, channel, Index))                 \
    {                                                                \
      for (int pair = 0; pair < 8; pair++)                           \
      {                                                              \
        if ((outputRoutes >> pair) % 2)                              \
        {                                                            \
          if (runOutputFilter(type, data1, channel, (MidiPair)pair)) \
          {                                                          \
            midilist[pair]->send(type, data1, data2, channel);       \
          }                                                          \
        }                                                            \
      }                                                              \
    }                                                                \
  };

#define CREATE_DO_HANDLE_MESSAGE_USB(Name, Index)                      \
  void doHandleMessage##Name##_TUSB(const midi::Message<128> &message) \
  {                                                                    \
    midi::MidiType type = message.type;                                \
    midi::DataByte data1 = message.data1;                              \
    midi::DataByte data2 = message.data2;                              \
    midi::Channel channel = message.channel;                           \
    usbMIDI.send(type, data1, data2, channel, (int)Index);             \
    for (int pair = 0; pair < 8; pair++)                               \
    {                                                                  \
      if (runOutputFilter(type, data1, channel, (MidiPair)pair))       \
      {                                                                \
        midilist[pair]->send(type, data1, data2, channel);             \
      }                                                                \
    }                                                                  \
  };
