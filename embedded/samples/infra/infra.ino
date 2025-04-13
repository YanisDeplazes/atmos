#include <IRrecv.h>
#include <IRutils.h>

#define IR_RECEIVE_PIN 15 // Using GPIO15 (D15)

IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(resultToHumanReadableBasic(&results));
    irrecv.resume(); // Prepare to receive the next signal
  }
}
