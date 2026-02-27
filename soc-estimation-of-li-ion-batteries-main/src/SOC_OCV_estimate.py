import numpy as np
import matplotlib.pyplot as plt

# 1. Dữ liệu Lookup Table (x là OCV, y là SOC)
ocv_samples = np.array([3.2729, 3.7692, 3.8204, 3.8748, 3.9084, 3.9644, 4.0016, 4.0388, 4.0492, 4.0644, 4.14])
soc_samples = np.array([0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])

# 2. Tính toán phương trình y = ax + b cho từng đoạn
equations = []
for i in range(len(ocv_samples) - 1):
    x1, x2 = ocv_samples[i], ocv_samples[i+1]
    y1, y2 = soc_samples[i], soc_samples[i+1]
    a = (y2 - y1) / (x2 - x1)
    b = y1 - a * x1
    equations.append([x1, x2, a, b])

equations = np.array(equations)

# 3. Hàm ước lượng SOC (để phục vụ việc vẽ đồ thị mượt mà)
def estimate_soc(ocv_val):
    for row in equations:
        if row[0] <= ocv_val <= row[1]:
            return row[2] * ocv_val + row[3]
    if ocv_val < ocv_samples[0]: return 0.0
    if ocv_val > ocv_samples[-1]: return 1.0
    return None

# --- PHẦN VẼ ĐỒ THỊ BỔ SUNG ---
# Tạo mảng OCV mịn để vẽ đường biểu diễn
ocv_fine = np.linspace(ocv_samples.min() - 0.02, ocv_samples.max() + 0.02, 500)
soc_fine = [estimate_soc(v) for v in ocv_fine]

plt.figure(figsize=(10, 6))

# Vẽ đường nội suy tuyến tính
plt.plot(ocv_fine, soc_fine, 'b-', label='Linear Interpolation (Segments)', linewidth=2)

# Vẽ các điểm dữ liệu gốc (Samples)
plt.scatter(ocv_samples, soc_samples, color='red', zorder=5, label='Original Data Points')

# Thêm ghi chú cho từng điểm
for i, txt in enumerate(soc_samples):
    plt.annotate(f"{txt}", (ocv_samples[i], soc_samples[i]), textcoords="offset points", xytext=(0,10), ha='center', fontsize=9)

# Cấu hình trục tọa độ
plt.title('SOC vs OCV: Linear Interpolation Segments', fontsize=14)
plt.xlabel('Open Circuit Voltage (OCV) [V]', fontsize=12)
plt.ylabel('State of Charge (SOC)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()

# Hiển thị kết quả tính toán mảng C trong console
print("--- Đã lưu hệ số và sẵn sàng vẽ đồ thị ---")
print("\n// Copy mảng này vào code ESP32 của bạn:")
print("float lookup_table[][4] = {")
for row in equations:
    print(f"  {{{row[0]:.6f}f, {row[1]:.6f}f, {row[2]:.6f}f, {row[3]:.6f}f}},")
print("};")

plt.show()