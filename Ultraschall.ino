int trigger=2;
int echo=3;
long dauer=0;
long entfernung=0;
int helligkeit=0;
int lampe=13;
int Ton=12;

void setup()

{

Serial.begin (9600);
pinMode(trigger, OUTPUT);
pinMode(Ton, OUTPUT);
pinMode(lampe, OUTPUT);
pinMode(echo, INPUT);

}

 

void loop()

{

digitalWrite(trigger, LOW);

delay(5);

digitalWrite(trigger, HIGH);

delay(10);

digitalWrite(trigger, LOW);

dauer = pulseIn(echo, HIGH);


entfernung = dauer; //(dauer/2) / 29.1;
helligkeit = entfernung/10 % 255;


analogWrite(lampe, helligkeit);
analogWrite(Ton, helligkeit);
//Serial.println(entfernung);



//delay(1000);

}
