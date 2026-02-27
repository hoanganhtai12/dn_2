import re
import pandas as pd
import os

def convert_log_to_csv_direct():
    # --- ĐƯỜNG DẪN TRỰC TIẾP ---
    base_path = r'D:\Kalman_new\soc-estimation-of-li-ion-batteries-main\src'
    input_file = os.path.join(base_path, 'du_lieu_sac.txt') # Tên file log đầu vào của bạn
    output_file = os.path.join(base_path, 'du_lieu_sac.csv')

    data_list = []
    
    # Biểu thức chính quy (Regex) để bóc tách từng trường dữ liệu
    pattern = re.compile(
        r'\[(?P<timestamp>[^\]]+)\] '  # Lấy thời gian trong ngoặc []
        r'V:(?P<voltage_V>[\d.-]+) \| ' # Lấy điện áp
        r'I:(?P<current_A>[\d.-]+) \| ' # Lấy dòng điện
        r'Soc_k:(?P<soc_k>[\d.-]+)% \| ' # Lấy Soc_k (bỏ %)
        r'Soc_c:(?P<soc_c>[\d.-]+)% \| ' # Lấy Soc_c (bỏ %)
        r'state:(?P<state>\w+)'         # Lấy trạng thái (CHG/DSG/IDLE)
    )

    if not os.path.exists(input_file):
        print(f"LỖI: Không tìm thấy file tại {input_file}")
        return

    print(f"Đang đọc dữ liệu từ: {input_file}...")

    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            for line in f:
                match = pattern.search(line)
                if match:
                    row = match.groupdict()
                    # Chuyển đổi kiểu dữ liệu để tính toán
                    row['voltage_V'] = float(row['voltage_V'])
                    row['current_A'] = float(row['current_A'])
                    row['soc_k'] = float(row['soc_k']) / 100.0 # Chuyển về dạng 0-1
                    row['soc_c'] = float(row['soc_c']) / 100.0 # Chuyển về dạng 0-1
                    data_list.append(row)

        if not data_list:
            print("Không tìm thấy dữ liệu khớp với định dạng trong file log.")
            return

        # Tạo DataFrame
        df = pd.DataFrame(data_list)
        
        # Tính toán thời gian thực (time_s) dựa trên timestamp
        df['timestamp'] = pd.to_datetime(df['timestamp'])
        df['time_s'] = (df['timestamp'] - df['timestamp'].iloc[0]).dt.total_seconds()
        
        # Sắp xếp lại cột cho đẹp
        cols = ['time_s', 'voltage_V', 'current_A', 'soc_k', 'soc_c', 'state', 'timestamp']
        df = df[cols]

        # Lưu file CSV
        df.to_csv(output_file, index=False)
        print("-" * 50)
        print(f"THÀNH CÔNG!")
        print(f"File CSV đã lưu tại: {output_file}")
        print(f"Tổng số dòng đã xử lý: {len(df)}")
        print(df.head()) # Hiển thị 5 dòng đầu tiên để kiểm tra

    except Exception as e:
        print(f"Đã xảy ra lỗi trong quá trình xử lý: {e}")

if __name__ == '__main__':
    convert_log_to_csv_direct()