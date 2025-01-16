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
  MidiInputFilters[0].filterAftertouch = false;
  MidiInputFilters[0].filterCC = false;
  MidiInputFilters[0].filterChannel = 0b0000000000000000;
  MidiInputFilters[0].filterClock = false;
  MidiInputFilters[0].filterNote = false;
  MidiInputFilters[0].filterPC = false;
  MidiInputFilters[0].filterPorts = 0b00000000;
  MidiInputFilters[0].filterSong = false;
  MidiInputFilters[0].filterSysEx = false;
  MidiInputFilters[0].filterTransport = false;

  MidiInputFilters[1].filterAftertouch = false;
  MidiInputFilters[1].filterCC = false;
  MidiInputFilters[1].filterChannel = 0b0000000000000000;
  MidiInputFilters[1].filterClock = true;
  MidiInputFilters[1].filterNote = false;
  MidiInputFilters[1].filterPC = true;
  MidiInputFilters[1].filterPorts = 0b00000000;
  MidiInputFilters[1].filterSong = true;
  MidiInputFilters[1].filterSysEx = true;
  MidiInputFilters[1].filterTransport = true;
  
  MidiInputFilters[2].filterAftertouch = true;
  MidiInputFilters[2].filterCC = false;
  MidiInputFilters[2].filterChannel = 0b0000000000000000;
  MidiInputFilters[2].filterClock = true;
  MidiInputFilters[2].filterNote = true;
  MidiInputFilters[2].filterPC = false;
  MidiInputFilters[2].filterPorts = 0b00000000;
  MidiInputFilters[2].filterSong = true;
  MidiInputFilters[2].filterTransport = true;
  MidiInputFilters[2].filterSysEx = true;

  MidiInputFilters[3].filterAftertouch = true;
  MidiInputFilters[3].filterCC = true;
  MidiInputFilters[3].filterChannel = 0b0000000000000000;
  MidiInputFilters[3].filterClock = true;
  MidiInputFilters[3].filterNote = false;
  MidiInputFilters[3].filterPC = true;
  MidiInputFilters[3].filterPorts = 0b00001000;
  MidiInputFilters[3].filterSong = true;
  MidiInputFilters[3].filterSysEx = true;
  MidiInputFilters[3].filterTransport = true;

  MidiInputFilters[4].filterAftertouch = true;
  MidiInputFilters[4].filterCC = true;
  MidiInputFilters[4].filterChannel = 0b1111111111111111;
  MidiInputFilters[4].filterClock = true;
  MidiInputFilters[4].filterPC = true;
  MidiInputFilters[4].filterPorts = 0b11111111;
  MidiInputFilters[4].filterSong = true;
  MidiInputFilters[4].filterSysEx = true;
  MidiInputFilters[4].filterTransport = true;

  MidiInputFilters[5].filterAftertouch = true;
  MidiInputFilters[5].filterCC = true;
  MidiInputFilters[5].filterChannel = 0b0000000000000000;
  MidiInputFilters[5].filterClock = true;
  MidiInputFilters[5].filterNote = true;
  MidiInputFilters[5].filterPC = true;
  MidiInputFilters[5].filterPorts = 0b11111111;
  MidiInputFilters[5].filterSong = true;
  MidiInputFilters[5].filterSysEx = true;
  MidiInputFilters[5].filterTransport = true;

  MidiInputFilters[6].filterAftertouch = true;
  MidiInputFilters[6].filterCC = true;
  MidiInputFilters[6].filterChannel = 0b0000000000000000;
  MidiInputFilters[6].filterClock = true;
  MidiInputFilters[6].filterNote = true;
  MidiInputFilters[6].filterPC = true;
  MidiInputFilters[6].filterPorts = 0b11111111;
  MidiInputFilters[6].filterSong = true;
  MidiInputFilters[6].filterSysEx = true;
  MidiInputFilters[6].filterTransport = true;

  MidiInputFilters[7].filterAftertouch = true;
  MidiInputFilters[7].filterCC = true;
  MidiInputFilters[7].filterChannel = 0b0000000000000000;
  MidiInputFilters[7].filterClock = true;
  MidiInputFilters[7].filterNote = true;
  MidiInputFilters[7].filterPC = true;
  MidiInputFilters[7].filterPorts = 0b11111111;
  MidiInputFilters[7].filterSong = true;
  MidiInputFilters[7].filterSysEx = true;
  MidiInputFilters[7].filterTransport = true;

  MidiOutputFilters[0].filterAftertouch = true;
  MidiOutputFilters[0].filterCC = false;
  MidiOutputFilters[0].filterChannel = 0b1000011100000000;
  MidiOutputFilters[0].filterClock = false;
  MidiOutputFilters[0].filterNote = false;
  MidiOutputFilters[0].filterPC = false;
  MidiOutputFilters[0].filterSong = true;
  MidiOutputFilters[0].filterSysEx = true;
  MidiOutputFilters[0].filterTransport = false;

  MidiOutputFilters[1].filterAftertouch = false;
  MidiOutputFilters[1].filterCC = false;
  MidiOutputFilters[1].filterChannel = 0b0000000000000000;
  MidiOutputFilters[1].filterClock = true;
  MidiOutputFilters[1].filterNote = false;
  MidiOutputFilters[1].filterPC = true;
  MidiOutputFilters[1].filterSong = true;
  MidiOutputFilters[1].filterSysEx = true;
  MidiOutputFilters[1].filterTransport = true;

  MidiOutputFilters[2].filterAftertouch = true;
  MidiOutputFilters[2].filterCC = false;
  MidiOutputFilters[2].filterChannel = 0b1111111111111011;
  MidiOutputFilters[2].filterClock = true;
  MidiOutputFilters[2].filterNote = false;
  MidiOutputFilters[2].filterPC = true;
  MidiOutputFilters[2].filterSong = true;
  MidiOutputFilters[2].filterSysEx = true;
  MidiOutputFilters[2].filterTransport = true;
  
  MidiOutputFilters[3].filterAftertouch = true;
  MidiOutputFilters[3].filterCC = true;
  MidiOutputFilters[3].filterChannel = 0b0000000000000000;
  MidiOutputFilters[3].filterClock = false;
  MidiOutputFilters[3].filterNote = false;
  MidiOutputFilters[3].filterPC = true;
  MidiOutputFilters[3].filterSong = true;
  MidiOutputFilters[3].filterSysEx = true;
  MidiOutputFilters[3].filterTransport = true;

  MidiOutputFilters[4].filterAftertouch = true;
  MidiOutputFilters[4].filterCC = false;
  MidiOutputFilters[4].filterChannel = 0b0000000000000000;
  MidiOutputFilters[4].filterClock = false;
  MidiOutputFilters[4].filterNote = false;
  MidiOutputFilters[4].filterPC = false;
  MidiOutputFilters[4].filterSong = true;
  MidiOutputFilters[4].filterSysEx = true;
  MidiOutputFilters[4].filterTransport = true;

  MidiOutputFilters[5].filterAftertouch = true;
  MidiOutputFilters[5].filterCC = false;
  MidiOutputFilters[5].filterChannel = 0b0000000000000000;
  MidiOutputFilters[5].filterClock = false;
  MidiOutputFilters[5].filterNote = false;
  MidiOutputFilters[5].filterPC = false;
  MidiOutputFilters[5].filterSong = true;
  MidiOutputFilters[5].filterSysEx = true;
  MidiOutputFilters[5].filterTransport = false;

  MidiOutputFilters[6].filterAftertouch = true;
  MidiOutputFilters[6].filterCC = false;
  MidiOutputFilters[6].filterChannel = 0b0000000000000000;
  MidiOutputFilters[6].filterClock = false;
  MidiOutputFilters[6].filterNote = false;
  MidiOutputFilters[6].filterPC = false;
  MidiOutputFilters[6].filterSong = true;
  MidiOutputFilters[6].filterSysEx = true;
  MidiOutputFilters[6].filterTransport = false;

  MidiOutputFilters[7].filterAftertouch = true;
  MidiOutputFilters[7].filterCC = false;
  MidiOutputFilters[7].filterChannel = 0b0000000000000000;
  MidiOutputFilters[7].filterClock = false;
  MidiOutputFilters[7].filterNote = false;
  MidiOutputFilters[7].filterPC = false;
  MidiOutputFilters[7].filterSong = true;
  MidiOutputFilters[7].filterSysEx = true;
  MidiOutputFilters[7].filterTransport = false;

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
      break;
    case 1:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI1.setHandleClock(handleExternalClock);
      }
      MIDI1.setHandleMessage(handleMIDI_1Message);
      break;
    case 2:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI2.setHandleClock(handleExternalClock);
      }
      MIDI2.setHandleMessage(handleMIDI_2Message);
      break;
    case 3:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI3.setHandleClock(handleExternalClock);
      }
      MIDI3.setHandleMessage(handleMIDI_3Message);
      break;
    case 4:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI4.setHandleClock(handleExternalClock);
      }
      MIDI4.setHandleMessage(handleMIDI_4Message);
      break;
    case 5:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI5.setHandleClock(handleExternalClock);
      }
      MIDI5.setHandleMessage(handleMIDI_5Message);
      break;
    case 6:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI6.setHandleClock(handleExternalClock);
      }
      MIDI6.setHandleMessage(handleMIDI_6Message);
      break;
    case 7:
      if (!MidiInputFilters[port].filterClock)
      {
        MIDI7.setHandleClock(handleExternalClock);
      }
      MIDI7.setHandleMessage(handleMIDI_7Message);
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
  uClock.setOnPPQN(handleOnPPQN);        // Handle the sync pulse
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
    //return outputFilter.filterClock;
    return true;
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
  MidiDuplex::pair = port;
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
    if((inputFilter.filterPorts >> (port)) & 1 ) {
      continue;
    }
    if ((inputFilter.filterChannel >> (channel - 1)) & 1 )
    {
      continue;
    }

    outputFilter = MidiOutputFilters[port];
    if(runOutputFilter(type, data1, channel, outputFilter)){
      continue;
    }
    if ((outputFilter.filterChannel >> (channel - 1)) & 1 )
    {
      continue;
    }

    if (port == 2) {
      channel = 3;
    }
    midilist[port]->send(type, data1, data2, channel);
  }
}