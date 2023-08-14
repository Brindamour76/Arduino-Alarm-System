#define VERSION 0
#define SUBVERSION 1

//---Hardware Variables---//
const int sirenLed = 4;
const int armedLed = 13;
const int armBut = 3;
const int pirSensor = 2;

bool isArmed = false; //Armed state
bool isAlarmed = false; //Alarmed State
bool sirenActive = false; //is the siren active

int userInput;
int inputStatus;
int inputActive = 0;
int exitDelay = 5;
int entryDelay = 5;

//---Siren Variables---//
unsigned long sirenStart; //Siren Start time
unsigned long sirenRun;   //how long has the siren been running
unsigned long sirenMax = 6000;   //max siren time, 6 sconds for testing
int sirenCount = 0;

//---Armed Functions---//
void armedInterface(){
  //check for inputs
  inputActive = checkInputs();
  //trigger alarm state
  if (inputActive == true){
    alarmTriggered();
  }

  //activate sirens 
  if (sirenActive == true){
    //activate siren
    sirenTimed();
  }

  //check for disarming
  codeInput;  
}

void armSeq(){
  //need to code in an Exit delay
  isArmed = true;
  digitalWrite(armedLed, HIGH);
  Serial.println("Armed");
}

void sirenTimed(){
  //trigger siren for set period
  //do not want to re-trigger siren more than once
  sirenRun = millis();
  if (sirenRun - sirenStart < sirenMax && sirenCount < 1){
    sirenActive = true;
    digitalWrite(sirenLed, HIGH);
    Serial.println("SIREN");
  } else {
    sirenActive = false;
    digitalWrite(sirenLed, LOW);
    sirenCount = sirenCount +1;
    Serial.println(sirenCount);
  }
  
}

bool checkInputs(){
  //check input status and return true or false
  inputActive = digitalRead(pirSensor);
  //Serial.println("inputActive");
  if (inputActive == 1){
    return true;
  } else {
    return false;
  }

}

void alarmTriggered(){
  isAlarmed = true;
  sirenActive = true;
  //find a better place to put sirenstart
  sirenStart = millis();
}


//---Disarmed Functions---//
void disarmedInterface(){
  //check for arming
}

void disarmSeq(){
  isArmed = false;
  digitalWrite(armedLed, LOW);
  isAlarmed = false;
  sirenActive = false;
  Serial.println("DisArmed");
  sirenCount = 2;
  sirenTimed();
  sirenCount = 0;
  //sirenTimed();
  digitalWrite(sirenLed, LOW);
}


//---User Input---//
void codeInput() {
  //using a basic switch for now
  
  userInput = digitalRead(armBut);
  if (userInput == LOW && isArmed == false){
    armSeq();
    Serial.println("Button");
  } else if (userInput == LOW && isArmed == true){
    disarmSeq();
  }
}

//*******************SETUP**********************//
void setup() {
  // put your setup code here, to run once:
  pinMode(sirenLed, OUTPUT);
  pinMode(armedLed, OUTPUT);
  pinMode(armBut, INPUT_PULLUP);
  pinMode(pirSensor, INPUT);
  
  Serial.begin(9600);
    
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //---Test for Arming---//
  codeInput();
  

  //---Test for Armed State---//
  if (isArmed == true){
    //Call the Armed Interface
    armedInterface();

  } else if (isArmed == true){
    //Call the Disarmed Interface
    disarmedInterface();

  }


  //---Test for Arming---//
  codeInput();
  
  //---Check Inputs---//
  //Kept for now, but inputs should only be checked when armed.
  /*if (checkInputs() == true && isArmed == true){
    Serial.println("Test");
  }*/
  
}
