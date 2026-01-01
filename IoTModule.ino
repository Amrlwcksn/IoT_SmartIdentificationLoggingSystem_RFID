#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- KONFIGURASI WIFI ---
const char* ssid = "vistar";
const char* password = "vistar66128";

// --- KONFIGURASI API ---
// IP 192.168.1.11 adalah IP laptop Anda dari ipconfig
const char* serverUrl = "http://192.168.1.6:8000/api/scan-rfid";

// --- PIN RFID (SPI) ---
#define SS_PIN    5
#define RST_PIN   22
MFRC522 rfid(SS_PIN, RST_PIN);

// --- PIN OLED (I2C) ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 25
#define OLED_SCL 26
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Inisialisasi I2C untuk OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED gagal diinisialisasi"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("SILS SYSTEM v1.0");
  display.println("Connecting WiFi...");
  display.display();

  // Koneksi WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  display.println("WiFi Connected!");
  display.display();
  delay(1000);

  // Inisialisasi RFID
  SPI.begin(); 
  rfid.PCD_Init();

  showIdleMessage();
}

void loop() {
  // Cek apakah ada kartu baru
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Ambil UID Kartu
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.println("Kartu Terdeteksi: " + uidString);
  processAttendance(uidString);

  // Hentikan pembacaan kartu saat ini
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void processAttendance(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Processing...");
    display.display();

    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Body JSON
    StaticJsonDocument<200> doc;
    doc["uid"] = uid;
    String jsonRequest;
    serializeJson(doc, jsonRequest);

    int httpResponseCode = http.POST(jsonRequest);

    if (httpResponseCode > 0) {
      String response = http.getString();
      StaticJsonDocument<500> resDoc;
      deserializeJson(resDoc, response);

      display.clearDisplay();
      display.setCursor(0, 0);

      if (httpResponseCode == 200) {
        // Berhasil Terdaftar atau Sudah Absen
        String msg = resDoc["message"];
        String name = resDoc["data"]["student"]["name"];
        
        display.setTextSize(2);
        display.println("BERHASIL!");
        display.setTextSize(1);
        display.println("---------------------");
        display.println(name); // Tampilkan nama siswa
        display.println(msg);
      } else {
        // Error (Misal: Siswa tidak ditemukan)
        display.setTextSize(2);
        display.println("GAGAL!");
        display.setTextSize(1);
        display.println("---------------------");
        display.println(resDoc["message"].as<String>());
      }
    } else {
      display.println("Server Error!");
    }
    
    display.display();
    http.end();
    delay(3000); // Tampilkan pesan selama 3 detik sebelum kembali ke idle
    showIdleMessage();
  }
}

void showIdleMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("SILS ONLINE");
  display.println("Ready to scan...");
  display.println("---------------------");
  display.println("Silakan tap kartu");
  display.display();
}