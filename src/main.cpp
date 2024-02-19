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

CREATE_DO_HANDLE_MESSAGE_MIDI(MIDI0, MIDI_0);
CREATE_DO_HANDLE_MESSAGE_MIDI(MIDI1, MIDI_1);
CREATE_DO_HANDLE_MESSAGE_MIDI(MIDI2, MIDI_2);
CREATE_DO_HANDLE_MESSAGE_MIDI(MIDI3, MIDI_3);
CREATE_DO_HANDLE_MESSAGE_MIDI(MIDI4, MIDI_4);
CREATE_DO_HANDLE_MESSAGE_MIDI(MIDI5, MIDI_5);
CREATE_DO_HANDLE_MESSAGE_MIDI(MIDI6, MIDI_6);
CREATE_DO_HANDLE_MESSAGE_MIDI(MIDI7, MIDI_7);

CREATE_DO_HANDLE_MESSAGE_USB(MIDI0, 0);
CREATE_DO_HANDLE_MESSAGE_USB(MIDI1, 1);
CREATE_DO_HANDLE_MESSAGE_USB(MIDI2, 2);
CREATE_DO_HANDLE_MESSAGE_USB(MIDI3, 3);
CREATE_DO_HANDLE_MESSAGE_USB(MIDI4, 4);
CREATE_DO_HANDLE_MESSAGE_USB(MIDI5, 5);
CREATE_DO_HANDLE_MESSAGE_USB(MIDI6, 6);
CREATE_DO_HANDLE_MESSAGE_USB(MIDI7, 7);

void handleExternalClock()
{
  uClock.clockMe();
}

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

  // Run through each midi port, begin, turn off thru, and set the message callback.
  // Set the first midi port to the external clock master
  for (int port = 0; port < 8; port++)
  {
    midilist[port]->begin(MIDI_CHANNEL_OMNI);
    midilist[port]->turnThruOff();
    switch (port)
    {
    case 0:
      MIDI0.setHandleMessage(doHandleMessageMIDI0);
      MIDI0.setHandleClock(handleExternalClock);
      MIDI0.setHandleStart(handleStart);
      MIDI0.setHandleStop(handleStop);
      MIDI0.setHandleContinue(handleContinue);
      break;
    case 1:
      MIDI1.setHandleMessage(doHandleMessageMIDI1);
      break;
    case 2:
      MIDI2.setHandleMessage(doHandleMessageMIDI2);
      break;
    case 3:
      MIDI3.setHandleMessage(doHandleMessageMIDI3);
      break;
    case 4:
      MIDI4.setHandleMessage(doHandleMessageMIDI4);
      break;
    case 5:
      MIDI5.setHandleMessage(doHandleMessageMIDI5);
      MIDI5.setHandleStart(handleStart);
      MIDI5.setHandleStop(handleStop);
      MIDI5.setHandleContinue(handleContinue);
      break;
    case 6:
      MIDI6.setHandleMessage(doHandleMessageMIDI6);
      break;
    case 7:
      MIDI7.setHandleMessage(doHandleMessageMIDI7);
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
        doHandleMessageMIDI0_TUSB(newMessage);
        break;
      case 1:
        doHandleMessageMIDI1_TUSB(newMessage);
        break;
      case 2:
        doHandleMessageMIDI2_TUSB(newMessage);
        break;
      case 3:
        doHandleMessageMIDI3_TUSB(newMessage);
        break;
      case 4:
        doHandleMessageMIDI4_TUSB(newMessage);
        break;
      case 5:
        doHandleMessageMIDI5_TUSB(newMessage);
        break;
      case 6:
        doHandleMessageMIDI6_TUSB(newMessage);
        break;
      case 7:
        doHandleMessageMIDI7_TUSB(newMessage);
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
  byte inputFilter = midiFilterData.midiInputFilters[pair];

  switch (type)
  {
  case midi::AfterTouchChannel:
  case midi::AfterTouchPoly:
  case midi::NoteOff:
  case midi::NoteOn:
  case midi::PitchBend:
    return ((inputFilter >> NOTES) % 2) ? true : false;
    break;
  case midi::ControlChange:
    return ((inputFilter >> PCHANGE) % 2) || (((inputFilter >> NOTES) % 2) && (data1 == midi::ModulationWheel))
               ? true
               : false;
    break;
  case midi::ProgramChange:
    return ((inputFilter >> PCHANGE) % 2) ? true : false;
    break;
  default:
    return false;
    break;
  }

  return false;
}

bool runOutputFilter(midi::MidiType type, midi::DataByte data1, midi::Channel channel, MidiPair pair)
{
  byte outputFilter = midiFilterData.midiOutputFilters[pair];
  uint16_t outputChannels = midiFilterData.midiOutputChannels[pair];

  if (((outputChannels >> (channel - 1)) % 2) == 0)
  {
    return false;
  }

  switch (type)
  {
  case midi::AfterTouchChannel:
  case midi::AfterTouchPoly:
  case midi::NoteOff:
  case midi::NoteOn:
  case midi::PitchBend:
    return ((outputFilter >> NOTES) % 2) ? true : false;
    break;
  case midi::ControlChange:
    return ((outputFilter >> PCHANGE) % 2) || (((outputFilter >> NOTES) % 2) && (data1 == midi::ModulationWheel))
               ? true
               : false;
    break;
  case midi::ProgramChange:
    return ((outputFilter >> PCHANGE) % 2) ? true : false;
    break;
  default:
    return false;
    break;
  }

  return false;
}