enum eGlobalState {
  idleState,
  activatedState,
  passwordLoopState,
  alarmState
};

const int buzzer = D3;
const int ledGreen = D5;
const int ledYellow = D6;
const int ledRed = D7;
const int buttonGreen = A1;
const int buttonYellow = A2;
const int buttonRed = A3;
const int doorSensor = A0;

const int password = 0b101001;

const int timeLoop = 30;
system_tick_t timerEnd = millis();

eGlobalState globalState = idleState;

bool buttonGreenLastState = false;
bool buttonRedLastState = false;
bool buttonYellowLastState = false;
bool buttonGreenState = false;
bool buttonRedState = false;
bool buttonYellowState = false;

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

void PlayTone(int frequency);
bool SensorRead();
int PasswordCheck(int buttonLeftTrigger, int buttonRightTrigger);
void GetInput();


void loop (){
    
    int passwordState = 0;
    
    updateButtonState(buttonGreen, buttonGreenLastState, buttonGreenState);
    updateButtonState(buttonRed, buttonRedLastState, buttonRedState);
    updateButtonState(buttonYellow, buttonYellowLastState, buttonYellowState);

    switch (globalState){
        
        case idleState:
            digitalWrite(ledGreen, LOW);
            digitalWrite(ledYellow, LOW);
            digitalWrite(ledRed, LOW);
            noTone(buzzer);
            
            if (buttonGreenState){
                PlayTone(500);
                globalState = activatedState;
            }
        break;
        
        case activatedState:                               
            if (SensorRead()){
                digitalWrite(ledYellow, HIGH);
                tone(buzzer, 50);
                timerEnd = millis()+ (timeLoop*1000);
            // end of State:
                globalState = passwordLoopState;
            }
        break;
        
        case passwordLoopState:                       
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
            if (millis() > timerEnd){
                digitalWrite(ledRed, HIGH);
                noTone(buzzer);         
                globalState = alarmState;
            }
        break;
    
        case alarmState:                                    
            PlayTone(100);
            delay(100);
            PlayTone(200);
            
             if ((digitalRead(buttonRed) == HIGH) &&    
                (digitalRead(buttonYellow) == HIGH)) {
                globalState = idleState;
             }
        break;
    }
}
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
void PlayTone(int frequency) {
  tone(buzzer, frequency);
  delay(100);
  noTone(buzzer);
}

int PasswordCheck(bool buttonLeftTrigger, bool buttonRightTrigger, bool buttonReset) {
    static int counter = 0;
    static int enteredPassword = 0b000000;
    
        if (buttonRightTrigger || buttonLeftTrigger){
            enteredPassword <<= 2; 
            if (buttonLeftTrigger) {
                enteredPassword |= 0b10; 
            } else {
                enteredPassword |= 0b01; 
            }
            digitalWrite(ledYellow, LOW);
            delay(20);
            digitalWrite(ledYellow, HIGH);
            counter++;
            if (counter == 3 ) {
                if (enteredPassword == password){
                    counter = 0;
                    enteredPassword = 0b000000;
                    return 1; 
                } else {
                    counter = 0;
                    enteredPassword = 0b000000;
                    digitalWrite(ledYellow, LOW);
                    delay(300);
                    digitalWrite(ledYellow, HIGH);
                    return -1; 
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
