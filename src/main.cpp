/* This creates 8 Serial midi pairs, hosts 8 USB MIDI devices as a host,
   and creates 8 Virtual MIDI cables to the computer over USB.

   Some basic MIDI routing and filtering is implemented.

   I have also implemented a high resolution uClock core.
 */
#include "midimanager.h"

MidiFilterData midiFilterData;

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

MidiInputPort MidiInputFilters[8];
MidiOutputPort MidiOutputFilters[8];


void doHandleMessageMIDI_0(const midi::Message<128> &message)
{
  MidiInputPort port = MidiInputFilters[0];
  port.mergeMap = 0b11111110;
  port.filterNote = true;
  port.filterAftertouch = true;
  port.filterTransport = true;
  port.filterPC = true;
  

  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  if (runInputFilter(type, data1, channel, MIDI_0))
  { for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_0)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } } }

void doHandleMessageMIDI_1(const midi::Message<128> &message)
{
  MidiInputPort port = MidiInputFilters[1];
  port.filterClock = true;

  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  if (runInputFilter(type, data1, channel, MIDI_1))
  { for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_1)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } } }

void doHandleMessageMIDI_2(const midi::Message<128> &message)
{
  MidiInputPort port = MidiInputFilters[2];
  port.filterNote = true;
  port.filterClock = true;

  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  if (runInputFilter(type, data1, channel, MIDI_2))
  { for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_2)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } } }

void doHandleMessageMIDI_3(const midi::Message<128> &message)
{
  MidiInputPort port = MidiInputFilters[3];
  port.filterNote = true;
  port.filterClock = true;

  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  if (runInputFilter(type, data1, channel, MIDI_3))
  { for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_3)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } } }

void doHandleMessageMIDI_4(const midi::Message<128> &message)
{
  MidiInputPort port = MidiInputFilters[4];
  port.filterNote = true;
  port.filterClock = true;

  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  if (runInputFilter(type, data1, channel, MIDI_4))
  { for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_4)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } } }

void doHandleMessageMIDI_5(const midi::Message<128> &message)
{
  MidiInputPort port = MidiInputFilters[5];
  port.filterClock = true;

  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  if (runInputFilter(type, data1, channel, MIDI_5))
  { for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_5)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } } }

void doHandleMessageMIDI_6(const midi::Message<128> &message)
{
  MidiInputPort port = MidiInputFilters[6];
  port.filterNote = true;
  port.filterClock = true;

  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  if (runInputFilter(type, data1, channel, MIDI_6))
  { for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_6)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } } }

void doHandleMessageMIDI_7(const midi::Message<128> &message)
{
  MidiInputPort port = MidiInputFilters[7];
  port.filterNote = true;
  port.filterClock = true;

  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_7)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } }

void doHandleMessageUSBMIDI_0(const midi::Message<128> &message)
{
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_0)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } }

void doHandleMessageUSBMIDI_1(const midi::Message<128> &message)
{
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_1)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } }

void doHandleMessageUSBMIDI_2(const midi::Message<128> &message)
{
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_2)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } }

void doHandleMessageUSBMIDI_3(const midi::Message<128> &message)
{
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_3)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } }

void doHandleMessageUSBMIDI_4(const midi::Message<128> &message)
{
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_4)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } }

void doHandleMessageUSBMIDI_5(const midi::Message<128> &message)
{
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_5)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } }

void doHandleMessageUSBMIDI_6(const midi::Message<128> &message)
{
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_6)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } }

void doHandleMessageUSBMIDI_7(const midi::Message<128> &message)
{
  midi::MidiType type = message.type;
  midi::DataByte data1 = message.data1;
  midi::DataByte data2 = message.data2;
  midi::Channel channel = message.channel;

  if (runInputFilter(type, data1, channel, MIDI_7))
  { for (int pair = 0; pair < 8; pair++)
    { if ((MidiInputFilters[0].mergeMap >> pair) % 2)
      { if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
        { if (runOutputFilter(type, data1, channel, MIDI_7)) {
            midilist[pair]->send(type, data1, data2, channel); }
        } } } } }

