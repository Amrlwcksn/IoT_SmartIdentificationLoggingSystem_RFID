# SILS (Smart Identification Logging System) v1.0

SILS adalah sistem logging identifikasi berbasis ESP32 menggunakan teknologi RFID. Perangkat ini berfungsi untuk melakukan pemindaian kartu RFID dan mengirimkan data UID ke server pusat melalui API untuk keperluan absensi atau identifikasi siswa secara real-time.

## 🚀 Fitur Utama
- **Konektivitas WiFi**: Menghubungkan perangkat ke jaringan lokal untuk komunikasi data.
- **Identifikasi RFID**: Membaca UID kartu RFID (13.56 MHz).
- **Integrasi API**: Mengirim data ke backend Laravel melalui endpoint REST API.
- **Display Status**: Menampilkan status koneksi, hasil pemindaian, dan nama siswa pada layar OLED.
- **Feedback Real-time**: Menampilkan pesan "BERHASIL" atau "GAGAL" berdasarkan respon server.

## 🛠️ Kebutuhan Perangkat Keras
1. **ESP32 Development Board**
2. **RFID Reader RC522**
3. **OLED Display 1306** (128x64, I2C)
4. **Kabel Jumper & Breadboard**

## 📌 Konfigurasi Pin
### RFID RC522 (SPI)
- **SS (SDA)**: GPIO 5
- **SCK**: GPIO 18
- **MOSI**: GPIO 23
- **MISO**: GPIO 19
- **RST**: GPIO 22

### OLED Display (I2C)
- **SDA**: GPIO 25
- **SCL**: GPIO 26

## 💻 Kebutuhan Perangkat Lunak
Pastikan pustaka (libraries) berikut sudah terinstall di Arduino IDE:
- `WiFi.h` (Built-in ESP32)
- `HTTPClient.h` (Built-in ESP32)
- `ArduinoJson` by Benoit Blanchon
- `MFRC522` by GithubCommunity
- `Adafruit GFX Library` by Adafruit
- `Adafruit SSD1306` by Adafruit

## ⚙️ Cara Penggunaan
1. **Instalasi Library**: Buka *Library Manager* di Arduino IDE dan cari pustaka yang disebutkan di atas.
2. **Konfigurasi WiFi**: Ubah variabel `ssid` dan `password` pada file `.ino` sesuai dengan jaringan Anda.
   ```cpp
   const char* ssid = "NAMA_WIFI";
   const char* password = "PASSWORD_WIFI";
   ```
3. **Konfigurasi API**: Sesuaikan `serverUrl` dengan alamat IP server/laptop Anda.
   ```cpp
   const char* serverUrl = "http://IP_ADDRESS:8000/api/scan-rfid";
   ```
4. **Flash Program**: Hubungkan ESP32 ke komputer dan klik **Upload**.
5. **Monitor**: Buka *Serial Monitor* dengan baud rate `115200` untuk melihat status debug.

## 📝 Catatan
Pastikan server backend (Laravel/Lumen) sudah berjalan dan endpoint `/api/scan-rfid` siap menerima request POST dengan body JSON:
```json
{
  "uid": "XXXXXXXX"
}
```
