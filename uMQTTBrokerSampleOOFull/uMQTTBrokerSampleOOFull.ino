/*
****** Modificaciones para permitir la conexion mediante WifiPortal
****** Basado en la libreria uMQTTBroker
*/

/*
 * uMQTTBroker demo for Arduino (C++-style)
 * 
 * The program defines a custom broker class with callbacks, 
 * starts it, subscribes locally to anything, and publishs a topic every second.
 * Try to connect from a remote client and publish something - the console will show this as well.
 */

#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
/*
 * Necesario para que funcione el wifi portal
 */
 
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>      

/*
 * Custom broker class with overwritten callback functions
 */
class myMQTTBroker: public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString()+" connected");
      return true;
    }
    
    virtual bool onAuth(String username, String password) {
      Serial.println("Username/Password: "+username+"/"+password);
      return true;
    }
    
    virtual void onData(String topic, const char *data, uint32_t length) {
      char data_str[length+1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';
      
      Serial.println("received topic '"+topic+"' with data '"+(String)data_str+"'");
    }
};

myMQTTBroker myBroker;


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
   WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP");
  
  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();

/*
 * Subscribe to anything
 */
  myBroker.subscribe("#");
}

int counter = 0;

void loop()
{
/*
 * Publish the counter value as String
 */
  myBroker.publish("broker/counter", (String)counter++);
   
  // Espera de 20 segundos
  delay(20000);
}
