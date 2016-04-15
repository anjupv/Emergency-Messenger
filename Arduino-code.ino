#include <LiquidCrystal.h>
#include <GSM.h>

// initialize LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);   // initialize the library with the numbers of the interface pins

// initialize GSM
#define PINNUMBER ""
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSM_SMS sms;
char* remoteNumber[]= {"0123456789", "0123456789"};
int remoteNumbercount=sizeof(remoteNumber)/2;

// SMS text
char txtMsg[] = "";
char location[] = ", Location : Sahrdaya College Decennial Block, Lab 2.";

// initialize the sensors
int GasPin = A0; //GAS sensor output pin to Arduino analog A0 pin
int FirePin = A1; //fire sensor output pin to Arduino analog A1 pin
int buzzerPin = 11; //Define buzzerPin

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
lcd.print("                    ");
lcd.setCursor(0, 2);
lcd.print("                    ");
lcd.setCursor(0, 3);
lcd.print("                    ");
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
Serial.println("Initializing Modules");
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
pinMode(buzzerPin, OUTPUT); //Set buzzerPin as output
beep(100); //Beep
delay(500);
lcd.print("OK");
//Serial.println("OK");
delay(1000);
lcd_clearLine(1);
lcd_clearLine(2);
lcd_clearLine(3);

// Initialize GSM module.
lcd.setCursor(0, 1);
lcd.print("GSM         : ");
Serial.println("Initialize GSM module.");
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
    Serial.println("GSM : Error");
    lcd_clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("GSM         : Error");
    delay(1000);
  }
}
Serial.println("GSM : OK");
lcd_clearLine(1);
lcd.setCursor(0, 1);
lcd.print("GSM         : OK");
delay(1000);
beep(100); //Beep

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Modules Initialized");
lcd.setCursor(0, 1);
lcd.print("   Starting the");
lcd.setCursor(0, 2);
lcd.print(" \"Fire Messenger\"");
delay(2000);

}

void loop() {

lcd.clear();
lcd.setCursor(0, 0); // (note: line 0 is the first row, since counting begins with 0):
lcd.print("Gas Value  : ");
Serial.print("Gas value : ");
lcd.print(analogRead(GasPin));
Serial.print(analogRead(GasPin));
Serial.println();

lcd.setCursor(0, 1);
lcd.print("Fire Value : ");
Serial.print("Fire value : ");
lcd.print(analogRead(FirePin));
Serial.print(analogRead(FirePin));
Serial.println();

if (analogRead(GasPin) > 150){gas_alert();}
else {
      if (gas_sms_sent) {
//        char txtMsg[75]="GAS warning lifted at \"Sahrdaya College Decennial Block, Lab 2.\"";
        sendSMS("GAS warning lifted");
        gas_sms_sent = false;
       }
      gas_alertcount = 0;
	  }
if (analogRead(FirePin) > 150){fire_alert();}
else {
      if (fire_sms_sent) {
//        char txtMsg[75]="Fire warning lifted at \"Sahrdaya College Decennial Block, Lab 2.\"";
        sendSMS("Fire warning lifted");
        fire_sms_sent = false;
       }
      fire_alertcount = 0;
	  }

delay(1000); // Print value every 1 sec.
}

void gas_alert(){
gas_alertcount++;
if (gas_alertcount > 4){
   beep(1000); 
   //Beep every 500 milliseconds
  Serial.println("\"Warning : Gas detected.\"");
  lcd_clearLine(3);
  lcd.setCursor(0, 3);
  lcd.print("   Warning : GAS   ");
  if (!gas_sms_sent){
//    char txtMsg[75]="Warning : GAS detected at \"Sahrdaya College Decennial Block, Lab 2.\"";
    sendSMS("Warning : GAS detected");
    gas_sms_sent = true;
    }
  }
}

void fire_alert(){
fire_alertcount++;
if (fire_alertcount > 4){
   beep(1000); 
   //Beep every 500 milliseconds
  //Serial.println("\"Warning : fire detected.\"");
  lcd_clearLine(3);
  lcd.setCursor(0, 3);
  lcd.print("   Warning : FIRE   ");
  if (!fire_sms_sent){
//    char txtMsg[75]="Warning : Fire detected at \"Sahrdaya College Decennial Block, Lab 2.\"";
    sendSMS("Warning : Fire detected");
    fire_sms_sent = true;
    }
  }
}

void beep(unsigned char delayms) { //creating function
analogWrite(buzzerPin, 20); //Setting pin to high
delay(delayms); //Delaying
analogWrite(buzzerPin ,0); //Setting pin to LOW
delay(delayms); //Delaying
}

void sendSMS(char txtMsg[]){
for (int i = 0; i < remoteNumbercount; i++) {
//Serial.print("Message to mobile number: ");
Serial.println(remoteNumber[i]);

// sms text
//Serial.println("SENDING");
//Serial.println();
//Serial.println("Message:");
Serial.println(txtMsg);

// check if you getting txtMsg in Serial
// if working then remove the commenting for sms.XXX
// send the message
//sms.beginSMS(remoteNumber[i]);
//sms.print(txtMsg);
//sms.print(location);
//sms.endSMS();
Serial.println("\nCOMPLETE!\n");
}
}

void lcd_clearLine(int row){
  lcd.setCursor(0, row);
  lcd.print("                    ");
  lcd.setCursor(0, row);
}
