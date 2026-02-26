#include <LCD.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include "UIA8060.h"

// --- Cấu hình phần cứng ---
LCD TFT(80, 160);
uint16_t *BgrSprite;
#define LCD_LED 39
#define I2C_SDA 8
#define I2C_SCL 18
#define CHARGE_PIN 17  // LOW = Bật, HIGH = Tắt
#define LOAD_PIN 16    // HIGH = Bật
#define BOOT_BUTTON 0  
Adafruit_INA219 ina219(0x40);

// --- Tham số Pin & Hệ số đa thức bậc 8 ---
const float Ccb = 1.2 * 3600.0; 
const float V_CUTOFF = 2.7;

float ocv_coeffs[] = {-346.321533, 1511.260688, -2742.85593, 2690.55734, -1551.36387, 534.571454, -106.552712, 11.571511, 3.272998};
float r0_coeffs[]  = {-105.441338, 409.242533, -640.681395, 516.828652, -226.792802, 51.775075, -4.921223, -0.036914, 0.132014};
float rp_coeffs[]  = {562.227183, -2518.47514, 4715.92443, -4784.94629, 2851.14336, -1011.33033, 206.381763, -21.863367, 0.990126};
float cp_coeffs[]  = {10257675.44, -41364268.52, 68383786.98, -59606617.22, 29249346.93, -7892776.31, 1013057.03, -37187.58, 4989.27};

// --- Biến trạng thái EKF & Dung lượng ---
float Xk[2]; 
float Pk[2][2] = {{1e-2, 0}, {0, 1e-2}};
float Q[2][2]  = {{1e-6, 0}, {0, 1e-5}}; 
float R_noise  = 0.01;
float soc_coulomb = 0.0;
unsigned long last_time;

volatile float total_mah = 0.0; 
volatile float d_Vmeas, d_Vp, d_IA, d_SoCK, d_SoCC;
volatile bool is_charging = true; 

// --- Hàm toán học ---
float evaluatePolynomial(float coeffs[], int degree, float x) {
    float result = coeffs[0];
    for (int i = 1; i <= degree; i++) result = result * x + coeffs[i];
    return result;
}

float get_dOCV_dSOC(float x) {
    float d_coeffs[] = {-2770.57226, 10578.8248, -16457.1356, 13454.2294, -6205.45548, 1603.71436, -213.10542, 11.57151};
    return max(evaluatePolynomial(d_coeffs, 7, x), 0.01f);
}

float lookup_soc_from_ocv(float v_in) {
    float segments[][4] = {
        {3.2729, 3.7692, 0.201491, -0.659460}, {3.7692, 3.8204, 1.953125, -7.261719},
        {3.8204, 3.8748, 1.838235, -6.822794}, {3.8748, 3.9084, 2.976190, -11.232143},
        {3.9084, 3.9644, 1.785714, -6.579286}, {3.9644, 4.0016, 2.688172, -10.156989},
        {4.0016, 4.0388, 2.688172, -10.156989}, {4.0388, 4.0492, 9.615385, -38.134615},
        {4.0492, 4.0644, 6.578947, -25.839474}, {4.0644, 4.1400, 1.322751, -4.476190}
    };
    if (v_in <= segments[0][0]) return 0.0;
    if (v_in >= segments[9][1]) return 1.0;
    for (int i = 0; i < 10; i++) {
        if (v_in >= segments[i][0] && v_in < segments[i][1]) 
            return constrain(segments[i][2] * v_in + segments[i][3], 0.0, 1.0);
    }
    return 0.5;
}

