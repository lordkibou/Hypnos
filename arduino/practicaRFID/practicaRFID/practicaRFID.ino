//instalar libreria de: https://github.com/miguelbalboa/rfid 

/*
······GPIO MAP······

RC522 - M5STACK

SDA - 21(SDA)
SCK - 18(SCK)
MOSI - 23(MO)
GND - G
MISO - 19(MI)
3.3V - 3V3

LED RGB - M5STACK
R - 17
G - 16
B - 3
*/


#include <SPI.h>
#include <MFRC522.h>
#include <M5Stack.h>

#define SDA_PIN 21
#define RST_PIN 2
#define LED_R 17
#define LED_G 16
#define LED_B 3

unsigned char data[12] = {'H', 'y', 'p', 'n', 'o', 's', 'A', 'c', 't', 'i', 'v', 'o'};
unsigned char badData[12] = {'A', 'c', 'c', 'e', 's', 's', 'D', 'e', 'n', 'i', 'e', 'd'};

unsigned char *writeData = data;
unsigned char *writeDeniedData = badData;
unsigned char *str;
unsigned long lastTime = 0;  // Variable para almacenar el tiempo de la última ejecución
bool previousWriteToCardState = true;  // Variable para almacenar el estado anterior de writeToCard para controlar cuando sale el mensaje en pantalla del M5Stack.
bool previousWriteToCardStateBAD = true;
bool writeToCard = false; //Variable para señalar si se va a escribir en la tag
bool writeBadDataToCard = false;

MFRC522 mfrc522(SDA_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

//------------------------------------------------------------------
/*
                          setLEDColor()
*/
//------------------------------------------------------------------
void setLEDColor(int red, int green, int blue) {
  analogWrite(LED_R, red);
  analogWrite(LED_G, green);
  analogWrite(LED_B, blue);
}

//------------------------------------------------------------------
/*
                          printArray()
*/
//------------------------------------------------------------------
void printArray(byte *buffer, byte bufferSize) { //para imprimir lo que leemos y escribimos
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print((char)buffer[i]);
  }
}

//------------------------------------------------------------------
/*
                        authenticateCard()
*/
//------------------------------------------------------------------
void authenticateCard() {
  MFRC522::StatusCode status;
  byte trailerBlock = 7;

  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) {

    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));

    return;
  } 
  else{
    Serial.println();
    Serial.println("Autenticación completada.");
  }
}

//------------------------------------------------------------------
/*
                        writeDataToCard()
*/
//------------------------------------------------------------------
void writeDataToCard(byte blockAddr) {
  Serial.print(F("Escribir datos en sector "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));


  printArray((byte *)data, 12);
  Serial.println();
  MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, (byte *)data, 16);

  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }else{
    M5.Lcd.setCursor(0,100);
    M5.Lcd.println("¡Escritura completada!");
    delay(1000);
  }

}

//------------------------------------------------------------------
/*
                        writeDeniedDataToCard()
*/
//------------------------------------------------------------------
void writeDeniedDataToCard(byte blockAddr) {
  Serial.print(F("Escribir datos para no permitir acceso en sector "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));


  printArray((byte *)badData, 12);
  Serial.println();
  MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, (byte *)badData, 16);

  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }else{
    M5.Lcd.setCursor(0,100);
    M5.Lcd.println("¡Escritura completada!");
    delay(1000);
  }

}

//------------------------------------------------------------------
/*
                        readDataFromCard()
*/
//------------------------------------------------------------------
void readDataFromCard(byte blockAddr) {
  byte buffer[18];
  byte size = sizeof(buffer);

  Serial.print(F("Leer datos del sector "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));

  MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }

  Serial.print(F("Data in block "));
  Serial.print(blockAddr);
  Serial.println(F(":"));
  printArray(buffer, 12);

  verifyCode(buffer, 12);
}

