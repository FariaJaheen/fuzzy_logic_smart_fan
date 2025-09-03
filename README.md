# Fuzzy-Logic Smart Fan: Temperature-Aware PWM Cooling (Arduino + Tinkercad)

## 1. Objectives
This project designs and evaluates a closed-loop thermal controller that converts temperature measurements into PWM fan speed using fuzzy logic. The aims are to:
- Implement linguistic rule–based control (COOL/WARM/HOT; DARK/BRIGHT) with centroid defuzzification on a microcontroller.
- Demonstrate multi-sensor adaptation (temperature + ambient light) to realize context-aware, acoustically conservative cooling at night.
- Practice safe power interfacing (BJT/MOSFET driver and flyback diode) and reproducible telemetry for analysis.

---

## 2. Project Overview
An Arduino Uno reads a temperature sensor (TMP36) and an LDR-based light level. The system fuzzifies these inputs into linguistic memberships, evaluates a compact rule base to infer a fan speed class (LOW/MID/HIGH), and defuzzifies to a crisp PWM duty cycle (0–255). The fan is driven via a transistor or logic-level MOSFET with a flyback diode. Telemetry (temperature, light, memberships, PWM) is streamed to the Serial Plotter for validation and tuning. The entire build is reproducible in Tinkercad Circuits and transferable to physical hardware.

---

## 3. Components
All parts are available in Tinkercad Circuits.

- Arduino Uno R3  
- TMP36 temperature sensor *(or NTC + 10 kΩ divider as an alternative)*  
- Photoresistor (LDR) + 10 kΩ resistor (voltage divider)  
- DC motor (fan surrogate in Tinkercad)  
- NPN transistor (e.g., 2N2222) **or** logic-level MOSFET  
- Flyback diode (1N4007/1N4148) across the motor  
- 1 kΩ base resistor (if BJT); 100 Ω gate series optional (if MOSFET)  
- Breadboard, jumpers  
- (Optional) LEDs for status: “Heat Alert” and “Quiet/Night Mode”

**Pin Map (recommended)**

| Signal                 | Arduino Pin | Notes                                                                 |
|------------------------|-------------|-----------------------------------------------------------------------|
| Fan PWM output         | D5          | PWM to BJT base (via 1 kΩ) or MOSFET gate; motor needs flyback diode |
| TMP36 analog output    | A0          | TMP36: 0.5 V at 0 °C; 10 mV / °C                                     |
| LDR divider output     | A1          | LDR to +5 V, 10 kΩ to GND, junction to A1                             |
| Heat Alert LED (opt.)  | D12         | Lights when HOT membership is high                                    |
| Quiet Mode LED (opt.)  | D11         | Lights when DARK membership is high                                   |

**Wiring notes (hardware)**
- Motor + to +5 V; motor − to collector (BJT) or drain (MOSFET).  
- Emitter (BJT) or source (MOSFET) to GND.  
- Diode across motor: cathode to +5 V, anode to motor −.  
- Prefer a separate 5 V rail for the fan on real hardware; always common the grounds.

---

## 4. Implementation and Working Principle
1. **Sensing.** The Arduino samples `A0` (temperature) and `A1` (light). TMP36 readings are converted to °C; LDR is normalized to [0,1].  
2. **Fuzzification.** Memberships are computed via triangular/trapezoidal functions:
   - Temperature: COOL, WARM, HOT  
   - Light: DARK, BRIGHT
3. **Inference (Mamdani).** Rules encode interpretable behavior, e.g.:  
   - R1: IF COOL → LOW  
   - R2: IF WARM AND BRIGHT → MID  
   - R3: IF WARM AND DARK → LOW (quieter at night)  
   - R4: IF HOT AND BRIGHT → HIGH  
   - R5: IF HOT AND DARK → MID (cool, but limit noise)
4. **Defuzzification.** Singleton outputs {LOW, MID, HIGH} map to PWM set-points (e.g., 60, 140, 230). A weighted average (centroid of singletons) yields the duty cycle.  
5. **Actuation and Telemetry.** The PWM is written to D5. Optional LEDs indicate heat and quiet modes. A CSV stream supports Serial Plotter visualization and lab reporting.

---

## 5. Tinkercad Setup (concise)
1. Place **Arduino Uno**, **TMP36**, **Photoresistor**, **10 kΩ resistor**, **NPN/MOSFET**, **diode**, **DC motor**.  
2. Build the **LDR divider**: LDR to +5 V, 10 kΩ to GND, junction to A1.  
3. Wire **TMP36**: +5 V, GND, Vout to A0.  
4. Wire **fan driver** and **flyback diode** as per Pin Map and notes.  
5. Start Simulation → open **Code (Text)** → paste the `.ino` (below) → open **Serial Plotter**.

---

## 6. Applications
- Context-aware cooling for embedded systems and equipment enclosures.  
- 3D printer or workstation airflow that attenuates acoustics in low light.  
- Industrial cabinets where interpretable control is preferred to PID.

---

## 7. Learning Outcomes
Students will be able to:
- Implement Mamdani fuzzy inference (fuzzify → rule evaluation → aggregation → centroid).  
- Engineer and justify membership functions and rule bases.  
- Interface sensors and power electronics safely for inductive loads.  
- Produce and interpret quantitative telemetry for controller validation.

---

## 8. Validation Plan
- **Static plateaus:** hold ~20 °C, ~26 °C, ~34 °C; record steady PWM and memberships.  
- **Day/Night test:** vary LDR; show reduced PWM under DARK at equal temperature.  
- **Thermal step:** warm the sensor gently; quantify rise time and overshoot.  
- **Rule ablation:** disable night-quiet rules (R3/R5) and compare noise vs. temperature tracking.

---

## 9. Assessment Rubric (suggested)
- Hardware correctness and safety (20%)  
- Fuzzy design quality (memberships, rules, defuzzification) (30%)  
- Empirical validation with plots and analysis (25%)  
- Code structure and clarity (15%)  
- Extensions (hysteresis/deadband, EEPROM settings, UI) (10%)

---

## 10. Repository Structure
02_fuzzy_fan_controller/
├─ README.md
└─ fuzzy_fan_controller.ino
