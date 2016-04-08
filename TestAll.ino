#include <Stepper.h> //Lib for stepper Motor
#include <TimerOne.h>  //Lib for TimerInterupts
#include <IRremote.h>  //Liv for IR Empf
#include <LiquidCrystal.h>

 
//LCD Display use
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);  //VSS Masse VDD=5+ V0=Poti RS=0 R/W=Masse E=1 D4=2 D5=3 D6=4 D7=5 A=5+ K=Masse




//Infarot Sensor
int RECV_PIN = 6; //Pin nummer 6 Für IR
IRrecv irrecv(RECV_PIN);
decode_results results; //Dieser Befehl sorgt dafür, dass die Daten, die per Infrarot eingelesen werden unter „results“ abgespeichert werden.

// Motor Control
int SPMU = 2048;  //Number of Steps per Round (max 2048)
Stepper myStepper(SPMU, 42,44,43,45); //8 = IN4;  9 = IN3; 10 = IN2; 11 = IN1 
int akPosition = 0; //inPerzent
int MaxSteps = 2048; // maximale anzahl der Schritte
int MyTarget = 0;

//Single LED setup Heartbeat
int LED=13; //Pin for LED
int helligkeit= 0; //startvalue
int fadeschritte= 1; //stepwith



void setup()

{
//Stepper Setup
  myStepper.setSpeed(100); //rpm

//LED setup
pinMode(LED, OUTPUT);

//LCD 16 Spalten, 2 Zeilen
lcd.begin(16, 2);

//Set USB interface
Serial.begin(9600);
Serial.println("Los Gehts");

//Set InteruptTimers
Timer1.initialize(6*1000); //x ms Timer
Timer1.attachInterrupt(SetMyLED);


irrecv.enableIRIn(); //Init IR Sensor

}

void loop()
{


if (irrecv.decode(&results)) 
{
switch (results.value) 
{
case 16724175: // 1
MyTarget = 10;  
break;
case 16718055: // 2
MyTarget = 20;
break;
case 16743045: // 3
MyTarget = 30;
break;
case 16716015: // 4
MyTarget = 40;
break;
case 16726215: // 5
MyTarget = 50;
break;
case 16734885: // 6
MyTarget = 60;
break;
case 16728765: // 7
MyTarget = 70;
break;
case 16730805: // 8
MyTarget = 80;
break;
case 16732845: // 9
MyTarget = 90;
break;
case 16738455: //0
MyTarget = 0;
break;
case 16720605: //I<<
MyTarget = 0;
break;
case 16750695: //100+
MaxSteps = MaxSteps + SPMU;
break;
case 16756815: //200+
MaxSteps = MaxSteps - SPMU;
break;
case 16748655: //EQ
MaxSteps =  SPMU;
break;
case 16754775: //+
akPosition = MaxSteps - SPMU / 5;
MyTarget = 100;
break;
case 16769055: //-
akPosition = SPMU / 5;
MyTarget = 0;
break;
case 16720610: //I<<
MyTarget = 0;
break;
case 16712445: //>>I
MyTarget = 100;
break;

}

//MoveMotor(MyTarget, 10);

Serial.println(results.value, DEC);
Serial.println(MyTarget);
irrecv.resume();
}  

MoveMotor(MyTarget, 10);


delay(10);
//lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Soll: ");
lcd.print(MyTarget);
lcd.print("%       ");
lcd.setCursor(0, 1);
lcd.print("Ist: ");
lcd.print((float) akPosition / (float) MaxSteps * 100.0 );
lcd.print("%       ");

TestMotor();
}



void TestMotor()
{// Runs the Motor between 0 - 100%
  //Serial.println(akPosition);
  if (akPosition <= 0 )
    {
      MyTarget = 100;
    }
  else if (akPosition >= MaxSteps)
  {
    MyTarget = 0;
  }
  MoveMotor(MyTarget, 10);  


}


void SetMyLED()
{
  analogWrite(LED, helligkeit); //PWM 
  helligkeit = helligkeit + fadeschritte; //change Pulsewith
  if (helligkeit == 0 || helligkeit == 255) //if helligkeit = 0 or 255 than change direction
    {
      fadeschritte = -fadeschritte ;
    }

}

//int akPosition = 0 //inSteps
//int MaxSteps = 2048 // maximale anzahl der Schritte


void MoveMotor(int PrzTarget,int MotorStepWith)
{
int StepTarget = 0;
StepTarget = (float) MaxSteps *  ((float)PrzTarget / 100.0);

//Serial.println(PrzTarget);

if (PrzTarget<=100)
{
if (StepTarget > akPosition + MotorStepWith)
  {
    myStepper.step(MotorStepWith);
    akPosition = akPosition + MotorStepWith;
  }
  else if (StepTarget < akPosition - MotorStepWith)
  {
    myStepper.step(MotorStepWith * -1);
    akPosition = akPosition - MotorStepWith; 
  }
  else 
  {
    myStepper.step(StepTarget - akPosition);
    akPosition = StepTarget;
  }
  //delay(100);
}

else
{
  
}

} 
