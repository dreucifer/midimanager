/* This creates 8 Serial midi pairs, hosts 8 USB MIDI devices as a host,
   and creates 8 Virtual MIDI cables to the computer over USB.

   Some basic MIDI routing and filtering is implemented.

   I have also implemented a high resolution uClock core.
 */
#include "midimanager.h"

// Create the Serial MIDI ports
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI0);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial4, MIDI3);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial5, MIDI4);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial6, MIDI5);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial7, MIDI6);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial8, MIDI7);

midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *midilist[8] = {
    &MIDI0,
    &MIDI1,
    &MIDI2,
    &MIDI3,
    &MIDI4,
    &MIDI5,
    &MIDI6,
    &MIDI7,
};

MidiDuplex duplex0 = MidiDuplex(0);
MidiDuplex duplex1 = MidiDuplex(1);
MidiDuplex duplex2 = MidiDuplex(2);
MidiDuplex duplex3 = MidiDuplex(3);
MidiDuplex duplex4 = MidiDuplex(4);
MidiDuplex duplex5 = MidiDuplex(5);
MidiDuplex duplex6 = MidiDuplex(6);
MidiDuplex duplex7 = MidiDuplex(7);

// Create the ports for USB devices plugged into Teensy's 2nd USB port (via hubs)
USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
USBHub hub4(myusb);
MIDIDevice t_uMIDI0(myusb);
MIDIDevice t_uMIDI1(myusb);
MIDIDevice t_uMIDI2(myusb);
MIDIDevice t_uMIDI3(myusb);
MIDIDevice t_uMIDI4(myusb);
MIDIDevice t_uMIDI5(myusb);
MIDIDevice t_uMIDI6(myusb);
MIDIDevice t_uMIDI7(myusb);

MIDIDevice *t_usbmidilist[8] = {
    &t_uMIDI0,
    &t_uMIDI1,
    &t_uMIDI2,
    &t_uMIDI3,
    &t_uMIDI4,
    &t_uMIDI5,
    &t_uMIDI6,
    &t_uMIDI7};

MidiInputFilter MidiInputFilters[8];
MidiOutputFilter MidiOutputFilters[8];

void handleNoteOn(midi::Channel channel, midi::DataByte data1, midi::DataByte data2)
{
  midi::Channel _channel = channel;
  midi::DataByte _data1 = data1;
  midi::DataByte _data2 = data2;
  usbMIDI.sendNoteOn(_data1, _data2, _channel, 0);
  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterNote)
    {
      midilist[pair]->sendNoteOn(_data1, _data2, _channel);
    }
  }
};

void handleNoteOff(midi::Channel channel, midi::DataByte data1, midi::DataByte data2)
{
  midi::Channel _channel = channel;
  midi::DataByte _data1 = data1;
  midi::DataByte _data2 = data2;
  usbMIDI.sendNoteOff(_data1, _data2, _channel, 0);
  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterNote)
    {
      midilist[pair]->sendNoteOff(_data1, _data2, _channel);
    }
  }
};

void handlePitchBend(midi::Channel channel, int value)
{
  midi::Channel _channel = channel;
  int _value = value;

  usbMIDI.sendPitchBend(_value, _channel, 0);
  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterNote)
    {
      midilist[pair]->sendPitchBend(_value, _channel);
    }
  }
}

void handleControlChange(midi::Channel channel, byte data1, byte data2)
{
  midi::Channel _channel = channel;
  byte _control = data1;
  byte _value = data2;

  usbMIDI.sendControlChange(_control, _value, _channel, 0);
  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterCC)
    {
      midilist[pair]->sendControlChange(_control, _value, _channel);
    }
  }
}

void handleProgramChange(midi::Channel channel, byte data)
{
  midi::Channel _channel = channel;
  byte _program = data;

  usbMIDI.sendProgramChange(_program, _channel, 0);

  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterPC)
    {
      midilist[pair]->sendProgramChange(_program, _channel);
    }
  }
}

