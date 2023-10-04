/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>


#define STASSID "home"
#define STAPSK  "30011980"


IPAddress ip(192, 168, 1, 40);// Задаем статический IP-адрес:
IPAddress gateway(192,168,1,198);
IPAddress primaryDNS(192, 168, 1, 198);   // опционально
IPAddress subnet(255, 255, 255, 0);
IPAddress secondaryDNS(8, 8, 8, 8); // опционально

const char* host = "esp8266-webupdate";
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

#include <ESP8266FtpServer.h> 
FtpServer ftpSrv; //для фтп


void setup(void) {



  Serial.begin(9600);  Serial.println();  Serial.println("Booting Sketch...");
  
 // WiFi.mode(WIFI_AP_STA);
    WiFi.mode (WIFI_STA); 
    WiFi.begin(ssid, password);
    WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {    WiFi.begin(ssid, password);    Serial.println("WiFi failed, retrying.");  }

  //MDNS.begin(host);

  httpUpdater.setup(&httpServer);     //для обновления
  httpServer.begin();

  ftpSrv.begin("admin5","");                                        // Поднимаем FTP-сервер для удобства отладки работы HTML (логин: relay, пароль: relay)

 // MDNS.addService("http", "tcp", 80);  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}

void loop(void) {
  httpServer.handleClient();
    delay(1);
    ftpSrv.handleFTP();                                                 // Обработчик FTP-соединений  
    delay(1);

  //MDNS.update();
}
