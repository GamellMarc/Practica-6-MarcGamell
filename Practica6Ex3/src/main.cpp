#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SD.h>
#include <time.h>

// --- Credencials Wi-Fi ---
const char* ssid = "Nautilus";
const char* password = "20000Leguas";

// --- Configuració de Pins ESP32-S3 ---
// Pins compartits del bus SPI
#define SCK_PIN 36
#define MISO_PIN 37
#define MOSI_PIN 35

// Pins específics del RFID
#define RFID_CS_PIN 10
#define RFID_RST_PIN 9

// Pin específic de l'SD (Pots canviar-lo per un altre que tinguis lliure)
#define SD_CS_PIN 4 

// --- Configuració Servidor de Temps (NTP) ---
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;      // Zona horària (Espanya península = +1h = 3600s)
const int   daylightOffset_sec = 3600; // Horari d'estiu (+1h)

// Inicialització del lector RFID
MFRC522 mfrc522(RFID_CS_PIN, RFID_RST_PIN);

// Servidor web al port 80
WebServer server(80);

// Variable per emmagatzemar la lectura i mostrar-la a la web
String ultimaTargeta = "Cap targeta detectada encara";

// Funció que genera el contingut de la pàgina web
void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'><meta http-equiv='refresh' content='2'>";
  html += "<style>body{font-family:sans-serif; text-align:center; padding-top:50px; background:#eef2f3;} ";
  html += ".box{background:white; padding:30px; border-radius:15px; display:inline-block; box-shadow:0 4px 6px rgba(0,0,0,0.1);} ";
  html += "h1{color:#2c3e50;} .uid{font-size:35px; color:#e74c3c; font-weight:bold;}</style></head><body>";
  html += "<div class='box'><h1>Monitor RFID SPI</h1>";
  html += "<p>Darrera lectura realitzada:</p>";
  html += "<div class='uid'>" + ultimaTargeta + "</div>";
  html += "<br><p>L'estat del bus SPI és correcte.</p></div></body></html>";
  
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  // 1. Configuració del bus SPI compartit (per a ESP32-S3)
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);
  
  // 2. Inicialització del lector RFID
  mfrc522.PCD_Init();
  Serial.println("Lector RFID inicialitzat.");

  // 3. Inicialització de la targeta SD (li passem el pin CS i el bus SPI configurat)
  if (!SD.begin(SD_CS_PIN, SPI)) {
    Serial.println("Error inicialitzant la targeta SD! Comprova les connexions.");
  } else {
    Serial.println("Targeta SD inicialitzada correctament.");
  }
  
  // 4. Connexió al Wi-Fi
  Serial.print("Connectant a la xarxa: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnexió establerta!");
  Serial.print("Adreça IP del servidor: http://");
  Serial.println(WiFi.localIP());

  // 5. Configuració de l'hora (NTP)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Sincronitzant l'hora amb el servidor NTP...");

  // Rutes del servidor web 
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  // Gestió de les peticions web
  server.handleClient();

  // Detecció de noves targetes RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    
    // Convertim l'UID a un String Hexadecimal
    String codiHex = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      codiHex += (mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      codiHex += String(mfrc522.uid.uidByte[i], HEX);
    }
    codiHex.toUpperCase();
    codiHex.trim(); // Treiem espais al principi i al final
    
    // Actualitzem la variable global per a la pàgina web
    ultimaTargeta = codiHex;
    
    Serial.print("Lectura detectada: ");
    Serial.println(ultimaTargeta);

    // --- PART DE LA SD: Obtenir l'hora i guardar a fichero.log ---
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Error obtenint l'hora");
    } else {
      // Donem format a l'hora: DD/MM/AAAA HH:MM:SS
      char timeStringBuff[50];
      strftime(timeStringBuff, sizeof(timeStringBuff), "%d/%m/%Y %H:%M:%S", &timeinfo);
      
      // Obrim l'arxiu en mode afegir (FILE_APPEND)
      File dataFile = SD.open("/fichero.log", FILE_APPEND);
      
      if (dataFile) {
        dataFile.print("[");
        dataFile.print(timeStringBuff);
        dataFile.print("] UID detectat: ");
        dataFile.println(ultimaTargeta);
        dataFile.close(); // Tancar sempre el fitxer per guardar els canvis
        Serial.println("Registre guardat correctament a fichero.log");
      } else {
        Serial.println("Error obrint /fichero.log per escriure.");
      }
    }
    // -------------------------------------------------------------
    
    // Aturem la lectura actual per no llegir la mateixa targeta 1000 vegades per segon
    mfrc522.PICC_HaltA();
    delay(500);
  }
}