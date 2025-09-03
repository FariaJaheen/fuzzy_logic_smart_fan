/*
  Fuzzy-Logic Smart Fan (Temperature → PWM)
  Platform: Arduino Uno (Tinkercad-ready)
  Inputs:
    - TMP36 on A0  (°C)
    - LDR divider on A1 (0..1 normalized proxy for illuminance)
  Output:
    - PWM on D5 to motor driver (BJT/MOSFET) with flyback diode
  Optional indicators:
    - D12 = HEAT ALERT (HOT)
    - D11 = QUIET MODE (DARK)
*/

const int PIN_FAN   = 5;
const int PIN_TMP   = A0;
const int PIN_LDR   = A1;
const int LED_HEAT  = 12;
const int LED_QUIET = 11;

// ---------- Sensing ----------
float readTempC_TMP36() {
  int r = analogRead(PIN_TMP);
  float v = r * (5.0f / 1023.0f);
  return (v - 0.5f) * 100.0f;  // TMP36: 10 mV/°C with 500 mV offset
}

float readLDRnorm() {
  int r = analogRead(PIN_LDR);
  // Normalize to 0..1 for fuzzy sets; add calibration if desired
  return r / 1023.0f;
}

// ---------- Membership functions ----------
float tri(float x, float a, float b, float c) {
  if (x <= a || x >= c) return 0.0f;
  if (x == b) return 1.0f;
  return (x < b) ? (x - a) / (b - a) : (c - x) / (c - b);
}

float trap(float x, float a, float b, float c, float d) {
  if (x <= a || x >= d) return 0.0f;
  if (x >= b && x <= c) return 1.0f;
  if (x > a && x < b)   return (x - a) / (b - a);
  /* x in (c, d) */     return (d - x) / (d - c);
}

// Temperature memberships (°C) — tune for your lab environment
float mu_COOL(float T) { return trap(T, -10, -10, 18, 22); } // strong below ~18, fades by ~22
float mu_WARM(float T) { return tri (T,  20,  26,  32); }    // peak near ~26
float mu_HOT (float T) { return trap(T,  28,  32,  50,  60); } // grows >~32

// Light memberships (0..1) from LDR normalization
float mu_DARK  (float L) { return trap(L, -0.1, -0.1, 0.25, 0.45); }
float mu_BRIGHT(float L) { return trap(L,  0.35, 0.55, 1.10, 1.10); }

// Output singletons for defuzzification (PWM 0..255)
const int Y_LOW  =  60;   // quiet
const int Y_MID  = 140;   // moderate
const int Y_HIGH = 230;   // strong

// Optional output smoothing (to reduce audible dithering)
int lastPWM = 0;
const int PWM_DEADBAND = 5;  // minimum change to apply

void setup() {
  pinMode(PIN_FAN,   OUTPUT);
  pinMode(LED_HEAT,  OUTPUT);
  pinMode(LED_QUIET, OUTPUT);
  analogWrite(PIN_FAN, 0);
  Serial.begin(115200);
  Serial.println("T_C,L_norm,COOL,WARM,HOT,DARK,BRIGHT,PWM");
}

void loop() {
  // 1) Sense
  float T = readTempC_TMP36();
  float L = readLDRnorm();

  // 2) Fuzzify
  float COOL = mu_COOL(T);
  float WARM = mu_WARM(T);
  float HOT  = mu_HOT(T);
  float DARK = mu_DARK(L);
  float BRGT = mu_BRIGHT(L);

  // 3) Rule base (Mamdani, min for AND)
  float r1 = COOL;               // IF COOL -> LOW
  float r2 = min(WARM, BRGT);    // IF WARM AND BRIGHT -> MID
  float r3 = min(WARM, DARK);    // IF WARM AND DARK -> LOW (quiet)
  float r4 = min(HOT,  BRGT);    // IF HOT  AND BRIGHT -> HIGH
  float r5 = min(HOT,  DARK);    // IF HOT  AND DARK  -> MID

  // 4) Defuzzify (centroid of singletons via weighted average)
  float num = r1*Y_LOW + r2*Y_MID + r3*Y_LOW + r4*Y_HIGH + r5*Y_MID;
  float den = r1 + r2 + r3 + r4 + r5 + 1e-6f;
  int pwm   = (int)constrain(num / den, 0, 255);

  // Optional deadband to prevent small oscillations
  if (abs(pwm - lastPWM) < PWM_DEADBAND) pwm = lastPWM;
  lastPWM = pwm;

  // 5) Actuate and indicators
  analogWrite(PIN_FAN, pwm);
  digitalWrite(LED_HEAT,  (HOT  > 0.5f));
  digitalWrite(LED_QUIET, (DARK > 0.5f));

  // 6) Telemetry for Serial Plotter
  Serial.print(T,1);   Serial.print(",");
  Serial.print(L,2);   Serial.print(",");
  Serial.print(COOL,2);Serial.print(",");
  Serial.print(WARM,2);Serial.print(",");
  Serial.print(HOT,2); Serial.print(",");
  Serial.print(DARK,2);Serial.print(",");
  Serial.print(BRGT,2);Serial.print(",");
  Serial.println(pwm);

  delay(150);
}