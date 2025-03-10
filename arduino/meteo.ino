#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <DS3231.h>

#include <Ethernet.h>
#include <ArduinoJson.h>  
#include <SD.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C
RTClib rtc;

// MAC адрес Ethernet Shield (можно найти на устройстве или задать вручную)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const int chipSelect = 4; // Пин для SD карты

// Данные сервера
char serverAddress[40];
char serialNumber[8];
int serverPort;
char apiEndpoint[50];
int measurementsCount;        // Количество измерений для усреднения
int delayBetweenMeasurements; // Задержка между измерениями в секундах


// Инициализация Ethernet клиента
EthernetClient client;


// Настройки
const char* settingsFile = "settings.jsn";

File logFile;

void setup() {
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */  


  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.clear();
  
  
  lcd.setCursor(0, 0);
  lcd.print("Start init...");
  
  //SD.begin(chipSelect);
  initSdCard();
  loadSettings(); // Загрузка настроек из файла


  // Инициализируем Ethernet
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");

  if (Ethernet.begin(mac) == 0) {

    Serial.println("Failed to configure Ethernet using DHCP");

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {

      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");

    } else if (Ethernet.linkStatus() == LinkOFF) {

      Serial.println("Ethernet cable is not connected.");

    }

    // no point in carrying on, so do nothing forevermore:

    while (true) {

      delay(1);

    }

  }

  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  
  
  
  
 // Ethernet.begin(mac, ip);
  
  // Ждем, пока подключится к сети
  delay(1000);
  
  Serial.println("Ethernet подключен!");

  lcd.setCursor(0, 0);
  lcd.print("Init compleate...");

  showTempOnStart();
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

void initSdCard(){
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}


// Функция для чтения настроек
void loadSettings() {
  File settings = SD.open(settingsFile);
      
  DynamicJsonDocument doc(1024);
  if (!settings) {
    Serial.println("No settings file found, creating with default values.");
    // Если файл не найден, создадим с настройками по умолчанию
    doc["serverAddress"] = "192.168.87.29";
    doc["serialNumber"] = "333";
    doc["serverPort"] = 8090;
    doc["apiEndpoint"] = "/api/reading";
    doc["measurementsCount"] = 10;          // Количество измерений
    doc["delayBetweenMeasurements"] = 5;    // Задержка между измерениями в секундах
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
  measurementsCount = doc["measurementsCount"];
  delayBetweenMeasurements = doc["delayBetweenMeasurements"];

  if (settings) {
    settings.close();
  }
}

void showTempOnStart() {
  // Чтение данных с BMP280
  float temperature = bmp.readTemperature();  // Температура в градусах Цельсия
  float pressure = bmp.readPressure() * 0.7500637554192 /100;  // Давление 
  float altitude = bmp.readAltitude(1013.25);  // Высота над уровнем моря (в метрах)  
  DateTime now = rtc.now();
  String dateNow = String(now.day()) + "." + String(now.month()) + "." + String(now.year());

  lcd.clear();
  String lcdMessage = "TEMA>" + dateNow;
  lcd.setCursor(0, 0);
  lcd.print(lcdMessage);
  
  String lcdMessage1 =  "T=" + String(temperature) + " P=" + String(pressure);
  lcd.setCursor(0, 1);
  lcd.print(lcdMessage1);
}

void sendPostRequest() {
  OpenLogFile();
  
  // Подключаемся к серверу
  if (client.connect(serverAddress, serverPort)) {
    Serial.println("Соединение с сервером установлено!");
  
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
    
    Serial.println("GET запрос отправлен!");

    // Чтение ответа от сервера (необязательно)
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    
    // Закрываем соединение с сервером
    client.stop();


    lcd.clear();
      String lcdMessage = "TEMA> " + dateNow;
      // Устанавливаем курсор на вторую строку и нулевой символ.
     lcd.setCursor(0, 0);
      // Выводим на экран количество секунд с момента запуска ардуины
     lcd.print(lcdMessage);
     
     String lcdMessage1 =  "T=" + String(temperature) + " P=" + String(pressure);
     lcd.setCursor(0, 1);
     lcd.print(lcdMessage1);

    String logEntry = dateNow + ", " +
                        String(temperature) + ", " +
                        String(pressure) + ", " +
                        String(altitude) + "\n";
      logFile.println(logEntry);

    
    Serial.println("Соединение закрыто.");
    logFile.println("Соединение закрыто.");
    
  } else {
    Serial.println("Не удалось подключиться к серверу.");
    logFile.println("Не удалось подключиться к серверу.");
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
    Serial.println("Error opening log file:" +  logFilename);
    while (1);
  }

  if(!fileExist){
    // Заголовок для файла лога
    logFile.println("DateTime, Temperature, Pressure, Altitude");
  }
}
