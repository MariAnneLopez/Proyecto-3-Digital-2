//  ESP32 Web Server
//  Universidad del Valle de Guatemala
//  Electronica digital 2
//  Proyecto 3
//  Maria Andrea López
//  Enma Lopez

//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>

//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "Luna";  // Nombre de la red de internet
const char* password = "enma2010";  // Contraseña de la red de internet
WebServer server(80);  // Crear el servidor (escoger puerto del servidor habilitado) 80 = default para ver paginas web

// Leds simulados de parqueo
uint8_t PUSH1pin = 21;
uint8_t LED1pin = 15;
bool LED1status = LOW;
uint8_t PUSH2pin = 23;
uint8_t LED2pin = 4;
bool LED2status = LOW;

// Datos para actualizar pagina web
char p1status = 0;
char p2status = 0;
char p3status = 0;
char p4status = 0;
char p5status = 0;
char p6status = 0;
char p7status = 0;
char p8status = 0;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200); // Inicializar el serial
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Leds parqueo
  pinMode(PUSH1pin, INPUT_PULLUP);
  pinMode(LED1pin, OUTPUT);
  pinMode(PUSH2pin, INPUT_PULLUP);
  pinMode(LED2pin, OUTPUT);

  // Conectando al modem Wi-Fi
  WiFi.begin(ssid, password);

  // Revisar si se conecto a la red Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP()); // Obtiene la IP del ESP32

  server.on("/", handle_OnConnect); // Al acceder al servidor, realizar funcion
  server.on("/actualizar", handle_actualizar);  // Al presionar boton de actualizar, realizar funcion
  server.onNotFound(handle_NotFound); // Al no encontrar el servidor, realizar funcion

  server.begin(); // Iniciar servidor
  Serial.println("HTTP server started");
  delay(100);

  //Comunicacion UART con tiva
  Serial2.begin(115200);

}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient(); // Hace que el servidor atienda a los clientes que se quieren conectar
  
  // Recibir información del UART de la Tiva
  //delay(100);
  //p1status = Serial2.read();
  //p2status = Serial2.read();
  //p3status = Serial2.read();
  //p4status = Serial2.read();

  //Serial.println(p1status);
  //Serial.println(p2status);
  //Serial.println(p3status);
  //Serial.println(p4status); 

  // Actualizar datos de parqueo
  p1status = digitalRead(PUSH1pin);
  if (p1status == 1)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else if (p1status == 0)
  {
    digitalWrite(LED1pin, LOW);
  }

  p2status = digitalRead(PUSH2pin);
  if (p2status == 1)
  {
    digitalWrite(LED2pin, HIGH);
  }
  else if (p2status == 0)
  {
    digitalWrite(LED2pin, LOW);
  }
}

//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  LED1status = LOW;
  server.send(200, "text/html", SendHTML(p1status,p2status,p3status,p4status,p5status,p6status,p7status,p8status)); // Indica que va a enviar una pagina en formato texto
}

//************************************************************************************************
// Handler de Actualizar pagina
//************************************************************************************************
void handle_actualizar() {
  Serial.println("Actualizar pagina");
  server.send(200, "text/html", SendHTML(p1status,p2status,p3status,p4status,p5status,p6status,p7status,p8status)); // Indica que va a enviar una pagina en formato texto
}

//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

