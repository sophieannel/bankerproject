#include <Watchdog.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Stepper.h>


int switchState= 0; //valeurs
int switchStateo= 0; //blanc
int switchStater= 0; //reset
float ro, go, bo; //blanc
float r, g, b; //valeurs
double Total=0; // total valeur


// Define number of steps per rotation:
const int stepsPerRevolution = 1500;

// Create stepper object called 'myStepper', note the pin order:
Stepper myStepper = Stepper(stepsPerRevolution, 10, 8, 9, 7);

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display


void setup() {
  // interrupteur
pinMode (13, INPUT_PULLUP); //Bouton blanc
pinMode(12, INPUT_PULLUP); //Bouton noir
pinMode(11, INPUT_PULLUP); //reset
pinMode (6,OUTPUT); //out active le capteur


//Écran
 lcd.init();
  //lcd.clear();         
 lcd.backlight();      // Make sure backlight is on
    // Print a message on both lines of the LCD.

//moteur
  // Set the speed to 5 rpm:
  myStepper.setSpeed(15);
 


switchStateo=digitalRead(13); //blanc
switchState=digitalRead(12);  //noir
switchStater=digitalRead(11); //reset
Serial.begin(9600);

//Capteur (si détecté= go on; sinon= erreur)
  if (tcs.begin()) {
    Serial.println("Capteur détecté");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Capteur detecte");  
    delay(500);
    Serial.println("Procedez au blanc");
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Effectuer le"); 
    lcd.setCursor(5,1);
    lcd.print("blanc");  
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("*erreur* Capteur");  
    while (1);
  }  
}

  
void loop() {
  
      switchStateo=digitalRead(13); //blanc
      switchState=digitalRead(12); //valeurs
      switchStater=digitalRead(11);//reset

//code pour reset
    if (switchStater==LOW) {
       lcd.clear();
       lcd.setCursor(4,0);
       lcd.print("reset"); 
       delay(500);
       resetFunc();
       }
      
//code pour le blanc
    // si bouton blanc enfoncé = capteur prend des données pour le blanc
          if (switchStateo==LOW){
            digitalWrite(6, HIGH);
            delay(1000);
            tcs.getRGB(&ro, &go, &bo);
            Serial.println("Blanc effectué");
             lcd.clear();
             lcd.setCursor(1,0);
             lcd.print("Blanc effectue"); 
             delay(2000);
             digitalWrite(6, LOW);
            Serial.println("Insérez votre billet");
             lcd.clear();
             lcd.setCursor(1,0);
             lcd.print("Inserez votre"); 
             lcd.setCursor(4,1);
             lcd.print("billet"); 
            }

  
// obtenir valeurs
    //si le blanc n'est pas encore effectué et le bouton noir est enfoncé = erreur)
          if (ro==0 & go==0 & bo==0 & switchState==LOW){
            Serial.println("Ne pas oublier le blanc"); 
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("Ne pas oublier"); 
            lcd.setCursor(3,1);
            lcd.print("le blanc");
            }
  //si le blanc est fait et le bouton noir est enfoncé = lecture de la couleur)
          else if (switchState==LOW){
  
        //moteur tourne
            for (int i = 0; i <= 1; i++) {
            myStepper.step(stepsPerRevolution);
            }
           
      
        //obtention des données
          digitalWrite(6, HIGH);
          delay(500);
          tcs.getRGB(&r, &g, &b);
          Serial.print("R1: "); Serial.print(r); Serial.print(" ");
          Serial.print("G1: "); Serial.print(g); Serial.print(" ");
          Serial.print("B1: "); Serial.print(b); Serial.println(" ");
          Serial.print("Ro: "); Serial.print(ro); Serial.print(" ");
          Serial.print("Go: "); Serial.print(go); Serial.print(" ");
          Serial.print("Bo: "); Serial.print(bo); Serial.println(" ");
          Serial.print("R: "); Serial.print((ro-r)/ro); Serial.print(" ");
          Serial.print("G: "); Serial.print((go-g)/go); Serial.print(" ");
          Serial.print("B: "); Serial.print((bo-b)/bo); Serial.println(" ");
          Serial.println(" ");
          digitalWrite(6, LOW);
  

        //valeur billet
        if (0.13<((ro-r)/ro) & ((ro-r)/ro)<0.28 & (-0.05)<((go-g)/go) & ((go-g)/go)<0.09 & (-0.20)<((bo-b)/bo) & ((bo-b)/bo)<(-0.07)){
          Serial.println ("5$");
          lcd.clear();
          lcd.setCursor(7,0);
          lcd.print("5$"); 
          int cinq = 5;
          Total=Total + cinq;
          }
          
        else if ((-0.1)<((ro-r)/ro) & ((ro-r)/ro)<0.13 & 0<((go-g)/go) & ((go-g)/go)<0.15 & (-0.15)<((bo-b)/bo) & ((bo-b)/bo)<0.1){
          Serial.println ("10$");  
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("10$"); 
          int dix = 10;
          Total=Total + dix; 
          }
          else if ((-0.1)<((ro-r)/ro) & ((ro-r)/ro)<(0.05) & (-0.1)<((go-g)/go) & ((go-g)/go)<(0.05) & (0)<((bo-b)/bo) & ((bo-b)/bo)<0.15){
          Serial.println ("20$");  
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("20$");   
          int vingt = 20;
          Total=Total + vingt; 
          }
          else if ((-0.35)<((ro-r)/ro) & ((ro-r)/ro)<(-0.18) & (0.02)<((go-g)/go) & ((go-g)/go)<(0.09) & (0.02)<((bo-b)/bo) & ((bo-b)/bo)<0.08){
          Serial.println ("50$");  
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("50$");   
          int cinquante = 50;
          Total=Total + cinquante;
          }
          else if ((-0.21)<((ro-r)/ro) & ((ro-r)/ro)<(-0.09) & (-0.2)<((go-g)/go) & ((go-g)/go)<(0.1) & (-0.13)<((bo-b)/bo) & ((bo-b)/bo)<0.12){
          Serial.println ("100$"); 
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("100$"); 
          int cent = 100;
          Total=Total + cent; 
          } 
          else {
          Serial.println ("*ERREUR*");
          Serial.println ("Aucun billet détecté");
          Serial.println ("*ERREUR*");
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("*ERREUR*");
          }

          //TOTAL
          delay(1500);
          Serial.print("Total:");
          Serial.println(Total);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Total:"); 
          lcd.setCursor(0,1);
          lcd.print(Total);
          lcd.print("$");
         
        

        //moteur tourne
          for (int i = 0; i <= 3; i++) {
            myStepper.step(stepsPerRevolution);
            }
          Serial.println(" ");
          Serial.println("Insérez votre billet");
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Inserez votre"); 
          lcd.setCursor(4,1);
          lcd.print("billet"); 
  }
}

// fonction pour le reset
void resetFunc() {
    // call the watchdog timer to force a reset
    wdt_enable(WDTO_15MS);  
    while(1) {}}
