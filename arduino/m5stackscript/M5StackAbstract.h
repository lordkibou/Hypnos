#ifndef M5STACKABSTRACT_H
#define M5STACKABSTRACT_H

class M5StackAbstract {
public:
    int receivedSensorsData;
    static M5StackAbstract* getInstance(const char *ssid, const char *pass, int udp) {
        if (instance == nullptr) {
            instance = new M5StackAbstract(ssid, pass, udp);
            instance->initializeM5StackAbstract();
            instance->openUDPConnection();
        }
        return instance;
    }

    void switchLightM5StackAbstract() {
        int steps = 5;  // Número de pasos para la transición gradual de color (incluyendo el color inicial y final)
        uint32_t targetColor;
        isNight = !isNight;
        if (isNight) {
            M5.Lcd.fillScreen(0x000000);  // Negro (Noche)
            M5.Lcd.fillCircle(110, 60, 20, WHITE);
            M5.Lcd.fillCircle(120, 55, 20, BLACK);
        } else {
            M5.Lcd.fillScreen(0xFFFFFF);  // Blanco (Día)
            //playAlarmSound();
        }
    }

    void startRestingTrackRoutine() {
        char texto[200];
        StaticJsonDocument<200> jsonBuffer;
        
        jsonBuffer["mensaje"] = "START_TRACKING";
        Serial.println("Se manda el start tracking");
        serializeJson(jsonBuffer, texto);

        udp.broadcastTo(texto, udpPort);
    }

    void showDataInScreen() {
        M5.Lcd.fillScreen(WHITE); // Borra la pantalla
        M5.Lcd.textsize = 3;
        M5.Lcd.setCursor(0,0); // Establece la posición del cursor en la pantalla
        M5.Lcd.setTextColor(0x164499); // Establece el color del texto
        M5.Lcd.println("Temperatura: " + String(this->averageTemperature) + " C"); // Muestra la temperatura en la pantalla
        M5.Lcd.println("Ronquidos: " + String(this->snoreAmount));
        M5.Lcd.println("Tiempo Descanso: " + String((this->tiempoEncendido)/1000) + " s");
        this->receivedSensorsData = 0;
        playAlarmSound();
    }

    void printLogoWhiteBackground() {
        M5.Lcd.textsize = 6;
        M5.Lcd.fillScreen(WHITE);
        M5.Lcd.setTextColor(0x164499);  // Color del texto azul
        M5.Lcd.setCursor(55, 85);
        M5.Lcd.print("Hypnos");

        M5.Lcd.textsize = 2;
        M5.Lcd.setCursor(45, 200);
        M5.Lcd.print("Boton 1 para empezar");
        //Dibujar luna
        M5.Lcd.fillCircle(110, 60, 20, 0x164499); // Coordenadas (170, 50), radio 20, color del texto azul
        M5.Lcd.fillCircle(120, 55, 20, WHITE);    // Otra luna blanca más fina
    }

    void receiveSensorsData() {
    udp.onPacket([this](AsyncUDPPacket &packet) {
        char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
        int len = packet.length();
        
        // Leer los datos del paquete en el buffer
        for (int i = 0; i < len; i++) {
            packetBuffer[i] = packet.data()[i];
        }
        packetBuffer[len] = '\0';

        // Parsear el JSON recibido
        DynamicJsonDocument jsonDoc(200);  // Ajusta el tamaño según tus necesidades
        DeserializationError error = deserializeJson(jsonDoc, packetBuffer);

        if (!error) {
            // Acceder a los valores recibidos y almacenarlos según tus necesidades
            Serial.println("Recibe los datos");
            int isTemperatureReceived = jsonDoc["averageTemperature"];
            Serial.println(isTemperatureReceived);
            if(isTemperatureReceived >0){
              this->averageTemperature = jsonDoc["averageTemperature"];
              this->snoreAmount = jsonDoc["snoreAmount"];
            }
            else{
              this->tiempoEncendido = jsonDoc["sleepTime"];
            }
            Serial.println(averageTemperature);
            Serial.println(snoreAmount);
            Serial.println(tiempoEncendido);

            this->receivedSensorsData +=1;
        } else {
            Serial.println("Error al analizar los datos JSON recibidos");
        }
    });
    }


private:
    bool isNight = false;
    int udpPort;
    char ssidWifi[32]; // Array para almacenar el SSID, con un máximo de 32 caracteres incluyendo el carácter nulo '\0'
    char passwordWifi[64];
    static const int UDP_TX_PACKET_MAX_SIZE = 200;

    int averageTemperature;
    unsigned int snoreAmount;
    unsigned int tiempoEncendido;

    uint16_t mainColor = 0x164499;
    AsyncUDP udp;


   M5StackAbstract(const char *ssid, const char *pass, int udp) {
        strncpy(ssidWifi, ssid, sizeof(ssidWifi) - 1); // Copia el SSID a ssidWifi, asegurando que no sobrepase el tamaño del array
        strncpy(passwordWifi, pass, sizeof(passwordWifi) - 1); // Copia la contraseña a passwordWifi, asegurando que no sobrepase el tamaño del array
        ssidWifi[sizeof(ssidWifi) - 1] = '\0'; // Asegura que ssidWifi tenga el carácter nulo al final
        passwordWifi[sizeof(passwordWifi) - 1] = '\0'; // Asegura que passwordWifi tenga el carácter nulo al final
        udpPort = udp;
        receivedSensorsData = 0;
    }

    void initializeM5StackAbstract() {
        M5.begin(); //Init M5Core. Initialize M5Core
        M5.Power.begin(); //Init Power module. Initialize the power module
    }

    void openUDPConnection(){
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssidWifi, passwordWifi);
        if(WiFi.waitForConnectResult()!=WL_CONNECTED) {
            Serial.println("Conectando a WiFi...");
            while(1) {
              delay(1000);
            }
        }

        Serial.println("Conexión WiFi establecida");

        if(udp.listen(udpPort)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        }
    }

    void playAlarmSound() { // Sonido de alarma
      unsigned long startTime = millis(); // Guardar el tiempo inicial
      unsigned long maxDuration = 5000;   // Duración máxima en milisegundos (en este caso, 5 segundos)
      int numBeeps = 5;                   // Número de pitidos
      unsigned long beepDuration = 200;   // Duración de cada pitido en milisegundos
      unsigned long beepInterval = 1500;  // Intervalo entre pitidos en milisegundos

      while (millis() - startTime < maxDuration) {
          for (int i = 0; i < numBeeps; ++i) {
              M5.Speaker.tone(500, beepDuration);  // Tono de 500 Hz durante la duración del pitido
              delay(beepInterval);                 // Espera breve entre tonos
              M5.Speaker.mute();                    // Silencio entre pitidos
              delay(beepInterval);                 // Espera breve entre pitidos
              Serial.println("BEEP");
          }
      }

      // Detener el sonido
      M5.Speaker.mute();
    }
    
    static M5StackAbstract* instance;
};

M5StackAbstract* M5StackAbstract::instance = nullptr;

#endif