//************************************************************************************************
// Pagina de HTML
//************************************************************************************************
String SendHTML(char led1stat, char led2stat, char led3stat, char led4stat, char led5stat, char led6stat, char led7stat, char led8stat){//,char led2stat, char led3stat, char led4stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<!-- Bootstrap CSS -->";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Estado Parqueomatic &#128664 </h1>";
  ptr += "<br>\n";
  // Boton que envia al handler de actualizar pagina
  ptr += "<a class=\"button button-off\" href=\"actualizar\">Actualizar</a>";
  ptr += "<br>";
  ptr += "<table class=\"table\">";
  ptr += "<thead class=\"table-dark\">";
  ptr += "<tr>";
  ptr += "<th scope=\"col\"># Parqueo</th>";
  ptr += "<th scope=\"col\">Estado</th>";
  ptr += "<tr>";
  ptr += "</thead>";
  ptr += "<tbody>";

  // Actualizar tabla segun estado de parqueo 1
  if (led1stat==1)
  {
    ptr += "<tr class=\"table-success\">\n";
    ptr += "<th scope=\"row\">1</th>\n";
    ptr += "<td>Disponible &#128664</td>\n";
    ptr += "</tr>\n";
  }
  else
  {
    ptr += "<tr class=\"table-danger\">\n";
    ptr += "<th scope=\"row\">1</th>\n";
    ptr += "<td>Ocupado &#128683</td>\n";
    ptr += "</tr>\n";
  }

  // Actualizar tabla segun estado de parqueo 2
  if (led2stat==1)
  {
    ptr += "<tr class=\"table-success\">\n";
    ptr += "<th scope=\"row\">2</th>\n";
    ptr += "<td>Disponible &#128664</td>\n";
    ptr += "</tr>\n";
  }
  else
  {
    ptr += "<tr class=\"table-danger\">\n";
    ptr += "<th scope=\"row\">2</th>\n";
    ptr += "<td>Ocupado &#128683</td>\n";
    ptr += "</tr>\n";
  }

  // Actualizar tabla segun estado de parqueo 3
  if (led3stat==1)
  {
    ptr += "<tr class=\"table-success\">\n";
    ptr += "<th scope=\"row\">3</th>\n";
    ptr += "<td>Disponible &#128664</td>\n";
    ptr += "</tr>\n";
  }
  else
  {
    ptr += "<tr class=\"table-danger\">\n";
    ptr += "<th scope=\"row\">3</th>\n";
    ptr += "<td>Ocupado &#128683</td>\n";
    ptr += "</tr>\n";
  }

  // Actualizar tabla segun estado de parqueo 4
  if (led4stat==1)
  {
    ptr += "<tr class=\"table-success\">\n";
    ptr += "<th scope=\"row\">4</th>\n";
    ptr += "<td>Disponible &#128664</td>\n";
    ptr += "</tr>\n";
  }
  else
  {
    ptr += "<tr class=\"table-danger\">\n";
    ptr += "<th scope=\"row\">4</th>\n";
    ptr += "<td>Ocupado &#128683</td>\n";
    ptr += "</tr>\n";
  }

  // Actualizar tabla segun estado de parqueo 5
  if (led5stat==1)
  {
    ptr += "<tr class=\"table-success\">\n";
    ptr += "<th scope=\"row\">5</th>\n";
    ptr += "<td>Disponible &#128664</td>\n";
    ptr += "</tr>\n";
  }
  else
  {
    ptr += "<tr class=\"table-danger\">\n";
    ptr += "<th scope=\"row\">5</th>\n";
    ptr += "<td>Ocupado &#128683</td>\n";
    ptr += "</tr>\n";
  }

  // Actualizar tabla segun estado de parqueo 6
  if (led6stat==1)
  {
    ptr += "<tr class=\"table-success\">\n";
    ptr += "<th scope=\"row\">6</th>\n";
    ptr += "<td>Disponible &#128664</td>\n";
    ptr += "</tr>\n";
  }
  else
  {
    ptr += "<tr class=\"table-danger\">\n";
    ptr += "<th scope=\"row\">6</th>\n";
    ptr += "<td>Ocupado &#128683</td>\n";
    ptr += "</tr>\n";
  }

  // Actualizar tabla segun estado de parqueo 7
  if (led7stat==1)
  {
    ptr += "<tr class=\"table-success\">\n";
    ptr += "<th scope=\"row\">7</th>\n";
    ptr += "<td>Disponible &#128664</td>\n";
    ptr += "</tr>\n";
  }
  else
  {
    ptr += "<tr class=\"table-danger\">\n";
    ptr += "<th scope=\"row\">7</th>\n";
    ptr += "<td>Ocupado &#128683</td>\n";
    ptr += "</tr>\n";
  }

  // Actualizar tabla segun estado de parqueo 8
  if (led8stat==1)
  {
    ptr += "<tr class=\"table-success\">\n";
    ptr += "<th scope=\"row\">8</th>\n";
    ptr += "<td>Disponible &#128664</td>\n";
    ptr += "</tr>\n";
  }
  else
  {
    ptr += "<tr class=\"table-danger\">\n";
    ptr += "<th scope=\"row\">8</th>\n";
    ptr += "<td>Ocupado &#128683</td>\n";
    ptr += "</tr>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
