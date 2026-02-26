#ifndef DHT_SENSOR_READER_H
#define DHT_SENSOR_READER_H

#include "Arduino.h"      // Cần thiết cho các hàm như Serial, millis
#include "DHT.h"          // Thư viện DHT sensor
#include "DHT_U.h"        // Thư viện Adafruit Unified Sensor
#include <Adafruit_Sensor.h> // Đảm bảo include này để sử dụng sensors_event_t

class DhtSensorReader {
public:
    // Constructor: Khởi tạo cảm biến với chân và loại DHT
    DhtSensorReader(uint8_t pin, uint8_t type);

    // Bắt đầu giao tiếp với cảm biến DHT
    void begin();

    // Đọc nhiệt độ từ cảm biến
    float readTemperature();

    // Đọc độ ẩm từ cảm biến
    float readHumidity();

    // Lấy độ trễ tối thiểu giữa các lần đọc của cảm biến (ms)
    uint32_t getMinDelayMs();

private:
    DHT_Unified dht;        // Đối tượng DHT_Unified
    uint32_t minDelayMs;    // Biến lưu độ trễ tối thiểu
};

#endif // DHT_SENSOR_READER_H