#include <TimerOne.h>  //Lib for TimerInterupts

int iLaufzeit = 0;

bool bInteruptM = false; //Zum erkennen des 1.Zykluses
bool b1Sec = false; //True alle Sekunde
bool b6Sec = false; //True alle 0,1 Minute
bool b1Min = false; //True alle Minute
bool b6Min = false; //True alle 0,1 Stunde
bool bStd  = false; //True alle Stunde
bool bTag  = false; //True 1x am Tag

bool bStunde = false;

void setup() {
  // put your setup code here, to run once:

//Set InteruptTimers
Timer1.initialize(1000000); //x Mycro Sekunden Timer
Timer1.attachInterrupt(SekundenZahler);


//Set USB interface
Serial.begin(9600);
Serial.println("Los Gehts");
}


void loop() {
 setzen();

// Hier ein paar Beispiele
if (b6Sec) //alle 0.1 Minute
  {
    Serial.print("Zeit seit Start ");
    Serial.print(iLaufzeit / 3600);
    Serial.print(":");
    Serial.print(iLaufzeit / 60);
    Serial.print(":");
    Serial.println(iLaufzeit % 60);
  }


 zurucksetzen();
}


void SekundenZahler()
{ 
  //dieser Code wird alle Secunde aufgefrufen
  //Hierbei handelt es sich um einen Interupt der den aktuellen Code unterbricht.
  //Nicht zu viel hier hinein, das könnte den anderen Code drucheinander bringen.
  //Is nicht allzu genau. Einen Wecker würde ich damit jetzt nicht machen

  iLaufzeit = iLaufzeit + 1;
  bInteruptM = true;
 
    if (iLaufzeit % 3600*24  == 0 )    
    {
      iLaufzeit = 0;
    }
}

void setzen()
{
  if (bInteruptM)
  {
  bInteruptM = false;
  b1Sec = true; //True alle Sekunde
       if (iLaufzeit % 6 == 0 )          b6Sec = 1; //True alle 0,1 Minute
  else if (iLaufzeit % 60 == 0 )         b1Min = 1; //True alle 0,1 Minute
  else if (iLaufzeit % 600 == 0 )        b6Min = 1; //True alle 0,1 Minute
  else if (iLaufzeit % 3600 == 0 )       bStd = 1; //True alle 0,1 Minute
  else if (iLaufzeit % 3600*24  == 0 )    bTag = 1; //True alle 0,1 Minute
  
  }
}
void zurucksetzen()
{
  b1Sec = false; //True alle Sekunde
  b6Sec = false; //True alle 0,1 Minute
  b1Min = false; //True alle Minute
  b6Min = false; //True alle 0,1 Stunde
  bStd  = false; //True alle Stunde
  bTag  = false; //True 1x am Tag  
}

