#include <SPI.h>
#include <SD.h>

File myFile;

void setup()
{
  Serial.begin(9600);
  Serial.print("Iniciando SD ...");
  
  if (!SD.begin(4))
  {
    Serial.println("No se pudo inicializar");
    return;
  }

  Serial.println("inicializacion exitosa");
  
  myFile = SD.open("archivo.txt");
  
  if (myFile) {
    Serial.println("archivo.txt:");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("Error al abrir el archivo");
  }
}

void loop()
{
}