import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 
import os 

# --- ĐƯỜNG DẪN CƠ SỞ ---
BASE_PATH = r'D:\Kalman_new\soc-estimation-of-li-ion-batteries-main\src'

# --- 1. DỮ LIỆU THAM SỐ ĐA THỨC (GIỮ NGUYÊN) ---
OCV_COEFFS = np.array([-3.4632153300e+02, 1.5112606885e+03, -2.7428559297e+03, 2.6905573401e+03, -1.5513638679e+03, 5.3457145432e+02, -1.0655271238e+02, 1.1571510897e+01, 3.2729981641e+00])
R0_COEFFS  = np.array([-1.0544133772e+02, 4.0924253342e+02, -6.4068139512e+02, 5.1682865175e+02, -2.2679280194e+02, 5.1775075156e+01, -4.9212228520e+00, -3.6913891712e-02, 1.3201441401e-01])
Rp_COEFFS  = np.array([5.6222718254e+02, -2.5184751401e+03, 4.7159244281e+03, -4.7849462868e+03, 2.8511433589e+03, -1.0113303315e+03, 2.0638176321e+02, -2.1863366616e+01, 9.9012600001e-01])
Cp_COEFFS  = np.array([1.0257675439e+07, -4.1364268515e+07, 6.8383786980e+07, -5.9606617217e+07, 2.9249346932e+07, -7.8927763115e+06, 1.0130570251e+06, -3.7187578342e+04, 4.9892719479e+03])

OCV_FUNC = np.poly1d(OCV_COEFFS)
dOCV_FUNC = OCV_FUNC.deriv(m=1)
R0_FUNC = np.poly1d(R0_COEFFS)
Rp_FUNC = np.poly1d(Rp_COEFFS)
Cp_FUNC = np.poly1d(Cp_COEFFS)

def run_multi_soc_ekf():
    input_file_path = os.path.join(BASE_PATH, 'data_final_2.csv')
    try:
        data = pd.read_csv(input_file_path)
        time_s = (data['time_s'] - data['time_s'].iloc[0]).values
        I_data = data['current_A'].values 
        V_measured = data['voltage_V'].values
        SOC_actual = data['soc_c'].values if 'soc_c' in data.columns else None
        Np = len(I_data)
        ts = np.mean(np.diff(time_s))
        Ccb = 1.2 * 3600.0 
    except Exception as e:
        print(f"LỖI ĐỌC FILE: {e}"); return

    # Danh sách các SoC ban đầu từ 0.1 đến 1.0
    initial_soc_list = np.linspace(0.1, 1.0, 10)
    
    plt.figure(figsize=(10, 8))
    ax1 = plt.subplot(2, 1, 1)
    ax1.plot(time_s, I_data, 'b-', label='Current I (A)')
    ax1.set_ylabel("Current (A)"); ax1.grid(True); ax1.legend()

    ax2 = plt.subplot(2, 1, 2)
    if SOC_actual is not None:
        ax2.plot(time_s, SOC_actual, 'k--', linewidth=2, label='Actual SOC', zorder=10)

    print(f"{'Initial SOC':<15} | {'MAE SOC (%)':<15}")
    print("-" * 35)

    for init_soc in initial_soc_list:
        Xk = np.array([init_soc, 0.0]) 
        Pk = np.diag([1e-2, 1e-2])
        Q = np.diag([1e-6, 1e-5]) 
        R = 0.01 
        
        SOC_est = np.zeros(Np)
        
        for k in range(Np):
            soc_k = Xk[0]
            R0_k, Rp_k, Cp_k = R0_FUNC(soc_k), Rp_FUNC(soc_k), Cp_FUNC(soc_k)
            dOCV_dSOC = max(dOCV_FUNC(soc_k), 0.01)

            term_exp = np.exp(-ts / max(Rp_k * Cp_k, 1e-3))
            Ad = np.array([[1.0, 0.0], [0.0, term_exp]])
            Bd = np.array([[ts / Ccb], [-Rp_k * (1.0 - term_exp)]])
            Cd = np.array([[dOCV_dSOC, -1.0]]) 

            # Prediction
            Xk_pred = Ad @ Xk + Bd.flatten() * I_data[k]
            Pk_pred = Ad @ Pk @ Ad.T + Q

            # Update
            V_pred = OCV_FUNC(Xk_pred[0]) - Xk_pred[1] + I_data[k] * R0_k
            err_y = V_measured[k] - V_pred
            Sk = Cd @ Pk_pred @ Cd.T + R
            Kk = Pk_pred @ Cd.T / Sk
            
            Xk = Xk_pred + Kk.flatten() * err_y
            Pk = (np.eye(2) - Kk @ Cd) @ Pk_pred
            Xk[0] = np.clip(Xk[0], 0.0, 1.0)
            SOC_est[k] = Xk[0]

        # Tính MAE cho từng trường hợp
        if SOC_actual is not None:
            mae = np.mean(np.abs(SOC_actual - SOC_est)) * 100
            print(f"{init_soc:<15.1f} | {mae:<15.4f}")
        
        ax2.plot(time_s, SOC_est, label=f'Init SoC {init_soc:.1f}', alpha=0.8)

    ax2.set_xlabel("Time (s)"); ax2.set_ylabel("SOC")
    ax2.grid(True)
    ax2.legend(loc='upper right', fontsize='small', ncol=2)
    
    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    run_multi_soc_ekf()