void handleAfterTouchPoly(midi::Channel channel, byte note, byte velocity)
{
  midi::Channel _channel = channel;
  byte _note = note;
  byte _velocity = velocity;

  usbMIDI.sendAfterTouchPoly(_note, _velocity, _channel, 0);

  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterAftertouch)
    {
      midilist[pair]->sendAfterTouch(_velocity, _channel);
    }
  }
}

void handleAfterTouchChannel(midi::Channel channel, byte velocity)
{
  midi::Channel _channel = channel;
  byte _velocity = velocity;

  usbMIDI.sendAfterTouch(_velocity, _channel, 0);

  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterAftertouch)
    {
      midilist[pair]->sendAfterTouch(_velocity, _channel);
    }
  }
}

void handleExternalClock()
{
  uClock.clockMe();
};

// Send clock sync to the external midi ports and the USB interface
void handleOnPPQN(uint32_t tick)
{
  for (int pair = 0; pair < 8; pair++)
  {
    MidiOutputFilter outputFilter = MidiOutputFilters[pair];
    if (outputFilter.filterClock)
    {
      return;
    }
    midilist[pair]->sendRealTime(midi::Tick);
  }
}

// Send clock sync to the external midi ports and the USB interface
void handleOnSync24(uint32_t tick)
{
  usbMIDI.sendRealTime(usbMIDI.Clock);
  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterClock)
    {
      midilist[pair]->sendRealTime(midi::Clock);
    }
  }
}

void handleStart()
{
  usbMIDI.sendRealTime(usbMIDI.Start);
  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterTransport)
    {
      midilist[pair]->sendRealTime(midi::Start);
    }
  }
}

void handleStop()
{
  usbMIDI.sendRealTime(usbMIDI.Stop);
  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterTransport)
    {
      midilist[pair]->sendRealTime(midi::Stop);
    }
  }
}

void handleContinue()
{
  usbMIDI.sendRealTime(usbMIDI.Continue);
  for (int pair = 0; pair < 8; pair++)
  {
    if (!MidiOutputFilters[pair].filterTransport)
    {
      midilist[pair]->sendRealTime(midi::Continue);
    }
  }
}

void handleMIDI_0Message(const midi::Message<128> &message)
{
  midi::Message<128> _message = message;
  duplex0.MessageCallback(_message);
}

void handleMIDI_1Message(const midi::Message<128> &message)
{
  midi::Message<128> _message = message;
  duplex1.MessageCallback(_message);
}

void handleMIDI_2Message(const midi::Message<128> &message)
{
  midi::Message<128> _message = message;
  duplex2.MessageCallback(_message);
}

void handleMIDI_3Message(const midi::Message<128> &message)
{
  midi::Message<128> _message = message;
  duplex3.MessageCallback(_message);
}

void handleMIDI_4Message(const midi::Message<128> &message)
{
  midi::Message<128> _message = message;
  duplex4.MessageCallback(_message);
}

void handleMIDI_5Message(const midi::Message<128> &message)
{
  midi::Message<128> _message = message;
  duplex5.MessageCallback(_message);
}

void handleMIDI_6Message(const midi::Message<128> &message)
{
  midi::Message<128> _message = message;
  duplex6.MessageCallback(_message);
}

void handleMIDI_7Message(const midi::Message<128> &message)
{
  midi::Message<128> _message = message;
  duplex7.MessageCallback(_message);
}

