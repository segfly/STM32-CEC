#define ARDUINO_ARCH_STM32F1 1

#include "CEClient.h" // https://github.com/arpruss/cec

#define DEFAULT_DEVICE          CEC_LogicalDevice::CDT_PLAYBACK_DEVICE //CEC_LogicalDevice::CDT_RECORDING_DEVICE

#define CEC_PHYSICAL_ADDRESS    0x1000
#define CEC_INPUT_PIN           PB0
#define CEC_OUTPUT_PIN          -1

#define BUFFER_SIZE 16

// create a CEC client
CEClient ceclient(CEC_PHYSICAL_ADDRESS, CEC_INPUT_PIN, CEC_OUTPUT_PIN);


void setup() {
    // initialize digital pin PB1 as an output
    pinMode(PB1, OUTPUT);
    pinMode(PB12, INPUT_PULLUP);

    // ceclient.setPhysicalAddress(CEC_PHYSICAL_ADDRESS);
    // ceclient.setPromiscuous(false);
    // ceclient.setMonitorMode(false);
    // ceclient.setMonitorModeWriting(false);
    // ceclient.Initialize((CEC_LogicalDevice::CEC_DEVICE_TYPE)DEFAULT_DEVICE);

    // ceclient.begin((CEC_LogicalDevice::CEC_DEVICE_TYPE)DEFAULT_DEVICE);
    //ceclient.onReceiveCallback(receiver);
}

int source = 0x04;
int dest = 0x0f;
unsigned char buffer[BUFFER_SIZE];
int count = 0;

bool startup = true;
bool xmit = true;
bool wakeup = true;

void loop() {
  wakeup = digitalRead(PB12);

  if (startup) {
    digitalWrite (PB1, LOW); // turn the LED on by making the voltage HIGH
    // delay(100);              // wait for a second
    // digitalWrite (PB1, HIGH);    // turn the LED off by making the voltage LOW
    // delay(100);              // wait for a second
    // digitalWrite (PB1, LOW); // turn the LED on by making the voltage HIGH
    // delay(100);              // wait for a second
    // digitalWrite (PB1, HIGH);    // turn the LED off by making the voltage LOW
    // delay(1000);              // wait for a second  

    ceclient.setPhysicalAddress(CEC_PHYSICAL_ADDRESS);
    ceclient.setPromiscuous(false);
    ceclient.setMonitorMode(false);
    ceclient.setMonitorModeWriting(false);
    //ceclient.Initialize((CEC_LogicalDevice::CEC_DEVICE_TYPE)DEFAULT_DEVICE);
    ceclient.begin((CEC_LogicalDevice::CEC_DEVICE_TYPE)DEFAULT_DEVICE);

    startup = false;
  } else {
    if (xmit && ceclient.isReady()) {
      if(wakeup) {
        buffer[0] = 0x04;
        count = 1;
      } else {
        // (Broadcast) switch HDMI source to input 3: 4f:82:30:00
        // (Broadcast) switch HDMI source to input 2: 4f:82:20:00
        dest = 0x0F;
        buffer[0] = 0x82;
        buffer[1] = 0x30;
        buffer[2] = 0x00;
        count = 3;
      }

      ceclient.write(dest, buffer, count, source);
      xmit = false;
    }
  }

  ceclient.run();
  digitalWrite (PB1, !ceclient.isReady());
}