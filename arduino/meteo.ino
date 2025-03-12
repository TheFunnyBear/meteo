#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <DS3231.h>

#include <Ethernet.h>
#include <ArduinoJson.h>  
#include <SD.h>

#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей
Adafruit_BMP280 bmp; 
RTClib rtc;

// MAC адрес Ethernet Shield (можно найти на устройстве или задать вручную)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const int chipSelect = 4; // Пин для SD карты

// Данные сервера
char serverAddress[40];
char serialNumber[8];
int serverPort;
char apiEndpoint[50];


// Инициализация Ethernet клиента
EthernetClient client;


// Настройки
const char* settingsFile = "settings.jsn";

File logFile;

void showLcdMessage(String lcdMessage = "", bool firstStringOnly = false) {
  if (firstStringOnly) {
    lcd.clear();
    lcd.setCursor(0, 0);    
    lcd.print(lcdMessage);
  } else {
    // Чтение данных с BMP280
    float temperature = bmp.readTemperature();  // Температура в градусах Цельсия
    float pressure = bmp.readPressure() * 0.7500637554192 /100;  // Давление 
    float altitude = bmp.readAltitude(1013.25);  // Высота над уровнем моря (в метрах)  
    DateTime now = rtc.now();
    String dateNow = String(now.day()) + "." + String(now.month()) + "." + String(now.year());
  
    lcd.clear();
    String defaultMessage = "TEMA>" + dateNow;
    lcd.setCursor(0, 0);
    if (lcdMessage == "") {
      lcd.print(defaultMessage);
    } else {
      lcd.print(lcdMessage);
    }
    
    String lcdMessage1 =  "T=" + String(temperature) + " P=" + String(pressure);
    lcd.setCursor(0, 1);
    lcd.print(lcdMessage1);
  }
}


void setup() {
  initLcd();
  initPreasureAndTempSensor();
 
  showLcdMessage();
  
  initEthernet(); 

  initSdCard();
  loadSettings(); // Загрузка настроек из файла
}

void loop() {
  
  static unsigned long lastRequestTime = 0;
  unsigned long currentMillis = millis();

  //Отправляем температуру 1 раз в 5 минут.
  if (currentMillis - lastRequestTime >= 300000) { 
    lastRequestTime = currentMillis;
    
    sendPostRequest();  // Отправка POST запроса
  }
}


void initPreasureAndTempSensor() {
  unsigned status;
  status = bmp.begin();
  if (!status) {
    showLcdMessage(String("Temp sensor failed"), true);
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */    
}

void initLcd() {
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
}

void initEthernet() {
  //Ожидаем старта сети
  delay(1000);
  
   // Инициализируем Ethernet
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    showLcdMessage(String("Failed DHCP"));
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      showLcdMessage(String("Ethernet not found"));
    } else if (Ethernet.linkStatus() == LinkOFF) {
      showLcdMessage(String("Ethernet cable"));
    }

    while (true) {
      delay(1);
    }
  }

  showLcdMessage(String("IP: Ok"));
}

void initSdCard(){
  if (!SD.begin(chipSelect)) {
    showLcdMessage(String("SD card failed"), false);
    while (1);
  }
}


// Функция для чтения настроек
void loadSettings() {
  File settings = SD.open(settingsFile);
      
  DynamicJsonDocument doc(1024);
  if (!settings) {
    // Если файл не найден, создадим с настройками по умолчанию
    doc["serverAddress"] = "192.168.87.29";
    doc["serialNumber"] = "333";
    doc["serverPort"] = 8090;
    doc["apiEndpoint"] = "/api/reading";

    File newSettings = SD.open(settingsFile, FILE_WRITE);
    serializeJson(doc, newSettings);
    newSettings.close();
  } else {
    deserializeJson(doc, settings);
  }

  strcpy(serverAddress, doc["serverAddress"]);
  serverPort = doc["serverPort"];
  strcpy(serialNumber, doc["serialNumber"]);
  strcpy(apiEndpoint, doc["apiEndpoint"]);


  if (settings) {
    settings.close();
  }
}


void sendPostRequest() {
  OpenLogFile();
  
  // Подключаемся к серверу
  if (client.connect(serverAddress, serverPort)) {
  // Чтение данных с BMP280
  float temperature = bmp.readTemperature();  // Температура в градусах Цельсия
  float pressure = bmp.readPressure() * 0.7500637554192 / 100;  // Давление
  float altitude = bmp.readAltitude(1013.25);  // Высота над уровнем моря (в метрах)
  DateTime now = rtc.now();
  String dateNow = String(String(now.hour()) + ":" +  String(now.minute()));

  char timeBuf[22];
   
  //Updated now.day to now.date
  sprintf(timeBuf, "%02d.%02d.%04d %02d:%02d:%02d",  now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second()); 
  String timeStamp = String(timeBuf);

  
  // Создание JSON-объекта для отправки
  StaticJsonDocument<200> doc;
  doc["serialNumber"] = serialNumber;
  doc["time"] = timeStamp;
  doc["temperature"] = temperature;
  doc["pressure"] = pressure;
  doc["altitude"] = altitude;

  
  // Сериализация JSON в строку
  String jsonData;
  serializeJson(doc, jsonData);
    // Заголовки HTTP-запроса
    client.println("POST " + String(apiEndpoint) + " HTTP/1.1");  
    client.println("Host: " + String(serverAddress));  
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonData.length());  // Указываем длину JSON-данных
    
    client.println();  // Пустая строка перед телом запроса
    client.print(jsonData);  // Отправляем JSON-строку
    
    // Чтение ответа от сервера (необязательно)
    while (client.available()) {
      char c = client.read();
    }
    
    // Закрываем соединение с сервером
    client.stop();


    String lcdMessage = "TEMA> " + dateNow;
    showLcdMessage(lcdMessage);

    String logEntry = dateNow + ", " +
                        String(temperature) + ", " +
                        String(pressure) + ", " +
                        String(altitude) + "\n";
    logFile.println(logEntry);
  } else {
    showLcdMessage(String("Can't post to server"));
    logFile.println("Can't post to server");
  }
     
  logFile.flush(); 
  CloseLogFile();
}


void CloseLogFile() {
  logFile.close();
}

void OpenLogFile() {
  // Открытие или создание файла для записи
  DateTime now = rtc.now();
  String logFilename = String(now.year()- 2000) + "_" + String(now.month()) + "_" + String(now.day()) + ".log";
  bool fileExist = SD.exists(logFilename.c_str());
  logFile = SD.open(logFilename.c_str(), FILE_WRITE);
  if (!logFile) {
    showLcdMessage(String("Log error:") + String(logFilename));
    while (1);
  }

  if(!fileExist){
    // Заголовок для файла лога
    logFile.println("DateTime, Temperature, Pressure, Altitude");
  }
}
