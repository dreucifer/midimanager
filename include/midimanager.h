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

typedef struct
{
  bool filterNote = false;
  bool filterAftertouch = false;
  bool filterCC = false;
  bool filterPC = false;
  bool filterClock = false;
  bool filterSong = false;
  bool filterTransport = false;
  bool filterSysEx = false;
  uint16_t filterChannel = 0b0000000000000000;
  byte filterPorts = 0b00000000;
} MidiInputFilter;

typedef struct
{
  bool filterNote = false;
  bool filterAftertouch = false;
  bool filterCC = false;
  bool filterPC = false;
  bool filterClock = false;
  bool filterSong = false;
  bool filterTransport = false;
  bool filterSysEx = false;
  uint16_t filterChannel = 0b0000000000000000;
} MidiOutputFilter;

bool runInputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiInputFilter filter);
bool runOutputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiOutputFilter filter);
void handleOnPPQN(uint32_t tick);
void handleOnSync24(uint32_t tick);

class MidiDuplex {
  private:
    int pair;
  
  public:
    explicit MidiDuplex(int port);
    void MessageCallback(const midi::Message<128> message);
};