import numpy as np
import matplotlib.pyplot as plt

# 1. Dữ liệu SOC và Rp trích xuất từ bảng tính
# (Lưu ý: Điền giá trị tại SOC=1 bằng SOC=0.9 và SOC=0 bằng SOC=0.1 để đủ mảng)
soc = np.array([1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1])
rp = np.array([0.051745, 0.071745, 0.095, 0.107, 0.1226, 0.11, 0.0917, 0.09, 0.08385, 0.098])

# 2. Tìm đa thức bậc 8 xấp xỉ Rp(SOC)
coeffs_rp = np.polyfit(soc, rp, 8)
p_rp = np.poly1d(coeffs_rp)

# 3. In hệ số định dạng float array cho ESP32
print("--- Hệ số đa thức Rp(SOC) bậc 8 ---")
coeff_str = ", ".join([f"{c:.10e}" for c in coeffs_rp])
print(f"float rp_coeffs[] = {{{coeff_str}}};")

# 4. Lưu vào file 'he_so_rp.txt'
np.savetxt('he_so_rp.txt', coeffs_rp, fmt='%.10e', header='He so da thuc Rp bac 8 (Tu x^8 den hang so)')
print("\n--- Đã lưu hệ số Rp vào file 'he_so_rp.txt' ---")

# 5. Vẽ đồ thị kiểm tra
x_range = np.linspace(0, 1, 100)
plt.figure(figsize=(8, 5))
plt.scatter(soc, rp, color='blue', label='Dữ liệu Rp thực tế')
plt.plot(x_range, p_rp(x_range), color='darkblue', linestyle='--', label='Xấp xỉ bậc 8')
plt.title('Mối quan hệ Rp - SOC')
plt.xlabel('SOC')
plt.ylabel('Polarization Resistance Rp (Ohm)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()