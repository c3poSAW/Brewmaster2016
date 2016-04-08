    /* Post a simple message to Twitter  */
    #include <Time.h> // Zeitfunktionen
    #include <Ethernet.h> // Ethernetfunktionen
    #include <EthernetUdp.h> // UDP Funktionen auf Ethernet
    #include <SPI.h> // SPI für Ethernetboard

     
    byte mac[] = { 0xDE, 0x00, 0xBE, 0xEF, 0xFE, 0xED };
    byte ip[] = { 192, 168, 178, 99 };
    byte DNS[] = { 8, 8, 8, 8 };
    byte gateway[] = { 192, 168, 178, 1 };
    byte subnet[] = { 255, 255, 255, 0 };

     static IPAddress timeServer(132, 163, 4, 101);
     const int NTP_PACKET_SIZE = 48; // NTP Paketgroeße
     byte packetBuffer[NTP_PACKET_SIZE]; // NTP Datnpuffer
     time_t prevDisplay = 0; // NTP Klasse - Zeit wurde noch nicht geändert
     const int ntpsync = 7200; // *** NTP Sync alle 2 Stunden

      EthernetClient client; // EthernetClient - Instanz erstellen
      EthernetUDP Udp; // UDP - Instanz erstellen


    
void setup()
{
  Ethernet.begin(mac);//, ip, DNS ,gateway, subnet);
  Serial.begin(9600);
  delay(4000);
  Udp.begin(8888);   // UDP starten Port 8888
  setSyncProvider(getNtpTime); // NTP Provider starten / erstmalig abfragen
  setSyncInterval(ntpsync); // NTP Sync alle 2 Stunden
  setTime(hour(),minute(),second(),day(),month(),year()); // Zeit auf Arduino setzen
  
}
 
void loop()
{
  if (timeStatus() != timeNotSet) // Anzeige nur aktualisieren wenn Zeit geändert
  {
    if (now() != prevDisplay)
    {
      prevDisplay = now();
      ClockDisplay();
    }
  
}  
}

void ClockDisplay(){
  Serial.print("Zeit / Datum: ");
  printDigits(hour());
  Serial.print(":");
  printDigits(minute());
  Serial.print(":");
  printDigits(second());
  Serial.print("  ");
  printDigits(day());
  Serial.print(".");
  printDigits(month());
  Serial.print(".");
  Serial.println(year());
//  Serial.println(ipbuf);
}
// 0 vorsetzen wenn übergebener Wert < 10
void printDigits(int digits)
{
  if(digits < 10) Serial.print('0'); // vorher 0 wenn einstellig
  Serial.print(digits);
}
// NTP Provider - prüft ob NTP läuft und holt die Daten
time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // vorhergehende Pakete verwerfen
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) // 1500 Millisekunden Wartezeit für NTP Antwort
  {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE)
    {
      Udp.read(packetBuffer, NTP_PACKET_SIZE); // Paket in Puffer einlesen
      unsigned long secsSince1900;
      // Byte 40,41,42,43 zusammen in long umwandeln
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + 1 * SECS_PER_HOUR;
    }
  }
  return 0;
}
// NTP Request senden
void sendNTPpacket(IPAddress &address)
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE); // Puffer auf 0 setzen
  // notwendige Werte für NTP-Request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum oder Uhrentyp
  packetBuffer[2] = 6;     // Pollingintervall
  packetBuffer[3] = 0xEC;  // Peer Clock Präzision
  /* 8 bytes 0 für Root Delay & Root Dispersion
   Es wird also immer eine Antwort entgegengenommen,
   egal welche Abweichung der Server vom Root hat */
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // Werte für Request gesetzt, Request senden auf Port 123
  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

