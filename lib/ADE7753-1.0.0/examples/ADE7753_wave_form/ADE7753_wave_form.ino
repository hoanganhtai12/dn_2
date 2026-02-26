// ESP32-1 (Sender)
#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "ESP32_AP";
const char *password = "12345678";
WiFiUDP udp;

const char *targetIP = "192.168.4.2"; // ESP32-2 IP address
const int targetPort = 4210;

void setup() {
  Serial.begin(115200);

  // Set WiFi to AP mode
  WiFi.softAP(ssid, password);
  delay(1000);

  // Set TX power to 0 dBm
  esp_wifi_set_max_tx_power(8); // 0 dBm = 8 * 0.25 dBm unit

  Serial.println("WiFi AP Started");
  Serial.println(WiFi.softAPIP());
  delay(5000); // Đợi ESP32-2 kết nối
}

void loop() {
  Serial.println("Start sending 1000 UDP packets");
  for (int i = 0; i < 1000; i++) {
    udp.beginPacket(targetIP, targetPort);
    udp.printf("Packet #%d", i);
    udp.endPacket();
    delay(10); // 10ms giữa các packet
  }
  Serial.println("Done sending 1000 packets");
  while (1); // Dừng lại
}
