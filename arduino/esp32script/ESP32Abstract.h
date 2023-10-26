//Cuando recibe la orden de tomar medidas, que encienda el led por 6 segundos simulando antes de acostarse
//la pantalla que vaya oscureciendo entera poco a poco simulando la noche
//hasta que se apagan las luces como que ya se acosto y se comienza a tomar medidas
//cuando llega la orden de parar de tomar medidas, comienza a sonar alarma del M5Stack o ESP32 y
//se encienden las luces

#ifndef ESP32Abstract_H
#define ESP32Abstract_H

class ESP32Abstract {
public:
    static ESP32Abstract* getInstance(const char *ssid, const char *pass, int udp, int tempPIN, int soundPIN, int ledPIN) {
        if (instance == nullptr) {
            instance = new ESP32Abstract(ssid, pass, udp, tempPIN, soundPIN, ledPIN);
            instance->openUDPConnection();
        }
        return instance;
    }

    void listenForMessages() {

        Serial.println("listening for messages");
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
            Serial.println("No hay error al descomprimir el json");
            const char *message = jsonDoc["mensaje"];
            if (message) {
                if (strcmp(message, "START_TRACKING") == 0) {
                    // Recibido un mensaje para iniciar el seguimiento
                    obtainSensorsData();
                } else if (strcmp(message, "STOP_TRACKING") == 0) {
                    Serial.println("recibio stoptracking");
                    // Recibido un mensaje para detener el seguimiento
                    sendDataToM5Stack(udpPort);
                } else {
                    // Mensaje desconocido, podrías manejarlo de alguna manera
                    Serial.println("No se reconoce la orden del mensaje JSON");
                }
            }
        } else {
            Serial.println("Error al analizar los datos JSON recibidos");
        }
    });
    }

private:
    int udpPort;
    char ssid[32];
    char pass[64];
    AsyncUDP udp;
    static const int UDP_TX_PACKET_MAX_SIZE = 200;

    TemperatureSensor* temperatureSensor;
    SoundSensor* soundSensor;
    LedLight* ledLight;

    int snoreAmount;
    int frecuenciaMinimaSonido = 600; // Numero aleatorio que hace de minimo valor de medida para los ronquidos
    int averageTemperature;
    // Para tomar la temperatura cada 5 segundos
    unsigned int previousTempMeasurementTime = 0;
    const unsigned int tempMeasurementInterval = 5000;
    unsigned int tempIndex = 0;
    // Para tomar el sonido cada 100 milisegundos
    unsigned int previousSoundMeasurementTime = 0;
    const unsigned int soundMeasurementInterval = 100;
    unsigned int soundIndex=0;
    // Son los milisegundos que separan cada medida tomada
    const unsigned int maxMeasurements = 256;
    int temperatureMeasurements[256];
    int soundMeasurements[256];
    

    ESP32Abstract(const char *ssidConstructor, const char *passConstructor, int udp, int TEMPERATUREPIN, int SOUNDPIN, int LEDPIN) {
        udpPort = udp;
        temperatureSensor = TemperatureSensor::getInstance(TEMPERATUREPIN);  // Crea una instancia del sensor de temperatura
        soundSensor = SoundSensor::getInstance(SOUNDPIN);  // Crea una instancia del sensor de sonido
        ledLight = LedLight::getInstance(LEDPIN);  // Crea una instancia del controlador de luz LED

        strncpy(ssid, ssidConstructor, sizeof(ssid) - 1);
        ssid[sizeof(ssid) - 1] = '\0';

        strncpy(pass, passConstructor, sizeof(pass) - 1);
        pass[sizeof(pass) - 1] = '\0';
        Serial.println("Wifi: ");
        Serial.print(ssid);
        Serial.print(pass);
    }

    void openUDPConnection(){
        Serial.println("openUDPConnection");
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, pass);
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

    void multipleTemperatureMeasurements() {
        int temperatura = temperatureSensor->takeMeasurement();
        temperatureMeasurements[tempIndex] = temperatura;
        tempIndex++;
        previousTempMeasurementTime = millis();
    }

    void multipleSoundMeasurements() {
        int sonido = soundSensor->takeMeasurement();
        soundMeasurements[soundIndex] = sonido;
        soundIndex++;
        previousSoundMeasurementTime = millis();
    }

    void obtainSensorsData() {
        // Implementa la obtención de datos de los sensores (temperatureSensor, soundSensor, etc.)
        unsigned long currentTime = millis();

        ledLight->turnOn();
        delay(2000);
        ledLight->turnOff();
        Serial.println("Se entro a obtainSensorsData, bucle infinito de toma de medidas");
        
        // for(;;) {
        //     if (currentTime - previousTempMeasurementTime >= tempMeasurementInterval && tempIndex < maxMeasurements) {
        //         multipleTemperatureMeasurements();
        //     }

        //     if (currentTime - previousSoundMeasurementTime >= soundMeasurementInterval && soundIndex < maxMeasurements) {
        //         multipleSoundMeasurements();
        //     }
        // }
        Serial.println("se toman medidas");
        
        // for(int i=0; i<5;i++) {
          temperatureMeasurements[0] = temperatureSensor->takeMeasurement();
          soundMeasurements[0] = soundSensor->takeMeasurement();
          // delay(1000);
          // tempIndex++;
          // soundIndex++;
        
    }

    int averageMeasurements(int *measurements, unsigned int measurementsQuantity) {
        int summary = 0;
        for(int i=measurementsQuantity; i; i--) {
            summary = summary + measurements[i];
        }
        return summary/measurementsQuantity;
    }

    // Si el valor del sonido supera un cierto umbral (por ejemplo, frecuenciaMinimaSonido), 
    // podrías considerarlo como un ronquido y aumentar el contador de ronquidos (snoreAmount).
    int snoreSummary(int* measurements, unsigned int measurementsQuantity) {
        int snores = 0;
        for(int i=measurementsQuantity; i; i--) {
            if(measurements[i] >= frecuenciaMinimaSonido) {
                snores++;
            } 
        }
        return snores;
    }

    void sendDataToM5Stack(int puerto) {
        // Implementa el envío de datos al M5Stack
        // Crear un objeto JSON para almacenar los datos
        Serial.print("Se enciende el led");
        ledLight->turnOn();
        delay(5000);
        ledLight->turnOff();
        Serial.println("esp32 denota que se hace de dia");

        StaticJsonDocument<200> jsonBuffer;
        char medidas[200];

        Serial.print("muestra de temperatura");
        Serial.println(temperatureMeasurements[0]);
        Serial.print("muestra de sonido");
        Serial.println(soundMeasurements[0]);

        // Agrega los valores directamente al objeto JSON
        jsonBuffer["averageTemperature"] = 23;
        jsonBuffer["snoreAmount"] = 10;
        //jsonBuffer["averageTemperature"] = averageMeasurements(temperatureMeasurements, tempIndex);
        //jsonBuffer["snoreAmount"] = snoreSummary(soundMeasurements, soundIndex);

        // Serializar el objeto JSON en una cadena
        serializeJson(jsonBuffer, medidas);

        Serial.println("se intentan enviar los datos al m5stack");
        // Enviar los datos por UDP al M5Stack
        udp.broadcastTo(medidas, puerto); 

        // Reiniciar las variables de las medidas
        tempIndex = 0;
        soundIndex = 0;
    }

    static ESP32Abstract* instance;
};

ESP32Abstract* ESP32Abstract::instance = nullptr;

#endif