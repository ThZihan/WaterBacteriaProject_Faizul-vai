#define sensor_pin A5 
int read_ADC; 
int ntu;

void setup() {
  Serial.begin(9600);
  pinMode(sensor_pin, INPUT);
  delay(2000);  // Stabilization delay
}

void loop() {
  read_ADC = analogRead(sensor_pin);

  // Clamp the ADC value between the extended cleanest and dirtiest range
  if (read_ADC > 425) read_ADC = 425;
  if (read_ADC < 290) read_ADC = 290;

  // Recalibrate NTU mapping: 450 (hypothetical cleanest) -> 0 NTU, 280 (hypothetical dirtiest) -> 300 NTU
  ntu = map(read_ADC, 425, 290, 0, 300);

  // Display the NTU value
  Serial.print("Turbidity: ");
  Serial.println(ntu);

  // Provide qualitative feedback based on NTU thresholds
  if (ntu < 20)            Serial.println("Water Very Clean");
  else if (20 <= ntu && ntu  <= 50)       Serial.println("Water Norm Clean");
  else                     Serial.println("Water Very Dirty");

  delay(200);  // Delay between readings
}