void setup()
{
  Serial.begin(115200);

  // Manually set the input and output filters. Replace with read_or_initialize from persistent storage.
  MidiInputFilters[0].filterClock = false;
  MidiInputFilters[1].filterClock = true;
  MidiInputFilters[2].filterClock = true;
  MidiInputFilters[3].filterClock = true;
  MidiInputFilters[4].filterClock = true;
  MidiInputFilters[5].filterClock = true;
  MidiInputFilters[6].filterClock = true;
  MidiInputFilters[7].filterClock = true;
  MidiInputFilters[1].filterChannel = 0b0000000000000001;

  MidiOutputFilters[0].filterPC = true;
  MidiOutputFilters[2].filterTransport = true;
  MidiOutputFilters[4].filterTransport = true;
  MidiOutputFilters[5].filterTransport = true;
  MidiOutputFilters[6].filterNote = true;
  MidiOutputFilters[7].filterNote = true;

  // Run through each midi port, begin, turn off thru, and set the message callback.
  // Set the first midi port to the external clock master
  for (int port = 0; port < 8; port++)
  {
    midilist[port]->begin(MIDI_CHANNEL_OMNI);
    midilist[port]->turnThruOff();
    switch (port)
    {
    case 0:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI0.setHandleClock(handleExternalClock);
      }
      MIDI0.setHandleMessage(handleMIDI_0Message);
      t_uMIDI0.setHandleNoteOn(handleNoteOn);
      t_uMIDI0.setHandleNoteOff(handleNoteOff);
      t_uMIDI0.setHandlePitchChange(handlePitchBend);
      t_uMIDI0.setHandleControlChange(handleControlChange);
      t_uMIDI0.setHandleProgramChange(handleProgramChange);
      t_uMIDI0.setHandleAfterTouchPoly(handleAfterTouchPoly);
      t_uMIDI0.setHandleAfterTouchChannel(handleAfterTouchChannel);
      break;
    case 1:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI1.setHandleClock(handleExternalClock);
      }
      MIDI1.setHandleMessage(handleMIDI_1Message);
      t_uMIDI1.setHandleNoteOn(handleNoteOn);
      t_uMIDI1.setHandleNoteOff(handleNoteOff);
      break;
    case 2:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI2.setHandleClock(handleExternalClock);
      }
      MIDI2.setHandleMessage(handleMIDI_2Message);
      t_uMIDI2.setHandleNoteOn(handleNoteOn);
      t_uMIDI2.setHandleNoteOff(handleNoteOff);
      break;
    case 3:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI3.setHandleClock(handleExternalClock);
      }
      MIDI3.setHandleMessage(handleMIDI_3Message);
      t_uMIDI3.setHandleNoteOn(handleNoteOn);
      t_uMIDI3.setHandleNoteOff(handleNoteOff);
      break;
    case 4:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI4.setHandleClock(handleExternalClock);
      }
      MIDI4.setHandleMessage(handleMIDI_4Message);
      t_uMIDI4.setHandleNoteOn(handleNoteOn);
      t_uMIDI4.setHandleNoteOff(handleNoteOff);
      break;
    case 5:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI5.setHandleClock(handleExternalClock);
      }
      MIDI5.setHandleMessage(handleMIDI_5Message);
      t_uMIDI5.setHandleNoteOn(handleNoteOn);
      t_uMIDI5.setHandleNoteOff(handleNoteOff);
      break;
    case 6:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI6.setHandleClock(handleExternalClock);
      }
      MIDI6.setHandleMessage(handleMIDI_6Message);
      t_uMIDI6.setHandleNoteOn(handleNoteOn);
      t_uMIDI6.setHandleNoteOff(handleNoteOff);
      break;
    case 7:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI7.setHandleClock(handleExternalClock);
      }
      MIDI7.setHandleMessage(handleMIDI_7Message);
      t_uMIDI7.setHandleNoteOn(handleNoteOn);
      t_uMIDI7.setHandleNoteOff(handleNoteOff);
      break;
    }
  }

  // Wait 0.5 seconds before turning on USB Host.  If connected USB devices
  // use too much power, Teensy at least completes USB enumeration, which
  // makes isolating the power issue easier.
  delay(1500);
  Serial.println("dreucifer midi thing");
  delay(10);

  myusb.begin();

  uClock.init(); // Initialize the uClock system
  uClock.setPPQN(uClock.PPQN_96);
  uClock.setTempo(120);                  // Set a default tempo
  //uClock.setOnPPQN(handleOnPPQN);        // Handle the sync pulse
  uClock.setOnSync24(handleOnSync24);    // Handle the sync pulse
  uClock.start();                        // Start the clock
  uClock.setMode(uClock.EXTERNAL_CLOCK); // Set the clock to external
}

