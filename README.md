# Fuzzy-Logic Smart Fan: Temperature-Aware PWM Cooling 

## Objectives
A closed-loop thermal controller is designed and evaluated in this project, in which temperature measurements are converted into PWM fan speed using fuzzy logic. The aims are to:
- Implementing linguistic rule–based control (COOL/WARM/HOT; DARK/BRIGHT) with centroid defuzzification on a microcontroller.
- Demonstrating multi-sensor adaptation (temperature + ambient light) to realize context-aware, acoustically conservative cooling at night.
- Practicing safe power interfacing (BJT/MOSFET driver and flyback diode) and reproducible telemetry for analysis.

Explore, run, and modify this project directly in **Tinkercad Circuits**:
👉 [Open the Smart Fan in Tinkercad](https://www.tinkercad.com/things/7coI43mhA0o-fuzzylogicsmartfan)  

---

## Project Overview
An Arduino Uno reads a temperature sensor (TMP36) and an LDR-based light level. The system fuzzifies these inputs into linguistic memberships, evaluates a compact rule base to infer a fan speed class (LOW/MID/HIGH), and defuzzifies to a crisp PWM duty cycle (0–255). The fan is driven via a transistor or logic-level MOSFET with a flyback diode. Telemetry (temperature, light, memberships, PWM) is streamed to the Serial Plotter for validation and tuning. The entire build is reproducible in Tinkercad Circuits and transferable to physical hardware.

---

## Components
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
| Warm Mode LED (opt.)  | D10         | Lights when WARM membership is dominant                                 |
| Quiet Mode LED (opt.)  | D11         | Lights when DARK membership is high                                   |

**Wiring notes (hardware)**
- Motor + to +5 V; motor − to collector (BJT) or drain (MOSFET).  
- Emitter (BJT) or source (MOSFET) to GND.  
- Diode across motor: cathode to +5 V, anode to motor −.  
- Prefer a separate 5 V rail for the fan on real hardware; always common the grounds.

---

## Working Principle
The fuzzy-logic smart fan operates through a structured sequence of sensing, fuzzification, inference, defuzzification, actuation, and visual feedback. The Arduino Uno continuously samples signals from the TMP36 temperature sensor (A0) and the LDR-based divider (A1). The TMP36 output is translated into degrees Celsius, while the LDR reading is normalized to the 0–1 range to capture relative illuminance. During fuzzification, temperature readings are classified into three linguistic sets—COOL, WARM, and HOT—and light levels are represented by DARK and BRIGHT. Membership degrees are derived through triangular and trapezoidal functions, ensuring smooth transitions across environmental conditions. The inference stage uses Mamdani reasoning with five interpretable rules. For instance, COOL conditions activate a LOW fan speed, WARM conditions trigger either LOW or MID speeds depending on light intensity, and HOT conditions command HIGH or MID speeds depending on whether the environment is bright or dark. These rules embed practical considerations such as quieter operation at night. In defuzzification, the fuzzy results are aggregated and converted into a crisp control value through a centroid-based weighted average of singleton set-points (LOW = 60, MID = 140, HIGH = 230). A PWM deadband filter suppresses minor oscillations, thereby reducing fan noise from rapid adjustments. For actuation, the computed PWM signal is applied to pin D5 to drive the fan through a transistor or MOSFET. State indication is provided by three LEDs: a WARM LED on D10 illuminates when WARM is the dominant state, a HEAT ALERT LED on D12 indicates HOT conditions, and a QUIET MODE LED on D11 signals dark environments. This winner-takes-all indicator scheme ensures unambiguous visual feedback. Finally, the system streams CSV telemetry comprising temperature, light intensity, membership degrees, and PWM duty cycle to the Serial Plotter. This supports real-time visualization, facilitates parameter tuning, and allows students to analyze fuzzy control behavior in laboratory experiments.

---

## Tinkercad Setup
1. Place **Arduino Uno**, **TMP36**, **Photoresistor**, **10 kΩ resistor**, **NPN/MOSFET**, **diode**, **DC motor**.  
2. Build the **LDR divider**: LDR to +5 V, 10 kΩ to GND, junction to A1.  
3. Wire **TMP36**: +5 V, GND, Vout to A0.  
4. Wire **fan driver** and **flyback diode** as per Pin Map and notes.  
5. Start Simulation → open **Code (Text)** → paste the `.ino` (below) → open **Serial Plotter**.

---

## Applications
- Context-aware cooling for embedded systems and equipment enclosures.  
- 3D printer or workstation airflow that attenuates acoustics in low light.  
- Industrial cabinets where interpretable control is preferred to PID.

---

## Learning Outcomes
Students will be able to:
- Implement Mamdani fuzzy inference (fuzzify → rule evaluation → aggregation → centroid).  
- Engineer and justify membership functions and rule bases.  
- Interface sensors and power electronics safely for inductive loads.  
- Produce and interpret quantitative telemetry for controller validation.

---

## Validation Plan
- **Static plateaus:** hold ~20 °C, ~26 °C, ~34 °C; record steady PWM and memberships.  
- **Day/Night test:** vary LDR; show reduced PWM under DARK at equal temperature.  
- **Thermal step:** warm the sensor gently; quantify rise time and overshoot.  
- **Rule ablation:** disable night-quiet rules (R3/R5) and compare noise vs. temperature tracking.

---


## Repository Structure
fuzzy_logic_smart_fan/
├─ README.md
└─ fuzzy_fan_controller.ino
