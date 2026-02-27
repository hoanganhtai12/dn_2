import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 
import os 

# --- ĐƯỜNG DẪN CƠ SỞ (BASE PATH) ---
BASE_PATH = r'D:\Kalman_new\soc-estimation-of-li-ion-batteries-main\src'

# --- 1. DỮ LIỆU THAM SỐ ĐA THỨC (GIỮ NGUYÊN) ---
OCV_COEFFS = np.array([-3.4632153300e+02, 1.5112606885e+03, -2.7428559297e+03, 
                       2.6905573401e+03, -1.5513638679e+03, 5.3457145432e+02, 
                       -1.0655271238e+02, 1.1571510897e+01, 3.2729981641e+00])

R0_COEFFS  = np.array([-1.0544133772e+02, 4.0924253342e+02, -6.4068139512e+02, 
                       5.1682865175e+02, -2.2679280194e+02, 5.1775075156e+01, 
                       -4.9212228520e+00, -3.6913891712e-02, 1.3201441401e-01])

Rp_COEFFS  = np.array([5.6222718254e+02, -2.5184751401e+03, 4.7159244281e+03, 
                       -4.7849462868e+03, 2.8511433589e+03, -1.0113303315e+03, 
                       2.0638176321e+02, -2.1863366616e+01, 9.9012600001e-01])

Cp_COEFFS  = np.array([1.0257675439e+07, -4.1364268515e+07, 6.8383786980e+07, 
                       -5.9606617217e+07, 2.9249346932e+07, -7.8927763115e+06, 
                       1.0130570251e+06, -3.7187578342e+04, 4.9892719479e+03])

OCV_FUNC = np.poly1d(OCV_COEFFS)
dOCV_FUNC = OCV_FUNC.deriv(m=1)
R0_FUNC = np.poly1d(R0_COEFFS)
Rp_FUNC = np.poly1d(Rp_COEFFS)
Cp_FUNC = np.poly1d(Cp_COEFFS)

# --- 2. HÀM TRA CỨU SOC(OCV) (GIỮ NGUYÊN) ---
def lookup_soc_from_ocv(v_in):
    segments = [
        [3.27290000, 3.76920000, 0.20149103, -0.65946000],
        [3.76920000, 3.82040000, 1.95312500, -7.26171875],
        [3.82040000, 3.87480000, 1.83823529, -6.82279412],
        [3.87480000, 3.90840000, 2.97619048, -11.23214286],
        [3.90840000, 3.96440000, 1.78571429, -6.57928571],
        [3.96440000, 4.00160000, 2.68817204, -10.15698925],
        [4.00160000, 4.03880000, 2.68817204, -10.15698925],
        [4.03880000, 4.04920000, 9.61538462, -38.13461538],
        [4.04920000, 4.06440000, 6.57894737, -25.83947368],
        [4.06440000, 4.14000000, 1.32275132, -4.47619048]
    ]
    if v_in <= segments[0][0]: return 0.0
    if v_in >= segments[-1][1]: return 1.0
    for v_min, v_max, a, b in segments:
        if v_min <= v_in < v_max:
            return np.clip(a * v_in + b, 0.0, 1.0)
    return 0.5

