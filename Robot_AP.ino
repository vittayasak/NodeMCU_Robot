
/* ############################################################ 
 *  By: Vittaysak Rujivorakul
 *  Android App for Controller:  https://play.google.com/store/apps/details?id=com.br3.udpctl&hl=th
 *  My Demo Clip VDO: https://www.youtube.com/watch?v=E85RfNlRmHU 
 *  Hardware Micro Controller: NodeMCU V2 (ESP8266-12) found at http://www.nodemcu.com/
 *  Enjoy your kids
 *########################################################### */
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "AP_SSID";
const char *password = "";

ESP8266WebServer server(80);



  int pin1_motor_R = 1;   ///E2
 int pin2_motor_R = 2;
 int pin1_motor_L = 3;   // E1
 int pin2_motor_L = 4;
 
int FW=0,BK=1,ST=2;
int dir=FW;

String cmd="1";

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  cmd=server.arg("cmd");
  // get cmd parameter from url :  http://192.168.4.1/?cmd=1
  checkcommand();
  
	server.send(200, "text/html", "<h1>You are connected</h1>" + cmd);
}



void setup() {
	delay(1000);
	Serial.begin(115200);

   pinMode(pin1_motor_R , OUTPUT);
   pinMode(pin2_motor_R, OUTPUT);
   pinMode(pin1_motor_L, OUTPUT);
   pinMode(pin2_motor_L, OUTPUT);

 
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);

	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
   
	server.handleClient();
}

void forward()
 {
   delay(10);
   Serial.println("forward");
   
   dir=FW;
   digitalWrite(pin1_motor_R, HIGH);
   digitalWrite(pin2_motor_R, LOW);
   
   digitalWrite(pin1_motor_L, HIGH);
   digitalWrite(pin2_motor_L, LOW);
   
 }


 void back()
 {
   delay(10);
   Serial.println("back");
   dir=BK;
   digitalWrite(pin1_motor_R, LOW);
   digitalWrite(pin2_motor_R, HIGH);
   
   digitalWrite(pin1_motor_L, LOW);
   digitalWrite(pin2_motor_L, HIGH);
 }

 void left()
 {
  delay(10);
  Serial.println("left");
  digitalWrite(pin1_motor_R, LOW);
  digitalWrite(pin2_motor_R, LOW);
   
   digitalWrite(pin1_motor_L, HIGH);
   digitalWrite(pin2_motor_L, LOW);
 }
 void right()
 {
  delay(10);
  Serial.println("Right");
   digitalWrite(pin1_motor_R, HIGH);
  digitalWrite(pin2_motor_R, LOW);
   
   digitalWrite(pin1_motor_L, LOW);
   digitalWrite(pin2_motor_L, LOW);
 }


void turnR()
 {
  delay(10);
  Serial.println("Turn0");
   digitalWrite(pin1_motor_R, HIGH);
  digitalWrite(pin2_motor_R, LOW);
   
   digitalWrite(pin1_motor_L, LOW);
   digitalWrite(pin2_motor_L, HIGH);
 }
 void turnL()
 {
  delay(10);
  Serial.println("Turn1");
   digitalWrite(pin1_motor_R, LOW);
  digitalWrite(pin2_motor_R, HIGH);
   
   digitalWrite(pin1_motor_L, HIGH);
   digitalWrite(pin2_motor_L, LOW);
 }

 void Stop()
 {
  dir=ST;
  delay(10);
  Serial.println("Stop");
  
  digitalWrite(pin1_motor_R, LOW);
  digitalWrite(pin2_motor_R, LOW);
   
   digitalWrite(pin1_motor_L, LOW);
   digitalWrite(pin2_motor_L, LOW);
  
 }

 void next(){
            if(dir==FW){
              forward();
            }else if(dir==BK){
               back();
            }else{
              Stop(); 
            }
 }

 void checkcommand()
 {
    
   int cm = cmd.toInt();
   switch(cm){
    //UP
    case 1:                                       break;
    
    //DOWN
    case 2:                                       break;

    //LEFT-90
    case 3:  left();    delay((dir==ST)?570:1100);   next();     break;

    //RIGH-90
    case 4:  right();   delay((dir==ST)?570:1100);   next();     break;

    //STOP
    case 5:  Stop();                              break;

   //U-Tern 180
    case 6: turnL();  delay((dir==ST)?620:950);   next();       break;

   //Turn-Left
    case 7: turnL();                              break;

   //Turn-Righ
    case 8:   turnR();                            break;

   //FW
    case 9:   forward();                          break;

    //STEP-FW
    case 10:  forward();  delay(50);  Stop();     break;

   //STEP-LEFT
    case 11: left();  delay(50);  Stop();         break;

   //STEP-RIGH
    case 12: right(); delay(50); Stop();          break;

   //BACK
    case 13: back();                              break;

   //STEP-BACK
    case 14: back(); delay(50); Stop();           break;

   //LEFT
    case 15: left(); delay(200);  next();         break;

   //RIGH
    case 16: right(); delay(200); next();         break;

    default: Stop();                              break;
    
   }

 }
