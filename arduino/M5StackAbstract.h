#ifndef M5STACKABSTRACT_H
#define M5STACKABSTRACT_H

class M5StackAbstract {
public:
    static M5StackAbstract* getInstance(const char *ssid, const char *pass, int udp) {
        if (instance == nullptr) {
            instance = new M5StackAbstract(ssid, pass, udp);
            instance->initializeM5StackAbstract();
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
        }
    }

    //Cuando boton A se pulsa por 1,5 segundos en loop()
    void startRestingTrackRoutine() {
        openUDPConnection();
        // Aquí se debe comunicar con el ESP32 mediante UDP para indicarle que comience a tomar medidas con obtainSensorData() de la clase ESP32
        // Puedes usar el objeto udp para enviar el mensaje al ESP32
        udp.beginPacket(udp.remoteIP, udpPort);


        //REVISAR udp.remoteIP PUEDE PRODUCIR ERRORES 

        udp.printf("START_TRACKING");
        udp.endPacket();
    }

    //Cuando el boton B se pulsa por 1,5 segundos en loop()
    void stopRestingTrackRoutine(){
        // Aquí se debe comunicar con el ESP32 mediante UDP para indicarle que ya puede enviar los datos
        // Puedes usar la función receiveSensorsData() para recibir el JSON enviado por el ESP32
        udp.beginPacket(udp.remoteIP, udpPort);
        

        //REVISAR udp.remoteIP PUEDE PRODUCIR ERRORES 

        udp.printf("STOP_TRACKING");
        udp.endPacket();
        // Aquí se reciben los datos por UTP y se guardan en snoreAmount y averageTemperature

        //AQUI PUEDE QUE HAGA FALTA ESPERAR A QUE EL ESP32 ENVIE LAS MEDIDAS
        //DEBIDO A QUE ES ASINCRONO
        receiveSensorsData();
    }

    //Cuando el boton C se pulsa por 1,5 segundos en loop() se muestran lo
    void showDataInScreen(int snoreAmount,int averageTemperature) {
        M5.Lcd.fillScreen(WHITE); // Borra la pantalla
        M5.Lcd.textsize = 3;
        M5.Lcd.setCursor(0,0); // Establece la posición del cursor en la pantalla
        M5.Lcd.setTextColor(0x164499); // Establece el color del texto
        M5.Lcd.println("Temperatura: " + String(averageTemperature) + " C"); // Muestra la temperatura en la pantalla
        M5.Lcd.println("Ronquidos: " + String(snoreAmount));
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

private:
    bool isNight = false;
    int udpPort;
    char ssidWifi[32]; // Array para almacenar el SSID, con un máximo de 32 caracteres incluyendo el carácter nulo '\0'
    char passwordWifi[64];
    uint16_t mainColor = 0x164499;
    WiFiUDP udp;


   M5StackAbstract(const char *ssid, const char *pass, int udp) {
        strncpy(ssidWifi, ssid, sizeof(ssidWifi) - 1); // Copia el SSID a ssidWifi, asegurando que no sobrepase el tamaño del array
        strncpy(passwordWifi, pass, sizeof(passwordWifi) - 1); // Copia la contraseña a passwordWifi, asegurando que no sobrepase el tamaño del array
        ssidWifi[sizeof(ssidWifi) - 1] = '\0'; // Asegura que ssidWifi tenga el carácter nulo al final
        passwordWifi[sizeof(passwordWifi) - 1] = '\0'; // Asegura que passwordWifi tenga el carácter nulo al final
        udpPort = udp;
    }

    void initializeM5StackAbstract() {
        M5.begin(); //Init M5Core. Initialize M5Core
        M5.Power.begin(); //Init Power module. Initialize the power module
    }

    void openUDPConnection(){
        WiFi.begin(ssidWifi, passwordWifi);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Conectando a WiFi...");
        }
        Serial.println("Conexión WiFi establecida");

        udp.begin(udpPort);
    }

    void receiveSensorsData(){
         // Aquí se debe recibir la información JSON enviada por el ESP32 mediante UDP
        // Puedes usar el objeto udp para recibir los datos y luego parsear el JSON
        char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
        int packetSize = udp.parsePacket();
        if (packetSize) {
            int len = udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
            if (len > 0) {
                packetBuffer[len] = '\0';
                // Parsear el JSON recibido
                DynamicJsonDocument doc(1024);
                DeserializationError error = deserializeJson(doc, packetBuffer);
                if (!error) {
                    // Obtener los valores de las medidas
                    averageTemperature = doc["averageTemperature"];
                    snoreAmount = doc["snoreAmount"];
                }
            }
        }
    }

    static M5StackAbstract* instance;
};

M5StackAbstract* M5StackAbstract::instance = nullptr;

#endif