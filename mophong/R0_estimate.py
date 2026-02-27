import numpy as np
import matplotlib.pyplot as plt

# 1. Dữ liệu SOC và Ro trích xuất từ bảng tính của bạn
soc = np.array([1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0])
ro = np.array([0.104567, 0.1064, 0.103862, 0.1015, 0.105649, 0.11254, 0.1022, 0.11287, 0.10762, 0.11294, 0.132])

# 2. Tìm đa thức bậc 8 xấp xỉ Ro(SOC)
coeffs_ro = np.polyfit(soc, ro, 8)
p_ro = np.poly1d(coeffs_ro)

# 3. In kết quả ra màn hình (định dạng dùng cho C/C++)
print("--- Hệ số đa thức Ro(SOC) bậc 8 ---")
coeff_str = ", ".join([f"{c:.10e}" for c in coeffs_ro])
print(f"float ro_coeffs[] = {{{coeff_str}}};")

# 4. Lưu vào file 'he_so_ro.txt'
np.savetxt('he_so_ro.txt', coeffs_ro, fmt='%.10e', header='He so da thuc Ro bac 8 (Tu x^8 den hang so)')
print("\n--- Đã lưu hệ số Ro vào file 'he_so_ro.txt' ---")

# 5. Vẽ đồ thị kiểm tra
x_range = np.linspace(0, 1, 100)
plt.figure(figsize=(8, 5))
plt.scatter(soc, ro, color='green', label='Dữ liệu Ro thực tế')
plt.plot(x_range, p_ro(x_range), color='darkgreen', linestyle='--', label='Xấp xỉ bậc 8')
plt.title('Mối quan hệ Ro - SOC')
plt.xlabel('SOC')
plt.ylabel('Internal Resistance Ro (Ohm)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()