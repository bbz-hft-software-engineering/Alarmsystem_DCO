# Alarmsystem_DCO

**Use Case: Home Security Alarm System**

**Description:** This Use Case describes the behavior of a Home Security Alarm System built on the Particle Photon Inventor's Kit hardware. The system is designed to trigger an alarm in the event of unauthorized entry into a home and can be controlled through various sensors and actuators.

**Actors:**
1. **Activation Button**: Pressing this button activates the alarm system.
2. **Speaker**: Produces sounds to indicate the alarm state and key combinations.
3. **LEDs**: Three LEDs (green, yellow, red) serve as visual indicators for the alarm state and key combination status.

**Sensors:**
1. **Door Opening Sensor**: Detects the opening of the door and triggers the alarm.

**Use Case Scenarios:**

1. **Activate Alarm System:**
   - *Actors:* User
   - *Trigger:* The user presses the activation button to activate the alarm system.
   - *Flow:*
     1. The alarm system is activated.
     2. A short tone is played to confirm the activation.

2. **Detect Unauthorized Entry:**
   - *Actors:* Door Opening Sensor, Alarm System
   - *Trigger:* The door opening sensor detects the opening of the door after the alarm system has been activated.
   - *Flow:*
     1. The alarm system triggers the alarm.
     2. A yellow LED lights up.
     3. A tone is played to indicate the unauthorized entry.
     4. The alarm lasts for 30 seconds.

3. **Enter Key Combination:**
   - *Actors:* User, Buttons ("left", "right")
   - *Trigger:* The user wants to deactivate the alarm within 30 seconds.
   - *Flow:*
     1. The user presses the left button twice and the right button once to enter the correct key combination.
     2. If the correct key combination is entered within 30 seconds:
        - A green LED lights up.
        - Two short tones are played to confirm successful deactivation.
        - The alarm is deactivated.
     3. If the correct key combination is not entered within 30 seconds:
        - A red indicator light lights up.
        - The alarm remains activated.

4. **Re-activate Alarm System:**
   - *Actors:* User
   - *Trigger:* The alarm system has been successfully deactivated, and the user wants to re-activate it.
   - *Flow:*
     1. The user presses the activation button to re-activate the alarm system.
     2. The alarm system is now active and ready to detect any unauthorized entry.

**End of Use Case**
