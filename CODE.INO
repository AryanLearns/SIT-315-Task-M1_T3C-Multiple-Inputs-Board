const int pir_sensor_interrupt = 2;  
const int led_pin_pir = 6;            
volatile int motion_detected = 0;    

const int photodiodePin = A0;        // Photodiode sensor connected to analog pin A0
const int ledPin_button = 13;        
const int buttonPin = 3;             

volatile bool led_state = false;
volatile bool high_intensity = false;

void setup() {
  Serial.begin(9600);
  Serial.println("This task is for multiple interrupt function");

  // PIR setup
  pinMode(led_pin_pir, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pir_sensor_interrupt), motionInterrupt, HIGH);

  // button setup
  pinMode(ledPin_button, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);
}

void loop() {
  if (high_intensity && led_state) {
    Serial.println("Light intensity is high, Button LED is OFF");
    digitalWrite(ledPin_button, LOW);
    led_state = false;
  }

  if (motion_detected) {
    Serial.println("Motion is detected!");
    delay(100);  
    digitalWrite(led_pin_pir, LOW);   
    motion_detected = 0;             
  }
}

void motionInterrupt() {
  int motion = digitalRead(pir_sensor_interrupt);

  Serial.print("PIR Sensor Value: ");
  Serial.println(motion);

  if (motion == HIGH) {
    digitalWrite(led_pin_pir, HIGH);  
    motion_detected = 1;              
  } else {
    // Optional: Serial.println("Motion not detected");
  }
}

void buttonInterrupt() {
  led_state = !led_state;

  high_intensity = (analogRead(photodiodePin) > 512); // Adjust the threshold as needed

  if (!high_intensity && led_state) {
    // Turn on the button LED if light intensity is low
    digitalWrite(ledPin_button, HIGH);
    Serial.println("Light intensity is low, Button LED is ON");
  } else {
    Serial.println("Light intensity is high, Button LED is OFF");
    digitalWrite(ledPin_button, LOW);
    led_state = false;
  }
}
