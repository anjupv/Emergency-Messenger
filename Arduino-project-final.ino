#include <LiquidCrystal.h>
#include <GSM.h>

// initialize LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);   // initialize the library with the numbers of the interface pins

// initialize GSM
#define PINNUMBER ""
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSM_SMS sms;
char* remoteNumber[]= {"0987654321"};
int remoteNumbercount=sizeof(remoteNumber)/2;

// SMS text
char txtMsg[] = "";
char location[] = " Location : Sahrdaya College Decennial Block";

// initialize the sensors
#define GasPin A0 //GAS sensor output pin to Arduino analog A0 pin
#define FirePin A1 //fire sensor output pin to Arduino analog A1 pin
#define buzzerPin 11 //Define buzzerPin

int gas_threshold = 10;
int fire_threshold = 200;

int gas_alertcount = 0;
int fire_alertcount = 0;
boolean gas_sms_sent = false;
boolean fire_sms_sent = false;

void setup()
{
Serial.begin(9600); //Initialize serial port - 9600 bps
lcd.begin(20, 4);   // set up the LCD's number of columns and rows:
Serial.println("LCD started with 20x4");

//Logo start here.

Serial.print("Printing logo ...");

lcd.setCursor(0, 0);
lcd.print("   Fire Messenger   ");
lcd.setCursor(0, 1);
lcd.print("   Anju Varghese");
lcd.setCursor(0, 2);
lcd.print("    Christy Paul");
lcd.setCursor(0, 3);
lcd.print("    Akhila N V");
delay(1000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("                    ");
lcd.setCursor(0, 1);
lcd.print("                    ");
lcd.setCursor(0, 2);
lcd.print("                    ");
lcd.setCursor(0, 3);
lcd.print("                    ");
delay(1000);

Serial.println("Done");
//Logo end here.


// Initialize modules.
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Initializing Modules");
//Serial.println("Initializing Modules");
delay(500);

//GAS
lcd.setCursor(0, 1);
lcd.print("GAS Sensor  : ");
//Serial.print("GAS Sensor  : ");
delay(500);
lcd.print("OK");
//Serial.println("OK");
delay(500);

//Fire
lcd.setCursor(0, 2);
lcd.print("Fire Sensor : ");
//Serial.print("Fire Sensor : ");
delay(500);
lcd.print("OK");
//Serial.println("OK");
delay(500);

//Buzzer
lcd.setCursor(0, 3);
lcd.print("Buzzer      : ");
//Serial.print("Buzzer      : ");
//pinMode(buzzerPin, OUTPUT); //Set buzzerPin as output
analogWrite(buzzerPin, 100); //Beep
delay(200);
analogWrite(buzzerPin, 0); //Beep
lcd.print("OK");
//Serial.println("OK");
delay(1000);
lcd_clearLine(1);
lcd_clearLine(2);
lcd_clearLine(3);

// Initialize GSM module.
lcd.setCursor(0, 1);
lcd.print("GSM         : ");
Serial.println("Initialize GSM");
delay(500);
boolean notConnected = true;   // connection state
// Start GSM shield
// If your SIM has PIN, pass it as a parameter of begin() in quotes
while(notConnected)
{
  if(gsmAccess.begin(PINNUMBER)==GSM_READY)
    notConnected = false;
  else
  {
//    Serial.println("GSM : Error");
    lcd_clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("GSM         : Error");
    delay(500);
  }
}
Serial.println("GSM : OK");
lcd_clearLine(1);
lcd.setCursor(0, 1);
lcd.print("GSM         : OK");
delay(1000);
//beep(100); //Beep

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Modules Initialized");
lcd.setCursor(0, 1);
lcd.print("   Starting the");
lcd.setCursor(0, 2);
lcd.print(" \"Fire Messenger\"");
analogWrite(buzzerPin, 100); //Beep
delay(300);
analogWrite(buzzerPin, 0); //Beep
delay(200);
analogWrite(buzzerPin, 100); //Beep
delay(300);
analogWrite(buzzerPin, 0); //Beep
delay(1100);

}

void loop() {

unsigned int gas_value;
unsigned int fire_value;

for(int x = 0 ; x < 100 ; x++){delay(1);gas_value = gas_value + analogRead(GasPin);}
Serial.println("1");
Serial.println(gas_value);
gas_value = gas_value/100;
Serial.println("2");
Serial.println(gas_value);
gas_value = map(gas_value, 0, 1023, 0, 100);
Serial.println("3");  
Serial.println(gas_value);
for(int x = 0 ; x < 100 ; x++){delay(1);fire_value = fire_value + analogRead(FirePin);}
Serial.println("");
Serial.println("Fire re : ");
Serial.println(fire_value);
fire_value = fire_value/100;
Serial.println("Fire /100 : ");
Serial.println(fire_value);

lcd.clear();
lcd.setCursor(0, 0); // (note: line 0 is the first row, since counting begins with 0):
lcd.print("Gas level     : ");
lcd.print(gas_value);
lcd.print("%");
Serial.print("Gas:");
Serial.print(gas_value);
Serial.println("%");

lcd.setCursor(0, 1);
lcd.print("Fire detected : ");
Serial.print("Fire:");
if (fire_value < fire_threshold){
  lcd.print("Yes");
  Serial.println("Yes");
  fire_alert();
}
else {
  lcd.print("No");
  Serial.println("No");
  if (fire_sms_sent) {sendSMS("Fire warning lifted");fire_sms_sent = false;analogWrite(buzzerPin ,0);}
  fire_alertcount = 0;
}

if (gas_value > gas_threshold){gas_alert();}
else {
      if (gas_sms_sent) {sendSMS("GAS warning lifted");gas_sms_sent = false;analogWrite(buzzerPin ,0);}
      gas_alertcount = 0;
}

delay(5000); // Print value every 1 sec.
}

void gas_alert(){
gas_alertcount++;
if (gas_alertcount > 4){
//   beep(1000);
  analogWrite(buzzerPin, 100); //Beep
  Serial.println("\"Warning : Gas detected.\"");
  lcd_clearLine(3);
  lcd.setCursor(0, 3);
  lcd.print("   Warning : GAS   ");
  if (!gas_sms_sent){
    sendSMS("Warning : GAS detected,");
    gas_sms_sent = true;
    }
  }
}

void fire_alert(){
fire_alertcount++;
if (fire_alertcount > 4){
//   beep(1000); 
  analogWrite(buzzerPin, 100); //Beep
   //Beep every 500 milliseconds
  Serial.println("\"Warning : fire detected.\"");
  lcd_clearLine(3);
  lcd.setCursor(0, 3);
  lcd.print("   Warning : FIRE   ");
  if (!fire_sms_sent){
    sendSMS("Warning : Fire detected");
    fire_sms_sent = true;
    }
  }
}

void sendSMS(char txtMsg[]){
for (int i = 0; i < remoteNumbercount; i++) {
//Serial.print("Message to mobile number: ");
Serial.println(remoteNumber[i]);

// sms text
//Serial.println("SENDING");
//Serial.println();
//Serial.println("Message:");
Serial.print(txtMsg);
Serial.println(location);

// check if you getting txtMsg in Serial
// if working then remove the commenting for sms.XXX
// send the message
sms.beginSMS(remoteNumber[i]);
sms.print(txtMsg);
sms.print(location);
sms.endSMS();
Serial.println("\nCOMPLETE!\n");
}
}

void lcd_clearLine(int row){
  lcd.setCursor(0, row);
  lcd.print("                    ");
  lcd.setCursor(0, row);
}