// --- Task hiển thị LCD ---
void LCDTask(void* Para) {
    TickType_t GetTick = xTaskGetTickCount();
    const int rowH = 160 / 6;
    while(1) {
        TFT.FillScreen(TFT_BLACK);
        TFT.DrawRect(0, 0, 80, 160, 1, TFT_BLUEVIOLET);
        for (int i = 0; i < 6; i++) {
            int y = i * rowH;
            TFT.SetStrings(4, y + 6, TFT_GREEN, CHAR_16x8);
            switch(i) {
                case 0: TFT.Printf("V:%.3f", d_Vmeas); break;
                case 1: TFT.SetString(TFT_CYAN); TFT.Printf("I:%.3f", d_IA); break;
                case 2: TFT.SetString(TFT_YELLOW); TFT.Printf("K:%0.1f%%", d_SoCK * 100.0); break;
                case 3: TFT.SetString(TFT_ORANGE); TFT.Printf("C:%0.1f%%", d_SoCC * 100.0); break;
                case 4: 
                    TFT.SetString(is_charging ? TFT_GREEN : TFT_RED);
                    TFT.Printf("ST:%s", is_charging ? "CHG" : "DIS"); 
                    break;
                case 5: TFT.SetString(TFT_MAGENTA); TFT.Printf("mAh:%.1f", total_mah); break;
            }
        }
        TFT.PushSprite(0xFFFF);
        vTaskDelayUntil(&GetTick, 200 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LOAD_PIN, OUTPUT); digitalWrite(LOAD_PIN, HIGH);
    pinMode(CHARGE_PIN, OUTPUT); digitalWrite(CHARGE_PIN, HIGH); 
    pinMode(BOOT_BUTTON, INPUT_PULLUP);
    pinMode(LCD_LED, OUTPUT); analogWrite(LCD_LED, 1000);

    Wire.begin(I2C_SDA, I2C_SCL);
    if (!ina219.begin()) {
        Serial.println("Failed to find INA219 chip");
    }

    BgrSprite = new uint16_t[80 * 160];
    TFT.Config(BgrSprite, 40, 42, 41, 1, 1, 2);
    TFT.Init(&SPI);
    TFT.Setting();
    TFT.CreateSprite(0, 0, 80, 160);

    delay(1000);
    float V0 = ina219.getBusVoltage_V();
    float I0 = (ina219.getCurrent_mA() / 1000.0); 
    float soc_init_guess = lookup_soc_from_ocv(V0);
    float v_ocv_start = V0 - I0 * evaluatePolynomial(r0_coeffs, 8, soc_init_guess);
    
    Xk[0] = lookup_soc_from_ocv(v_ocv_start);
    Xk[1] = 0.0;
    soc_coulomb =Xk[0] ;
    last_time = millis();

    xTaskCreatePinnedToCore(LCDTask, "LCDTask", 10000, NULL, 3, NULL, 0);
}

void loop() {
    // --- Xử lý nút bấm ---
    if (digitalRead(BOOT_BUTTON) == LOW) {
        delay(50);
        if (digitalRead(BOOT_BUTTON) == LOW) {
            is_charging = !is_charging;
            digitalWrite(CHARGE_PIN, is_charging ? LOW : HIGH); 
            while(digitalRead(BOOT_BUTTON) == LOW); 
        }
    }

    unsigned long now = millis();
    float dt = (now - last_time) / 1000.0;
    if (dt < 0.1) return;
    last_time = now;

    // --- Đọc cảm biến ---
    float V_meas = ina219.getBusVoltage_V();
    float I_mA = ina219.getCurrent_mA(); 
    float I_k = I_mA / 1000.0; 

    // --- 1. Tính toán mAh ---
    total_mah += (I_mA * dt) / 3600.0;

    // --- 2. Coulomb Counting ---
    soc_coulomb += (I_k * dt) / Ccb;
    soc_coulomb = constrain(soc_coulomb, 0.0, 1.0);

    // --- 3. EKF Prediction & Update ---
    float r0 = evaluatePolynomial(r0_coeffs, 8, Xk[0]);
    float rp = evaluatePolynomial(rp_coeffs, 8, Xk[0]);
    float cp = evaluatePolynomial(cp_coeffs, 8, Xk[0]);
    float dOCV = get_dOCV_dSOC(Xk[0]);
    float exp_t = exp(-dt / max(rp * cp, 1e-3f));

    float X_p[2] = { Xk[0] + (dt / Ccb) * I_k, Xk[1] * exp_t - rp * (1.0f - exp_t) * I_k };
    float P_p[2][2] = { {Pk[0][0] + Q[0][0], Pk[0][1] * exp_t}, {Pk[1][0] * exp_t, exp_t * Pk[1][1] * exp_t + Q[1][1]} };
    
    float V_p = evaluatePolynomial(ocv_coeffs, 8, X_p[0]) - X_p[1] + I_k * r0;
    float err = V_meas - V_p;
    
    float H[2] = {dOCV, -1.0}; 
    float S = H[0]*P_p[0][0]*H[0] + H[0]*P_p[1][0]*H[1] + H[1]*P_p[0][1]*H[0] + H[1]*P_p[1][1]*H[1] + R_noise;
    float K[2] = {(P_p[0][0]*H[0] + P_p[0][1]*H[1])/S, (P_p[1][0]*H[0] + P_p[1][1]*H[1])/S};

    Xk[0] = constrain(X_p[0] + K[0]*err, 0.0, 1.0);
    Xk[1] = X_p[1] + K[1]*err;

    float IKH00 = 1.0f-K[0]*H[0], IKH01 = -K[0]*H[1], IKH10 = -K[1]*H[0], IKH11 = 1.0f-K[1]*H[1];
    Pk[0][0] = IKH00*P_p[0][0] + IKH01*P_p[1][0];
    Pk[0][1] = IKH00*P_p[0][1] + IKH01*P_p[1][1];
    Pk[1][0] = IKH10*P_p[0][0] + IKH11*P_p[1][0];
    Pk[1][1] = IKH10*P_p[0][1] + IKH11*P_p[1][1];

    // Cập nhật biến hiển thị
    d_Vmeas = V_meas; d_Vp = V_p; d_IA = I_k; d_SoCK = Xk[0]; d_SoCC = soc_coulomb;

    // --- 4. IN RA MONITOR ---
    Serial.print("V:");         Serial.print(V_meas, 3);      Serial.print(" | ");
    Serial.print("I:");         Serial.print(I_k, 3);         Serial.print(" | ");
    Serial.print("Soc_k:");     Serial.print(Xk[0] * 100.0, 1); Serial.print("% | ");
    Serial.print("Soc_c:");     Serial.print(soc_coulomb * 100.0, 1); Serial.print("% | ");
    Serial.print("state:");     Serial.print(is_charging ? "CHG" : "DIS");
    Serial.println();

    // Bảo vệ pin yếu
    if(V_meas < V_CUTOFF) {
        digitalWrite(LOAD_PIN, LOW);
    };
     if(V_meas > 4.0) {
        digitalWrite(LOAD_PIN, 1);
    };
    
    delay(1000); // Tần số cập nhật 2Hz để Monitor dễ đọc
}