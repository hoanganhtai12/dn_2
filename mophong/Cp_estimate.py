import numpy as np
import matplotlib.pyplot as plt

# 1. Dữ liệu SOC và Cp trích xuất từ bảng tính
# (Gán giá trị tại SOC=1 bằng SOC=0.9 và SOC=0 bằng SOC=0.1 để đủ 11 phần tử)
soc = np.array([1,0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1,0])
cp = np.array([ 8000,4831, 3646, 3031, 3253, 3255, 5374, 4773, 6862, 5811,5000])

# 2. Tìm đa thức bậc 8 xấp xỉ Cp(SOC)
coeffs_cp = np.polyfit(soc, cp, 8)
p_cp = np.poly1d(coeffs_cp)

# 3. In hệ số định dạng float array để sử dụng trong lập trình nhúng (ESP32/Arduino)
print("--- Hệ số đa thức Cp(SOC) bậc 8 ---")
coeff_str = ", ".join([f"{c:.10e}" for c in coeffs_cp])
print(f"float cp_coeffs[] = {{{coeff_str}}};")

# 4. Lưu vào file riêng 'he_so_cp.txt'
np.savetxt('he_so_cp.txt', coeffs_cp, fmt='%.10e', header='He so da thuc Cp bac 8 (Tu x^8 den hang so)')
print("\n--- Đã lưu hệ số Cp vào file 'he_so_cp.txt' ---")

# 5. Vẽ đồ thị kiểm tra độ khớp
x_range = np.linspace(0, 1, 100)
plt.figure(figsize=(8, 5))
plt.scatter(soc, cp, color='orange', label='Dữ liệu Cp thực tế')
plt.plot(x_range, p_cp(x_range), color='red', linestyle='--', label='Xấp xỉ bậc 8')
plt.title('Mối quan hệ Cp - SOC')
plt.xlabel('SOC')
plt.ylabel('Polarization Capacitance Cp (Farad)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()