    /* Post a simple message to Twitter  */
    #if defined(ARDUINO) && ARDUINO > 18
    #include <SPI.h>
    #endif
    #include <Ethernet.h>
    //#include <EthernetDNS.h> // only for IDE 0022 or earlier
    #include <Twitter.h>
     
    byte mac[] = { 0xDE, 0x00, 0xBE, 0xEF, 0xFE, 0xED };
    byte ip[] = { 192, 168, 178, 99 };
    byte DNS[] = { 8, 8, 8, 8 };
    byte gateway[] = { 192, 168, 178, 1 };
    byte subnet[] = { 255, 255, 255, 0 };
     
    Twitter twitter("2517630800-iNSsSYnXDCB6L9QMcj8KnTNRn8Ly0X3dgSF1yCS");
    char msg[] = "Hier muss ein toller Text rein.";

void setup()
{
  Ethernet.begin(mac, ip, DNS ,gateway, subnet);
  Serial.begin(9600);
 
  delay(4000);
 
  Serial.println("connecting ...");
  if (twitter.post(msg)) {
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }

  
}
 
void loop()
{
}  