//------------------------------------------------------------------
/*
                        verifyCode()
*/
//------------------------------------------------------------------
void verifyCode(byte *data, byte dataSize) {

  // Convertir los datos leídos a una cadena
  char buffer[dataSize + 1];
  memcpy(buffer, data, dataSize);
  buffer[dataSize] = '\0'; // Agregar el carácter nulo al final para formar una cadena de caracteres

  if (strcmp(buffer, "HypnosActivo") == 0) {
    setLEDColor(0, 255, 0);  // Verde
    previousWriteToCardState = true;
    M5.Speaker.tone(1300, 200); // Frecuencia y duración del tono
    delay(140);
  } else {
    setLEDColor(255, 0, 0);  // Rojo
    previousWriteToCardState = true;
    M5.Speaker.tone(200, 200); // Frecuencia y duración del tono
    delay(140);
  }
  setLEDColor(0, 0, 0);  // Apagar el LED después de 2 segundos
  M5.Lcd.fillScreen(TFT_BLACK);
/*

        CODIGO SIMULANDO LED CON M5STACK (PARA PROBAR CÓDIGO SIN LEDS)


  // Comparar con "HypnosActivo"
  if (strcmp(buffer, "HypnosActivo") == 0) {

    // Mostrar círculo verde en la pantalla del M5Stack durante 2 segundos
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.fillCircle(160, 120, 50, TFT_GREEN);

    previousWriteToCardState = true; //para que vuelva a salir el mensaje de "Acerca la tarjeta para leer"

    delay(2000);
    M5.Lcd.fillScreen(TFT_BLACK);

  } else {
    // Mostrar círculo rojo en la pantalla del M5Stack durante 2 segundos
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.fillCircle(160, 120, 50, TFT_RED);

    previousWriteToCardState = true; 

    delay(2000);
    M5.Lcd.fillScreen(TFT_BLACK);
  }
  */
}

//------------------------------------------------------------------
/*
                      ****** SETUP() ******
*/
//------------------------------------------------------------------
void setup() {
  M5.begin();
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0, 0);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

//------------------------------------------------------------------
/*
                      ****** LOOP() ******
*/
//------------------------------------------------------------------
void loop() {
  M5.update(); // Actualizar el estado del botón

  delay(50);

  if (M5.BtnA.wasPressed()) {
    writeToCard = true; // Indicar que se debe escribir en la tarjeta
    Serial.println();
    Serial.println("Se ha pulsado el boton para escribir");
    Serial.println();
  }

  if(M5.BtnC.wasPressed()){
    writeBadDataToCard = true;
    Serial.println();
    Serial.println("Se ha pulsado el boton para escribir datos para denegar acceso");
    Serial.println();
  }

  unsigned long currentTime = millis();

  // Realizar acciones solo si ha pasado al menos 1 segundo desde la última ejecución
  if (currentTime - lastTime >= 1000) {
    lastTime = currentTime;  // Actualizar el tiempo de la última ejecución


    
    // Verificar si el estado de writeToCard ha cambiado
    if (writeToCard != previousWriteToCardState) {
      previousWriteToCardState = writeToCard;  // Actualizar el estado anterior
      if (writeToCard) {
        
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.print("Acerca la tarjeta para escribir");
        
      } else {
        
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.print("Acerca la tarjeta para leer o pulsa el boton A para escribir");
      }
    }


    // Verificar si el estado de writeBadDataToCard ha cambiado
    if (writeBadDataToCard != previousWriteToCardStateBAD) {
      previousWriteToCardStateBAD = writeBadDataToCard;  // Actualizar el estado anterior
      if (writeBadDataToCard) {
        
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.print("Acerca la tarjeta para escribir el codigo para denegar acceso");
        
      } else {
        
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.print("Acerca la tarjeta para leer o pulsa el boton A para escribir");
      }
    }


    if (!mfrc522.PICC_IsNewCardPresent())
      return;

    if (!mfrc522.PICC_ReadCardSerial())
      return;

    Serial.println();
    Serial.println("¡Tarjeta detectada! Autenticando...");
    
    authenticateCard();
    
    byte blockAddr = 4;

    if (writeToCard) {
      Serial.println();
      Serial.println("Se ha llamado a escribir");
      writeDataToCard(blockAddr);
      writeToCard = false; // Restablecer la variable después de escribir
    }
    if(writeBadDataToCard){
      Serial.println();
      Serial.println("Se ha llamado a escribir datos de acceso denegado");
      writeDeniedDataToCard(blockAddr);
      writeBadDataToCard = false; // Restablecer la variable después de escribir
    }
    
    else {
      Serial.println();
      Serial.println("Se ha llamado a leer");
      readDataFromCard(blockAddr);
    }

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
  }
}