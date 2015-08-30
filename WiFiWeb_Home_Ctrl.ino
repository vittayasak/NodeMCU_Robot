/* ############################################################ 
 *  By: Vittaysak Rujivorakul
 *  Android App for Controller:  https://play.google.com/store/apps/details?id=com.br3.udpctl&hl=th
 *  Hardware Micro Controller: NodeMCU V2 (ESP8266-12) found at http://www.nodemcu.com/
 *  Enjoy your kids
 *########################################################### */

#include <ESP8266WiFi.h>

const char* ssid = "WIFI_SSD";
const char* password = "WIFI_Password";

int led1=1;
int led2=2;
int led3=3;
int led4=4;
int led5=5;
int led6=16;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  
  digitalWrite(led1, 0);
  digitalWrite(led2, 0);
  digitalWrite(led3, 0);
  digitalWrite(led4, 0);
  digitalWrite(led5, 0);
  digitalWrite(led6, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;


if (req.indexOf("/gpio/") != -1){
  int start = req.indexOf("/gpio/");
  int eidx = req.indexOf(" HTTP");
  String nval = req.substring(start+6,eidx);
  
  val = nval.toInt();
  
} else {
  val = 0;
//    Serial.println("invalid request");
//    client.stop();
//    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(led1, (((val == 1) !=0))?!digitalRead(led1):digitalRead(led1));
  digitalWrite(led2, (((val == 2) !=0))?!digitalRead(led2):digitalRead(led2));
  digitalWrite(led3, (((val == 3) !=0))?!digitalRead(led3):digitalRead(led3));
  digitalWrite(led4, (((val == 4) !=0))?!digitalRead(led4):digitalRead(led4));
  digitalWrite(led5, (((val == 5) !=0))?!digitalRead(led5):digitalRead(led5));
  digitalWrite(led6, (((val == 6) !=0))?!digitalRead(led6):digitalRead(led6));
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Access-Control-Allow-Origin:*\r\nAccess-Control-Allow-Methods:POST, GET\r\nAccess-Control-Allow-Headers:AUTHORISED\r\n";
  s += "Content-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";
  s += "\r\n<head>";
  s += "<style>";
  s += "a{";
  s += "   display:inline-block;text-align:center;height:60px; width:60px; border: black solid 4px;";
  s += "   font-size:2em; padding:0.6em; border-radius:1.2em;";
  s += "   magin:0.5em;";
  s += "}\r\n";
  s += ".on{ background-color:red; }\r\n";
  s += ".off{ background-color:white;}\r\n";
  s += "</style>";
  s+= "\r\n</head>";
  s+= "<body>";
  s += "<h1>";
  s += "<a href=\"/gpio/1\" class=\"" ;
  s +=  digitalRead(led1)?"on":"off";
  s += "\">1</a> ";

   s += "<a href=\"/gpio/2\" class=\"" ;
  s +=  digitalRead(led2)?"on":"off";
  s += "\">2</a> ";

   s += "<a href=\"/gpio/3\" class=\"" ;
  s +=  digitalRead(led3)?"on":"off";
  s += "\">3</a> ";

   s += "<a href=\"/gpio/4\" class=\"" ;
  s +=  digitalRead(led4)?"on":"off";
  s += "\">4</a> ";

   s += "<a href=\"/gpio/5\" class=\"" ;
  s +=  digitalRead(led5)?"on":"off";
  s += "\">5</a> ";

   s += "<a href=\"/gpio/6\" class=\"" ;
  s +=  digitalRead(led6)?"on":"off";
  s += "\">6</a> ";
  

  s += "</h1>";
  s += "\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "<br/><br/>[" + req +"]<br/>" + val;
  s += "</body>";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