# --- 3. HÀM CHÍNH CHO EKF ---
def run_extended_kalman_filter():
    print("--- Khởi động Bộ lọc Kalman Mở rộng (EKF) với dữ liệu mới ---")

    START_INDEX = 0
    input_file_path = os.path.join(BASE_PATH, 'data_final_2.csv')

    try:
        data = pd.read_csv(input_file_path) 
        if len(data) > START_INDEX:
            data = data.iloc[START_INDEX:].reset_index(drop=True)

        if 'time_s' in data.columns:
            time_offset = data['time_s'].iloc[0]
            data['time_s'] = data['time_s'] - time_offset
            
        SOC_actual = data['soc_c'].values if 'soc_c' in data.columns else None
        I_data =  data['current_A'].values 
        V_measured = data['voltage_V'].values
        time_s = data['time_s'].values       
        Np = len(I_data)      
        ts = data['time_s'].diff().mean() if len(data) > 1 else 1.0
        Ccb = 1.2 * 3600.0 
    except Exception as e:
        print(f"LỖI KHỞI TẠO: {e}"); return

    v_init_measure = V_measured[0]
    initial_soc = lookup_soc_from_ocv(v_init_measure)
    v_ocv_guess = v_init_measure - I_data[0] * R0_FUNC(initial_soc)
    initial_soc = lookup_soc_from_ocv(v_ocv_guess)
    
    Xk = np.array([0.4, 0.0]) 
    Pk = np.diag([1e-2, 1e-2])
    Q = np.diag([1e-6, 1e-5]) 
    R = 0.01       

    SOC_est, Up_est, Vk_est = np.zeros(Np), np.zeros(Np), np.zeros(Np)
    R0_vals, OCV_vals = np.zeros(Np), np.zeros(Np)

    for k in range(Np):
        soc_k = Xk[0]
        R0_k, Rp_k, Cp_k = R0_FUNC(soc_k), Rp_FUNC(soc_k), Cp_FUNC(soc_k)
        dOCV_dSOC = max(dOCV_FUNC(soc_k), 0.01)

        term_exp = np.exp(-ts / max(Rp_k * Cp_k, 1e-3))
        Ad = np.array([[1.0, 0.0], [0.0, term_exp]])
        Bd = np.array([[ts / Ccb], [-Rp_k * (1.0 - term_exp)]])
        Cd = np.array([[dOCV_dSOC, -1.0]]) 

        # 1. Prediction
        Xk_pred = Ad @ Xk + Bd.flatten() * I_data[k]
        Pk_pred = Ad @ Pk @ Ad.T + Q

        # 2. Update
        current_ocv = OCV_FUNC(Xk_pred[0])
        V_pred = current_ocv - Xk_pred[1] + I_data[k] * R0_k
        err_y = V_measured[k] - V_pred
        Sk = Cd @ Pk_pred @ Cd.T + R
        Kk = Pk_pred @ Cd.T / Sk
        
        Xk = Xk_pred + Kk.flatten() * err_y
        Pk = (np.eye(2) - Kk @ Cd) @ Pk_pred
        Xk[0] = np.clip(Xk[0], 0.0, 1.0)

        SOC_est[k], Up_est[k], Vk_est[k] = Xk[0], Xk[1], V_pred
        R0_vals[k], OCV_vals[k] = R0_k, current_ocv

    # --- 4. TÍNH TOÁN SAI SỐ MAE (PHẦN THÊM MỚI) ---
    mae_v = np.mean(np.abs(V_measured - Vk_est))
    mae_soc = None
    if SOC_actual is not None:
        soc_error_pct =- (SOC_actual - SOC_est) * 100
        mae_soc = np.mean(np.abs(soc_error_pct))
        print(f"\n>> MAE SOC: {mae_soc:.4f} %")
        print(f">> MAE Voltage: {mae_v:.4f} V\n")

    # --- 5. LƯU KẾT QUẢ ---
    results_df = pd.DataFrame({
        'Time_s': time_s, 'V_measured_V': V_measured, 'V_estimated_V': Vk_est,
        'SOC_estimated': SOC_est, 'V_error_V': V_measured - Vk_est
    })
    if SOC_actual is not None:
        results_df['SOC_actual'] = SOC_actual
        results_df['SOC_error_percent'] = soc_error_pct
    results_df.to_csv(os.path.join(BASE_PATH, 'EKF_Results_Full.csv'), index=False)

    # --- 6. ĐỒ THỊ (CẬP NHẬT HIỂN THỊ MAE) ---
    fig, axes = plt.subplots(4, 1, figsize=(10, 12), sharex=True) 

    axes[0].plot(time_s, I_data, 'b-', label='Current I (A)')
    axes[0].set_ylabel("Current (A)"); axes[0].grid(True); axes[0].legend()
    
    axes[1].plot(time_s, SOC_est, 'r-', label='SOC Estimated')
    if SOC_actual is not None: 
        axes[1].plot(time_s, SOC_actual, 'k--', label='SOC Actual')
    axes[1].set_ylabel("SOC"); axes[1].grid(True); axes[1].legend()
    
    if SOC_actual is not None:
        axes[2].plot(time_s, soc_error_pct, 'm-', label='SOC Error (%)')
        axes[2].axhline(0, color='k', linestyle=':')
        axes[2].text(0.02, 0.8, f'MAE: {mae_soc:.4f}%', transform=axes[2].transAxes, fontweight='bold', bbox=dict(facecolor='white', alpha=0.7))
        axes[2].set_ylabel("Error (%)"); axes[2].grid(True); axes[2].legend()

    axes[3].plot(time_s, V_measured, 'k--', alpha=0.6, label='V Measured')
    axes[3].plot(time_s, Vk_est, 'g-', alpha=0.8, label='V Calculator (EKF)')
    axes[3].text(0.02, 0.8, f'MAE: {mae_v:.4f}V', transform=axes[3].transAxes, fontweight='bold', bbox=dict(facecolor='white', alpha=0.7))
    axes[3].set_xlabel('Time (s)'); axes[3].set_ylabel("Voltage (V)")
    axes[3].grid(True); axes[3].legend()

    plt.tight_layout(); plt.show()

if __name__ == '__main__':
    run_extended_kalman_filter()