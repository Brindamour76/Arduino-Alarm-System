// Implement a menu struture for alarm panel project

//chip design stolen from here:
//  https://wokwi.com/projects/339700374803841619

// Using an analog LCD display 16*2
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

int lcd_key = 0;
//int adc_key_in = 0; moved to read_LCD_buttons 
int buttonValue = 0;

//GLobal Menu Variables
enum pageType {Root_Menu, Arm_Sub_Menu, User_Sub_Menu, Inputs_Sub_Menu, Outputs_Sub_Menu, 
               Time_Sub_Menu, Comms_Sub_Menu,Page_One, Page_Two, Page_Three, Page_Four, 
               Page_Five, Page_Six, Page_Seven, Page_Eight};

int current_Menu = 0;
int sub_pos = 0;

//Global Button Vairables
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

bool btn_Down_wasdown = false;

bool RIGHT = false;
bool LEFT = false;
bool UP = false;
bool DOWN = false;
bool ENTER = false;

String Root_Menu_Names[] = {"Arm/Disarm      ", "User Codes      ", "Input Settings  ", "Output Settings ", "Time/Date       ", "Comms           "};
String Arm_Menu_Names[] = {"Disarm          ", "Arm             "};

//************READ BUTTONS***********************//
//***********************************************//
// https://forum.arduino.cc/t/hardware-debounce-analog-switches/496945/6


int read_LCD_buttons()
{
  //int buttonstate;
  //int lastbuttonstate = 1023;
  //long lastDebounceTime = 0;
  //long debounceDelay = 250;

int buttonstate = analogRead(A1); // read the value from the sensor
// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
// we add approx 50 to those values and check to see if we are close

// MY best option for a debounce at this stage
//damn analog pins
//Maybe add a 100uF capacitor across A1 and GND as well??
if (buttonstate < 1000) {
  buttonstate = analogRead(A1);
  delay(50);
} 

if (buttonstate > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
// For V1.1 us this threshold
if (buttonstate < 50) return btnUP;
if (buttonstate < 250) return btnLEFT;
if (buttonstate < 450) {
  Serial.println("DOwn");
  return btnDOWN;
}
if (buttonstate < 650) return btnRIGHT;

if (buttonstate < 850) return btnSELECT;

 
return btnNONE; // when all others fail, return this...
}

//******************LCD FUNCTIONS************************//
//*******************************************************//

void showStartupMessage() {
  lcd.setCursor(4, 0);
  lcd.print("Welcome!");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  String message = "Alarm Panel";
  for (byte i = 0; i < message.length(); i++) {
    lcd.print(message[i]);
    delay(100);
  }
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("VERSION: 0.2");
  delay(1000);
  lcd.clear();
}



//***********page_RootMenu***************//
//***********************************************//
void display_RootMenu_Item(int curr_page) {
 lcd.setCursor(0,1);
 lcd.print(Root_Menu_Names[curr_page]);

}

void page_RootMenu() {
  while (true){
  lcd.setCursor(0,0);
  lcd.print("[MAIN MENU]      ");
  switch(sub_pos){
    case 0: display_RootMenu_Item(0); break;
    case 1: display_RootMenu_Item(1); break;
    case 2: display_RootMenu_Item(2); break;
    case 3: display_RootMenu_Item(3); break;
    case 4: display_RootMenu_Item(4); break;
    case 5: display_RootMenu_Item(5); break;
  }
  
  if (read_LCD_buttons() == btnDOWN) {
    if (sub_pos == 5) {sub_pos = 0;}
    else {sub_pos++;}
  }
  
  if (read_LCD_buttons() == btnUP) {
    if (sub_pos == 0) {sub_pos = 5;}
    else {sub_pos--;}
  }

  if (read_LCD_buttons() == btnSELECT){
    switch (sub_pos) {
      case 0: current_Menu = 1; return;
      case 1: current_Menu = 2; return;
      case 2: current_Menu = 3; return;
      case 3: current_Menu = 4; return;
      case 4: current_Menu = 5; return;
      case 5: current_Menu = 6; return;
    }
  }
  
  }
}

//***********page_Arm_SubMenu***************//
//***********************************************//
void display_ArmMenu_Item(int curr_page) {
 lcd.setCursor(0,1);
 lcd.print(Arm_Menu_Names[curr_page]);

}

void page_Arm_SubMenu() {
sub_pos = 0;
while (true){
  lcd.setCursor(0,0);
  lcd.print("[ARM/DISARM]    ");
  switch(sub_pos){
    case 0: display_ArmMenu_Item(0); break;
    case 1: display_ArmMenu_Item(1); break;
    /*case 2: display_RootMenu_Item(2); break;
    case 3: display_RootMenu_Item(3); break;
    case 4: display_RootMenu_Item(4); break;
    case 5: display_RootMenu_Item(5); break;*/
  }
  
  if (read_LCD_buttons() == btnDOWN) {
    if (sub_pos == 1) {sub_pos = 0;}
    else {sub_pos = 1;}
  }
  
  if (read_LCD_buttons() == btnUP) {
    if (sub_pos == 0) {sub_pos = 1;}
    else {sub_pos = 0;}
  }

  //Exit back to Main Menu
  if (read_LCD_buttons() == btnLEFT) {
    page_RootMenu();
    return;
    }
  
  
  }
}

//***********page_User_SubMenu***************//
//***********************************************//
void page_User_SubMenu() {

}

//***********page_Inputs_SubMenu***************//
//***********************************************//
void page_Inputs_SubMenu() {

}

//***********page_Outputs_SubMenu***************//
//**********************************************//
void page_Outputs_SubMenu() {

}

//***********page_Time_SubMenu***************//
//***********************************************//
void page_Time_SubMenu() {

}

//***********page_Comms_SubMenu***************//
//***********************************************//
void page_Comms_SubMenu() {

}


//********************SETUP***********************//
//***********************************************//
void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  //showStartupMessage();
}


//***************MAIN LOOP***********************//
//***********************************************//
void loop()
{
  switch(current_Menu){
    case 0: Root_Menu: page_RootMenu(); break;
    case 1: page_Arm_SubMenu(); break;
    case 2: page_User_SubMenu(); break;
    case 3: page_Inputs_SubMenu(); break;
    case 4: page_Outputs_SubMenu(); break;
    case 5: page_Time_SubMenu(); break;
    case 6: page_Comms_SubMenu(); break;
  }
  
  
  
}
//*************************************************//






//***********MAKE BUTTONS DO STUFF***************//
//***********************************************//
int do_buttons(){
// Placeholder for buttons actions
//Not Sure I still need this
lcd_key = read_LCD_buttons(); // read the buttons
  
  switch (lcd_key)
  { 
    case btnRIGHT:
    {
      //lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Right");
      break;
    }
    case btnLEFT:
    {
      //lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Left");
      break;
    }
    case btnUP:
    {
      //lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Up");
      break;
    }
    case btnDOWN:
    {
      //lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Down");
      break;
    }
    case btnSELECT:
    { 
      //lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Select");
      break;
    }
    case btnNONE:
    {
      //lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Waiting...");
      break;
    }
  }

}
