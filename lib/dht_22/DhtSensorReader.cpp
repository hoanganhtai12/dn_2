#include "DhtSensorReader.h" // Bao gồm file header của chính thư viện này

// Constructor: Khởi tạo đối tượng DHT và các biến thành viên
DhtSensorReader::DhtSensorReader(uint8_t pin, uint8_t type)
    : dht(pin, type) { // Khởi tạo dht với chân và loại đã cho
    minDelayMs = 0; // Khởi tạo giá trị
}

// Bắt đầu giao tiếp với cảm biến DHT
void DhtSensorReader::begin() {
    dht.begin(); // Gọi begin của thư viện DHT_Unified

    // Lấy độ trễ tối thiểu giữa các lần đọc từ cảm biến
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    minDelayMs = sensor.min_delay / 1000; // Chuyển từ micro giây sang mili giây

    // Đảm bảo độ trễ tối thiểu là 2 giây cho DHT22
    if (minDelayMs < 2000) {
        minDelayMs = 2000;
    }
}

// Đọc nhiệt độ
float DhtSensorReader::readTemperature() {
    sensors_event_t event;
    dht.temperature().getEvent(&event); // Đọc sự kiện nhiệt độ
    if (isnan(event.temperature)) {
        return NAN; // Trả về NAN nếu có lỗi
    }
    return event.temperature;
}

// Đọc độ ẩm
float DhtSensorReader::readHumidity() {
    sensors_event_t event;
    dht.humidity().getEvent(&event); // Đọc sự kiện độ ẩm
    if (isnan(event.relative_humidity)) { // Nhớ dùng .relative_humidity
        return NAN; // Trả về NAN nếu có lỗi
    }
    return event.relative_humidity;
}

// Trả về độ trễ tối thiểu
uint32_t DhtSensorReader::getMinDelayMs() {
    return minDelayMs;
}