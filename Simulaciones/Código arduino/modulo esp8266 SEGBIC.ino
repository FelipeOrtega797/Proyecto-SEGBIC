#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define WIFI_SSID "Alba"
//"Redmi 9C"
//"INFINITUMECA4_2.4"
#define WIFI_PASSWORD "alba3662"
//"123456789"
//"v68AHWq6yV"
#define BOT_TOKEN "5404446720:AAFzaBNBKoiXjphguIGFbYvY-FW7oOGhqaw"
#define chat_id "5204635211"
const unsigned long BOT_MTBS = 1000; // mean time between scan messages
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

const int ledPin = LED_BUILTIN;
int ledStatus = 0;
const int pinservo = 5;  // PIN D1 DEL MODULO WIFI
const int movimSensor = 14; //PIN D5 DEL MODULO WIFI
int x;

bool motionDetected = false; 

ICACHE_RAM_ATTR void detectamovimiento(){
    motionDetected = true;
    
  }

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)  
  {
 //   String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/Bloqueo")
    {
      digitalWrite(ledPin, LOW); // turn the LED on (HIGH is the voltage level)
    digitalWrite(pinservo, HIGH); //
      ledStatus = 1;
      bot.sendMessage(chat_id, "Bloqueo activo", "");
      motionDetected = false;
    }

    if (text == "/Desbloqueo")
    {
      ledStatus = 0;
      digitalWrite(ledPin, HIGH); // turn the LED off (LOW is the voltage level)
        digitalWrite(pinservo, LOW); //
      bot.sendMessage(chat_id, "Desbloqueando sistema", "");
      motionDetected = false;
    }

    if (text == "/status")
    {
      if (ledStatus)
      {
        bot.sendMessage(chat_id, "Bloqueado", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Desbloqueado", "");
      }
    }

    if (text == "/start")
    {
      String welcome = "Soy el Bot del sistema estas son las opciones, " + from_name + ".\n";
      welcome += "Siguientes comandos.\n\n";
      welcome += "/Bloqueo :El sistema se encuentra bloqueado\n";
      welcome += "/Desbloqueo: Sistema desbloqueado\n";
      welcome += "/status\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup()
{
  Serial.begin(115200);
  Serial.println();
     pinMode(pinservo, OUTPUT);
      
  pinMode(movimSensor, INPUT_PULLUP);
  // establecemos el pin movimSensor como interrupcion, asignamos la interrupcion en modo RISING 
  attachInterrupt(digitalPinToInterrupt(movimSensor), detectamovimiento, RISING);

  
  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, HIGH); // initialize pin as off (active LOW)



  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  if( ledStatus == 1){
 if(motionDetected){
    bot.sendMessage(chat_id, "Se Detecto un Movimiento", "");
    while (x<5){
     bot.sendMessage(chat_id, "Verificar su bicicleta", "");
    x++;
    motionDetected = false; 
    }
  }}
 
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