void loop()
{
  // Next read messages arriving from the 8 USB
  // devices plugged into the USB Host port
  // as well as the 8 Serial MIDI devices
  for (int pair = 0; pair < 8; pair++)
  {
    midilist[pair]->read();
    t_usbmidilist[pair]->read();
  }

  if (usbMIDI.read())
  {
    uint8_t cable = usbMIDI.getCable() % 8;

    midilist[cable]->send(
        (midi::MidiType)usbMIDI.getType(), (midi::DataByte)usbMIDI.getData1(), (midi::DataByte)usbMIDI.getData2(), (midi::Channel)usbMIDI.getChannel());
    t_usbmidilist[cable]->send(
        (midi::MidiType)usbMIDI.getType(), (midi::DataByte)usbMIDI.getData1(), (midi::DataByte)usbMIDI.getData2(), (midi::Channel)usbMIDI.getChannel());
  }
}

bool runInputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiInputFilter filter)
{
  MidiInputFilter inputFilter = filter;

  if (((inputFilter.filterChannel >> (channel - 1)) % 2))
  {
    return true;
  }

  switch (type)
  {
  case midi::PitchBend:
  case midi::NoteOff:
  case midi::NoteOn:
    return inputFilter.filterNote;
    break;
  case midi::AfterTouchPoly:
  case midi::AfterTouchChannel:
    return inputFilter.filterAftertouch;
    break;
  case midi::ControlChange:
    return inputFilter.filterCC;
    break;
  case midi::ProgramChange:
    return inputFilter.filterPC;
    break;
  case midi::SystemExclusive:
    return inputFilter.filterSysEx;
    break;
  case midi::TimeCodeQuarterFrame:
  case midi::SongPosition:
  case midi::SongSelect:
  case midi::Undefined_F4:
  case midi::Undefined_F5:
  case midi::TuneRequest:
    return inputFilter.filterSong;
    break;
  case midi::Clock:
  case midi::Tick:
    //return inputFilter.filterClock;
    return true;
    break;
  case midi::Start:
  case midi::Continue:
  case midi::Stop:
    return inputFilter.filterTransport;
  case midi::Undefined_FD:
  case midi::ActiveSensing:
  case midi::SystemReset:
    return false;
    break;
  default:
    return false;
    break;
  }
  return false;
}

bool runOutputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiOutputFilter filter)
{
  MidiOutputFilter outputFilter = filter;

  if (((outputFilter.filterChannel >> (channel - 1)) % 2))
  {
    return true;
  }

  switch (type)
  {
  case midi::PitchBend:
  case midi::NoteOff:
  case midi::NoteOn:
    return outputFilter.filterNote;
    break;
  case midi::AfterTouchPoly:
    return outputFilter.filterAftertouch;
    break;
  case midi::ControlChange:
    return outputFilter.filterCC;
    break;
  case midi::ProgramChange:
    return outputFilter.filterPC;
    break;
  case midi::SystemExclusive:
    return outputFilter.filterSysEx;
    break;
  case midi::TimeCodeQuarterFrame:
  case midi::SongPosition:
  case midi::SongSelect:
  case midi::Undefined_F4:
  case midi::Undefined_F5:
  case midi::TuneRequest:
    return outputFilter.filterSong;
    break;
  case midi::Clock:
  case midi::Tick:
    return outputFilter.filterClock;
    break;
  case midi::Start:
  case midi::Continue:
  case midi::Stop:
    return outputFilter.filterTransport;
    break;
  case midi::Undefined_FD:
  case midi::ActiveSensing:
  case midi::SystemReset:
    return false;
    break;
  default:
    return false;
    break;
  }
  return false;
}

MidiDuplex::MidiDuplex(int port) {
  pair = port;
}

void MidiDuplex::MessageCallback(const midi::Message<128> message) {
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  MidiInputFilter inputFilter = MidiInputFilters[pair];
  MidiOutputFilter outputFilter;

  if (runInputFilter(type, data1, channel, inputFilter))
  {
    return;
  }
  for (int port = 0; port < 8; port++)
  {
    printf("Message Handled");
    if((inputFilter.filterPorts >> port) % 2) {
      return;
    }
    outputFilter = MidiOutputFilters[port];
    midilist[port]->send(type, data1, data2, channel);
  }
}