# Pràctica 6 - Processadors Digitals: Busos de Comunicació II (SPI)

Aquest repositori conté el codi font i la memòria corresponents a la **Pràctica 6** de l'assignatura de Processadors Digitals. L'objectiu d'aquesta pràctica és l'estudi, configuració i ús del protocol de comunicació síncron d'alta velocitat **SPI (*Serial Peripheral Interface*)** per connectar i gestionar diversos perifèrics amb un microcontrolador ESP32-S3.

## 📂 Estructura del Repositori

El repositori s'organitza en les següents carpetes i fitxers, reflectint l'evolució de la pràctica des de la lectura bàsica de sensors fins a la gestió multiesclau:

* **`Practica6Ex1/`** (Lectura de Memòria SD): 
  * Codi per inicialitzar una targeta micro SD a través del bus SPI i llegir el contingut d'un arxiu de text plan (`.txt`), imprimint les dades pel terminal sèrie de l'ordinador.

* **`Practica6Ex2/`** (Lectura d'Etiqueta RFID): 
  * Programa per establir comunicació amb un mòdul lector de radiofreqüència RFID-RC522. Detecta la presència de targetes o clauers intel·ligents al seu camp magnètic i n'extreu l'Identificador Únic (UID).

* **`Practica6Ex3/`** (Exercici lliure SPI):
  * Implementació pràctica addicional utilitzant elements SPI, corresponent a l'exercici de treball autònom de la pràctica.

* **`Practica6Complementari/`** (Projecte IoT: SD + RFID + Servidor Web): 
  * Exercici de pujada de nota i integració total. L'ESP32 actua com a servidor web i gestiona dos esclaus SPI simultàniament (SD i RFID) resolent els conflictes compartint bus i controlant els pins *Chip Select* (CS).
  * Funcionalitat de *datalogger*: Registra els accessos RFID en un fitxer `fichero.log` a la targeta SD amb l'hora exacta sincronitzada via servidors NTP, i mostra la darrera lectura en una pàgina web interactiva amb auto-refresc.

* **`InformePràctica6PD.pdf`**: 
  * Memòria descriptiva completa del projecte. Inclou l'explicació de l'arquitectura del codi, la resolució de conflictes de maquinari, l'anàlisi de temps de la CPU (avaluant l'impacte de l'espera activa o *polling*), diagrames de flux, diagrames de temps i fotografies dels muntatges físics.

## 🛠️ Tecnologies i Maquinari Utilitzat

* **Microcontrolador:** Placa de desenvolupament ESP32-S3 (Mestre SPI)
* **Dispositius SPI (Esclaus):** * Mòdul adaptador per a targetes micro SD
  * Lector RFID-RC522 (Targetes i clauers a 13.56 MHz)
* **Entorn de desenvolupament:** Visual Studio Code amb l'extensió PlatformIO.
* **Llenguatge:** C++ (Framework d'Arduino)
* **Llibreries principals:** `SPI.h`, `SD.h`, `MFRC522.h` (per Miguel Balboa), `WiFi.h`, `WebServer.h`, `time.h`.

## 👤 Autor
**Marc Gamell**
