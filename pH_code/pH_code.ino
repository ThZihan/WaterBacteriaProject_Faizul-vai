#define pHpin A7
float OffsetpH = 6.20;
float calibration_value = 21.34+OffsetpH;
int buffer_arr[100];  // Buffer size increased for more readings
int buffer_index = 0;
unsigned long avgval = 0;
unsigned long start_time = 0;
const unsigned long sampling_interval = 30;  // 30ms between readings
const unsigned long duration = 5000;         // 5 seconds duration
unsigned long last_sample_time = 0;

void setup() 
{
  Serial.begin(9600);
  Serial.println("pH Code:");
  delay(2000);  // Initial delay for stability
}

void loop() 
{
  unsigned long current_time = millis();

  // Start the 5-second sampling period
  if (buffer_index == 0) {
    start_time = current_time;
  }

  // Take a reading every 30ms without blocking
  if (current_time - last_sample_time >= sampling_interval && current_time - start_time < duration) {
    last_sample_time = current_time;
    buffer_arr[buffer_index] = analogRead(pHpin);
    buffer_index++;
  }

  // After 5 seconds, calculate the average and display the result
  if (current_time - start_time >= duration) {
    int total_readings = buffer_index;
    avgval = 0;
    
    // Sort the buffer
    for (int i = 0; i < total_readings - 1; i++) {
      for (int j = i + 1; j < total_readings; j++) {
        if (buffer_arr[i] > buffer_arr[j]) {
          int temp = buffer_arr[i];
          buffer_arr[i] = buffer_arr[j];
          buffer_arr[j] = temp;
        }
      }
    }

    // Calculate the average by discarding outliers (middle 80%)
    int valid_start = total_readings * 0.1;
    int valid_end = total_readings * 0.9;

    for (int i = valid_start; i < valid_end; i++) {
      avgval += buffer_arr[i];
    }
    avgval /= (valid_end - valid_start);

    // Convert to voltage and then pH value
    float volt = (float)avgval * 5.0 / 1024;
    float ph_act = -5.70 * volt + calibration_value;

    // Boundary the pH value between 0 and 14
    ph_act = constrain(ph_act, 0.0, 14.0);

    Serial.print("pH Value: ");
    Serial.println(ph_act);

    // Reset buffer for the next round
    buffer_index = 0;
  }
}
