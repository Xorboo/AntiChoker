bool sgpInitialized = false;
bool ledsInitialized = false;


void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  sgpInitialized = setupSgp();
  ledsInitialized = setupLeds();
}

int counter = 0;
void loop() {
  int32_t airQuality = getAirQuality();
  renderLeds();
  delay(1000);
}