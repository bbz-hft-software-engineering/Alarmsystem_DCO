
//         Define Loopstate:

enum eGlobalState {
  idleState,
  activatedState,
  passwordLoopState,
  alarmState
};

//          Global Variables


// Definition of the Pin
const int buzzer = D3;
const int ledGreen = D5;
const int ledYellow = D6;
const int ledRed = D7;
const int buttonGreen = A1;
const int buttonYellow = A2;
const int buttonRed = A3;
const int doorSensor = A0;

// Definition of the Password
const int password = 0b101001;

// Definition of Time
const int timeLoop = 30; // in second
system_tick_t timerEnd = millis();

// Set State of enum
eGlobalState globalState = idleState;

// global variable
bool buttonGreenLastState = false;
bool buttonRedLastState = false;
bool buttonYellowLastState = false;
bool buttonGreenState = false;
bool buttonRedState = false;
bool buttonYellowState = false;

//          Setup()


void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buttonGreen, INPUT_PULLDOWN);
  pinMode(buttonRed, INPUT_PULLDOWN);
  pinMode(buttonYellow, INPUT_PULLDOWN);
  pinMode(doorSensor, INPUT_PULLUP);
  globalState = idleState;
}

//          Function decleration


void PlayTone(int frequency);
bool SensorRead();
int PasswordCheck(int buttonLeftTrigger, int buttonRightTrigger);
void GetInput();

//          Loop()
//          Basic Sequence like int main()

void loop (){
    
    int passwordState = 0;
    
    updateButtonState(buttonGreen, buttonGreenLastState, buttonGreenState);
    updateButtonState(buttonRed, buttonRedLastState, buttonRedState);
    updateButtonState(buttonYellow, buttonYellowLastState, buttonYellowState);

    switch (globalState){
        
        case idleState:
            // begin of State:                              // In this State
            digitalWrite(ledGreen, LOW);
            digitalWrite(ledYellow, LOW);
            digitalWrite(ledRed, LOW);
            noTone(buzzer);
            
            // end of State
            if (buttonGreenState){
                PlayTone(500);
                globalState = activatedState;
            }
        break;
        
        case activatedState:                                // In this State
            // begin of State:
            if (SensorRead()){
                digitalWrite(ledYellow, HIGH);
                tone(buzzer, 50);
                timerEnd = millis()+ (timeLoop*1000);
            // end of State:
                globalState = passwordLoopState;
            }
        break;
        
        case passwordLoopState:                             // In this State
            // begin of State
            passwordState = PasswordCheck(buttonRedState, buttonYellowState, buttonGreenState);
            if (passwordState == 1){
                noTone(buzzer);
                digitalWrite(ledYellow, LOW);
                digitalWrite(ledGreen, HIGH);
                PlayTone(100);
                delay(100);
                PlayTone(200);
                
                globalState = idleState;
            }
                // end of State
            if (millis() > timerEnd){
                digitalWrite(ledRed, HIGH);
                noTone(buzzer);         
                globalState = alarmState;
            }
        break;
    
        case alarmState:                                    // In this State
            // begin of State
            PlayTone(100);
            delay(100);
            PlayTone(200);
            
            // end of State
             if ((digitalRead(buttonRed) == HIGH) &&      // Option with reset the Microcontroller if alarmState is active
                (digitalRead(buttonYellow) == HIGH)) {
                globalState = idleState;
             }
        break;
    }
}


//          Function

//          SensorRead()
bool SensorRead(){
    bool SensorActivated = false;
    if (analogRead(doorSensor) > 500) {
        delay(20);
        SensorActivated = true;
        PlayTone(100); //
        delay(100);      //
        PlayTone(200);  //
    }
    return SensorActivated;
}

//          playTone(int speaker, int frequency)

void PlayTone(int frequency) {
  tone(buzzer, frequency);
  delay(100);
  noTone(buzzer);
}

//          int PasswordCheck(int buttonLeftTrigger, int buttonRightTrigger)
//          Checks if the Button pressed in correct order (Password correct)  

int PasswordCheck(bool buttonLeftTrigger, bool buttonRightTrigger, bool buttonReset) {
    static int counter = 0;
    static int enteredPassword = 0b000000;
    
        if (buttonRightTrigger || buttonLeftTrigger){
            enteredPassword <<= 2; // Shift password to make space for new entry
            if (buttonLeftTrigger) {
                enteredPassword |= 0b10; // Add combination for left button
            } else {
                enteredPassword |= 0b01; // Add combination for right button
            }
            digitalWrite(ledYellow, LOW);
            delay(20);
            digitalWrite(ledYellow, HIGH);
            counter++;
            if (counter == 3 ) {
                if (enteredPassword == password){
                    counter = 0;
                    enteredPassword = 0b000000;
                    return 1; // Password is correct
                } else {
                    counter = 0;
                    enteredPassword = 0b000000;
                    digitalWrite(ledYellow, LOW);
                    delay(300);
                    digitalWrite(ledYellow, HIGH);
                    return -1; // Wrong password
                }   
            }
        }
     return 0;
}


void updateButtonState(int buttonPin, bool &lastState, bool &buttonState)
{
    bool currentButtonState = !digitalRead(buttonPin);
    
    if(currentButtonState != lastState){
        buttonState = currentButtonState;
    }
    else{  
        buttonState = false;
    }
    lastState = currentButtonState;
}
