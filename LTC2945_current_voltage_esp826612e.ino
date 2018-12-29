//LTC2945 Test Code
//  original author http://www.kevindarrah.com/wiki/index.php?title=Power_Monitor
// Original program was set up to use an arduino mini.
// Modified by JAL April 27 2017 to use an ESP8266-12E to upload 0-80V inputVoltage to thingspeak.
// Use GPIO4 for SDA and GPIO5 for SCL
// The LTC2945 VDD supply can be any voltage between 2.7V to 80V.
// The LTC2945 can use either 3.3v or 5V for SDA and SCL. 
// Not using ALERT function.
// You must select and wire a resistance shunt to use across (sense +) and (sense -) to measure current and voltage.
// To measure volts only up to 80vdc, remove the resistance shunt and apply the voltage between (sense +) and ground.  No connection to (-sense).

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>



const char* host = "api.thingspeak.com"; // Your domain  
String ApiKey = "";  //put your thingspeak api key between the quotes
String path = "/update?key=" + ApiKey + "&field1=";  

const char* ssid = "";  //put your network ssid name between the quotes
const char* pass = "";  //put your network password between the quotes



//defines
#define LTCADDR B1101111//Table 1 both LOW (7bit address)
byte ADCvinMSB, ADCvinLSB, curSenseMSB, curSenseLSB, AinVMSB, AinVLSB;
unsigned int ADCvin, ADCcur, AinV;
float inputVoltage, ADCvoltage, current10, current1, current0p1, current0p01;


void setup() {//setup
  Serial.begin(115200);
  Wire.begin();

  WiFi.begin(ssid, pass);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  Wire.beginTransmission(LTCADDR);//first get Input Voltage - 80V max
  Wire.write(0x1E);
  Wire.endTransmission(false);
  Wire.requestFrom(LTCADDR, 2, true);
  delay(1);
  ADCvinMSB = Wire.read();
  ADCvinLSB = Wire.read();
  ADCvin = ((unsigned int)(ADCvinMSB) << 4) + ((ADCvinLSB >> 4) & 0x0F);//formats into 12bit integer
  inputVoltage = ADCvin * 0.025; //25mV resolution

  Wire.beginTransmission(LTCADDR);//get ADC Input 2V max
  Wire.write(0x28);
  Wire.endTransmission(false);
  Wire.requestFrom(LTCADDR, 2, true);
  delay(1);
  AinVMSB = Wire.read();
  AinVLSB = Wire.read();
  AinV = ((unsigned int)(AinVMSB) << 4) + ((AinVLSB >> 4) & 0x0F);//12 bit format
  ADCvoltage = AinV * 0.5E-3; //500uV resolution

  Wire.beginTransmission(LTCADDR);//get sense current
  Wire.write(0x14);
  Wire.endTransmission(false);
  Wire.requestFrom(LTCADDR, 2, true);
  delay(1);
  curSenseMSB = Wire.read();
  curSenseLSB = Wire.read();
  ADCcur = ((unsigned int)(curSenseMSB) << 4) + ((curSenseLSB >> 4) & 0x0F);//12 bit format
  //gets voltage across, 25uV resolution, then this converts to voltage for each sense resistor
  current10 = ADCcur * (25E-3) / 10.0; //10mA max, unit is mA
  current1 = ADCcur * (25E-3) / 1.0; //100mA max, unit is mA
  current0p1 = ADCcur * (25E-3) / 0.1; //1A max, unit is mA
  current0p01 = ADCcur * (25E-6) / 0.01;//10A max, unit is A

  //Print everything out
  Serial.print("Vin:25mV/80V>");
  Serial.print(inputVoltage, 2);
  Serial.print("V 10ohm:2.5uA/10mA>");
  Serial.print(current10, 3);
  Serial.print("mA 1ohm:25uA/100mA>");
  Serial.print(current1, 2);
  Serial.print("mA 0.1ohm:250uA/1A>");
  Serial.print(current0p1, 1);
  Serial.print("mA 0.01ohm:2.5mA/10A>");
  Serial.print(current0p01, 3);
  Serial.print("A ADC:0.5mV/2V>");
  Serial.print(ADCvoltage, 4);
  Serial.println("V");

 
 WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.print(String("GET ") + path + inputVoltage + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
               
  delay(30000);
  delay(30000);

 // delay(1000);


}
