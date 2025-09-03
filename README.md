# Fuzzy-Logic Smart Fan: Temperature-Aware PWM Cooling (Arduino + Tinkercad)

## 1. Objectives
A closed-loop thermal controller is designed and evaluated in this project, in which temperature measurements are converted into PWM fan speed using fuzzy logic. The aims are to:
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

**Pin Map**

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

## 4.and Working Principle
The implementation of the fuzzy-logic smart fan system integrates sensing, fuzzification, inference, defuzzification, and actuation into a unified control workflow. The Arduino Uno first performs sensing by sampling the analog signals from the TMP36 temperature sensor (A0) and the LDR-based voltage divider (A1). The TMP36 output is converted into degrees Celsius, while the LDR voltage is normalized to a [0,1] range to represent relative light intensity. In the fuzzification stage, these crisp sensor values are mapped into linguistic variables through triangular and trapezoidal membership functions, defining three sets for temperature—COOL, WARM, and HOT—and two sets for light intensity—DARK and BRIGHT. The inference mechanism, based on Mamdani reasoning, evaluates a set of interpretable rules that encode context-aware fan behavior. For example, when the temperature is COOL, the fan is set to a LOW speed (R1), whereas WARM conditions lead to a MID speed in bright environments (R2) but only a LOW speed under dark conditions (R3), prioritizing quieter operation at night. Similarly, HOT conditions drive the fan to a HIGH speed when accompanied by bright light (R4), but only to a MID speed when dark (R5), thereby balancing cooling demand with noise considerations. During defuzzification, the aggregated fuzzy outputs are converted into a crisp control signal by applying a centroid calculation over singleton PWM set-points (LOW = 60, MID = 140, HIGH = 230). This process yields a duty cycle value between 0 and 255. Finally, in the actuation and telemetry phase, the resulting PWM signal is written to pin D5 to drive the fan through a transistor or MOSFET, while optional LEDs provide quick feedback for heat-alert and quiet-night modes. In addition, the Arduino streams sensor values, membership degrees, and PWM outputs in CSV format, enabling visualization through the Serial Plotter and facilitating lab-based analysis and reporting.

---

## 5. Tinkercad Setup
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
