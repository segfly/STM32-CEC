#define ARDUINO_ARCH_STM32F1 1

#include "CEClient.h" // https://github.com/arpruss/cec

#define DEFAULT_DEVICE          CEC_LogicalDevice::CDT_PLAYBACK_DEVICE //CEC_LogicalDevice::CDT_RECORDING_DEVICE

// Note sure what the physical address does. May be used in receiving, which I have not done much with
#define CEC_PHYSICAL_ADDRESS    0x1000

// PINs to use for CEC, when using a single tri-state pin, output is set to -1, which in the modified
// client will just use the same pin for both.
#define CEC_INPUT_PIN           PB0
#define CEC_OUTPUT_PIN          -1

// I think I read somewhere that the max CEC frame is 16 bytes? This may be wrong
#define BUFFER_SIZE 16

// create a CEC client
CEClient ceclient(CEC_PHYSICAL_ADDRESS, CEC_INPUT_PIN, CEC_OUTPUT_PIN);

void setup() {
  // Initialize PB1 for LED indication
  pinMode(PB1, OUTPUT);

  // Initialize PB1 for mode switching (for testing different frames)
  pinMode(PB12, INPUT_PULLUP);

  // Initialize the CEC client
  ceclient.setPhysicalAddress(CEC_PHYSICAL_ADDRESS);
  ceclient.setPromiscuous(false);
  ceclient.setMonitorMode(false);
  ceclient.setMonitorModeWriting(false);

  // Depending on mode, this sends a PING on the CEC line and ensures the client's address is unique
  // After a few loops over ceclient.run(), the client internal state will be ready
  ceclient.begin((CEC_LogicalDevice::CEC_DEVICE_TYPE)DEFAULT_DEVICE);
}

// In CEC frame, source is the upper byte and dest is lower byte
// The cec client will bit shift source and logical-and with dest to create the src/dest byte in the frame
// These can be changed when writing, these are just defaults.
int source = 0x04;
int dest = 0x0f; //0x0f is broadcast

unsigned char buffer[BUFFER_SIZE];
int count = 0; // number of bytes to write in frame

bool xmit = true;
bool wakeup = true;

unsigned long previousMillis = 0;
int ledState = LOW;

// Basic LED blink to visually tell if something locked-up in the main loop
void ledUpdate(uint32_t ledPin, uint32_t interval) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

void loop() {
  // run() needs to be executed every loop, as often as possible, because internally it is tracking
  // complex timing on the CEC line
  ceclient.run();

  // It is important to wait for isReady() to become true before tryign to send frames
  // This exposes it externally for logic probe purposes.
  //digitalWrite(PB1, !ceclient.isReady());
  // OR
  // Blink an LED for status
  ledUpdate(PB1, 1000);

  // This is a physical mode switch, to aid in debugging.
  wakeup = digitalRead(PB12);

  // For debugging CEC, we only want to transmit once, and only when the client is ready
  if (xmit && ceclient.isReady()) {
    if(wakeup) {
      // with broadcast dest, this will wake the TV
      buffer[0] = 0x04;
      count = 1;
    } else {
      // (Broadcast) switch HDMI source to input 3: 4f:82:30:00
      // (Broadcast) switch HDMI source to input 2: 4f:82:20:00
      dest = 0x0F; // dest set differently here as I was experimenting with targeting devices
      buffer[0] = 0x82;
      buffer[1] = 0x30;
      buffer[2] = 0x00;
      count = 3;
    }

    // This doesn't actually write the bytes, run() does the work
    ceclient.write(dest, buffer, count, source);
    xmit = false;
  }
}