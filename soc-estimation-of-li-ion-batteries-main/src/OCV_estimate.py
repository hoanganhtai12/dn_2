import numpy as np
import matplotlib.pyplot as plt

# 1. Dữ liệu SOC và OCV trích xuất chính xác từ hình ảnh bảng tính
soc = np.array([1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0])
ocv = np.array([4.14, 4.0644, 4.0493, 4.0384, 4.0013, 3.9644, 3.9084, 3.8748, 3.8204, 3.7692, 3.2729])

# 2. Tìm đa thức bậc 8 xấp xỉ mối quan hệ OCV = f(SOC)
coeffs = np.polyfit(soc, ocv, 8)
p = np.poly1d(coeffs)

# 3. In phương trình ra console để kiểm tra
print("--- Phương trình OCV(SOC) bậc 8 ---")
print(p)

# 4. Lưu hệ số vào file riêng 'he_so_ocv_new.txt'
# Lưu từ bậc cao nhất (x^8) đến bậc thấp nhất (hằng số)
np.savetxt('he_so_ocv_new.txt', coeffs, fmt='%.10e', header='He so da thuc OCV bac 8 (Tu x^8 den hang so)')
print("\n--- Đã lưu hệ số mới vào file 'he_so_ocv_new.txt' ---")

# 5. Vẽ đồ thị để trực quan hóa độ khớp
x_range = np.linspace(0, 1, 100)
plt.figure(figsize=(10, 6))
plt.scatter(soc, ocv, color='blue', label='Dữ liệu thực tế từ Excel')
plt.plot(x_range, p(x_range), color='red', linestyle='--', label='Đường cong xấp xỉ bậc 8')

plt.title('Mối quan hệ OCV - SOC (Dữ liệu mới)')
plt.xlabel('State of Charge (SOC)')
plt.ylabel('Open Circuit Voltage (OCV) [V]')
plt.legend()
plt.grid(True, which='both', linestyle='--', alpha=0.5)

# Hiển thị đồ thị
plt.show()