void handleNoteOn(midi::Channel channel, midi::DataByte data1, midi::DataByte data2)
{
  midi::Channel _channel = channel;
  midi::DataByte _data1 = data1;
  midi::DataByte _data2 = data2;
  usbMIDI.sendNoteOn(_data1, _data2, _channel, 0);
  for (int pair = 0; pair < 8; pair++)
  {
    midilist[pair]->sendNoteOn(_data1, _data2, _channel);
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
    midilist[pair]->sendNoteOff(_data1, _data2, _channel);
  }
};

void handleExternalClock()
{
  uClock.clockMe();
};

// Send clock sync to the external midi ports and the USB interface
void handleOnPPQN(uint32_t tick)
{
  for (int pair = 0; pair < 8; pair++)
  {
    byte outputFilter = midiFilterData.midiOutputFilters[pair];
    if ((outputFilter >> 1) % 2)
    {
      midilist[pair]->sendRealTime(midi::Tick);
    }
  }
}

// Send clock sync to the external midi ports and the USB interface
void handleOnSync24(uint32_t tick)
{
  usbMIDI.sendRealTime(usbMIDI.Clock);
  for (int pair = 0; pair < 8; pair++)
  {
    byte outputFilter = midiFilterData.midiOutputFilters[pair];
    if ((outputFilter >> 1) % 2)
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
    byte outputFilter = midiFilterData.midiOutputFilters[pair];
    if ((outputFilter >> 2) % 2)
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
    byte outputFilter = midiFilterData.midiOutputFilters[pair];
    if ((outputFilter >> 2) % 2)
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
    byte outputFilter = midiFilterData.midiOutputFilters[pair];
    if ((outputFilter >> 2) % 2)
    {
      midilist[pair]->sendRealTime(midi::Continue);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  // Manually set the output filters. Replace with read_or_initialize from persistent storage.
  MidiOutputFilters[0].filterClock = true;
  MidiOutputFilters[0].filterPC = true;
  MidiOutputFilters[1].filterClock = true;
  MidiOutputFilters[1].filterTransport = true;
  MidiOutputFilters[4].filterClock = true;
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
      MIDI0.setHandleMessage(doHandleMessageMIDI_0);
      MIDI0.setHandleClock(handleExternalClock);
      MIDI0.setHandleStart(handleStart);
      MIDI0.setHandleStop(handleStop);
      MIDI0.setHandleContinue(handleContinue);
      t_uMIDI0.setHandleNoteOn(handleNoteOn);
      t_uMIDI0.setHandleNoteOff(handleNoteOff);
      break;
    case 1:
      MIDI1.setHandleMessage(doHandleMessageMIDI_1);
      t_uMIDI1.setHandleNoteOn(handleNoteOn);
      t_uMIDI1.setHandleNoteOff(handleNoteOff);
      break;
    case 2:
      MIDI2.setHandleMessage(doHandleMessageMIDI_2);
      t_uMIDI2.setHandleNoteOn(handleNoteOn);
      t_uMIDI2.setHandleNoteOff(handleNoteOff);
      break;
    case 3:
      MIDI3.setHandleMessage(doHandleMessageMIDI_3);
      t_uMIDI3.setHandleNoteOn(handleNoteOn);
      t_uMIDI3.setHandleNoteOff(handleNoteOff);
      break;
    case 4:
      MIDI4.setHandleMessage(doHandleMessageMIDI_4);
      t_uMIDI4.setHandleNoteOn(handleNoteOn);
      t_uMIDI4.setHandleNoteOff(handleNoteOff);
      break;
    case 5:
      MIDI5.setHandleMessage(doHandleMessageMIDI_5);
      MIDI5.setHandleStart(handleStart);
      MIDI5.setHandleStop(handleStop);
      MIDI5.setHandleContinue(handleContinue);
      t_uMIDI5.setHandleNoteOn(handleNoteOn);
      t_uMIDI5.setHandleNoteOff(handleNoteOff);
      break;
    case 6:
      MIDI6.setHandleMessage(doHandleMessageMIDI_6);
      t_uMIDI6.setHandleNoteOn(handleNoteOn);
      t_uMIDI6.setHandleNoteOff(handleNoteOff);
      break;
    case 7:
      MIDI7.setHandleMessage(doHandleMessageMIDI_7);
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

    if (t_usbmidilist[pair]->read())
    {
      midi::Message<128> newMessage;
      newMessage.type = (midi::MidiType)t_usbmidilist[pair]->getType();
      newMessage.data1 = t_usbmidilist[pair]->getData1();
      newMessage.data2 = t_usbmidilist[pair]->getData2();
      newMessage.channel = t_usbmidilist[pair]->getChannel();
      newMessage.valid = true;

      switch (pair)
      {
      case 0:
        doHandleMessageUSBMIDI_0(newMessage);
        break;
      case 1:
        doHandleMessageUSBMIDI_1(newMessage);
        break;
      case 2:
        doHandleMessageUSBMIDI_2(newMessage);
        break;
      case 3:
        doHandleMessageUSBMIDI_3(newMessage);
        break;
      case 4:
        doHandleMessageUSBMIDI_4(newMessage);
        break;
      case 5:
        doHandleMessageUSBMIDI_5(newMessage);
        break;
      case 6:
        doHandleMessageUSBMIDI_6(newMessage);
        break;
      case 7:
        doHandleMessageUSBMIDI_7(newMessage);
        break;
      default:
        break;
      }
    }
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

bool runInputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiPair pair)
{
  if (((MidiInputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
  {
    switch (type)
    {
    case midi::PitchBend:
    case midi::NoteOff:
    case midi::NoteOn:
      return MidiInputFilters[0].filterNote;
      break;
    case midi::AfterTouchPoly:
      return MidiInputFilters[0].filterAftertouch;
      break;
    case midi::ControlChange:
      return MidiInputFilters[0].filterCC;
      break;
    case midi::ProgramChange:
      return MidiInputFilters[0].filterPC;
      break;
    case midi::SystemExclusive:
      return MidiInputFilters[0].filterSysEx;
      break;
    case midi::TimeCodeQuarterFrame:
    case midi::SongPosition:
    case midi::SongSelect:
    case midi::Undefined_F4:
    case midi::Undefined_F5:
    case midi::TuneRequest:
      return MidiInputFilters[0].filterSong;
      break;
    case midi::Clock:
    case midi::Tick:
      return MidiInputFilters[0].filterClock;
    case midi::Start:
    case midi::Continue:
    case midi::Stop:
      return MidiInputFilters[0].filterTransport;
    case midi::Undefined_FD:
    case midi::ActiveSensing:
    case midi::SystemReset:
    default:
      return false;
      break;
    }
    return false;
  }
  return false;
}

bool runOutputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiPair pair)
{
  if (((MidiOutputFilters[pair].filterChannel >> (channel - 1)) % 2) == 0)
  {
    switch (type)
    {
    case midi::PitchBend:
    case midi::NoteOff:
    case midi::NoteOn:
      return MidiOutputFilters[pair].filterNote;
      break;
    case midi::AfterTouchPoly:
      return MidiOutputFilters[pair].filterAftertouch;
      break;
    case midi::ControlChange:
      return MidiOutputFilters[pair].filterCC;
      break;
    case midi::ProgramChange:
      return MidiOutputFilters[pair].filterPC;
      break;
    case midi::SystemExclusive:
      return MidiOutputFilters[pair].filterSysEx;
      break;
    case midi::TimeCodeQuarterFrame:
    case midi::SongPosition:
    case midi::SongSelect:
    case midi::Undefined_F4:
    case midi::Undefined_F5:
    case midi::TuneRequest:
      return MidiOutputFilters[pair].filterSong;
      break;
    case midi::Clock:
    case midi::Tick:
      return MidiOutputFilters[pair].filterClock;
    case midi::Start:
    case midi::Continue:
    case midi::Stop:
      return MidiOutputFilters[pair].filterTransport;
    case midi::Undefined_FD:
    case midi::ActiveSensing:
    case midi::SystemReset:
    default:
      return false;
      break;
    }
    return false;
  }
  return false;
}