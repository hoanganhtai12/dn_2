const char temperatureCode[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Biểu Đồ Nhiệt Độ - Real-time Monitor</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Poppins', sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            color: #333;
        }

        .container {
            min-height: 100vh;
            display: flex;
            flex-direction: column;
        }

        /* Header Styles */
        .header {
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(20px);
            border-bottom: 1px solid rgba(255, 255, 255, 0.2);
            padding: 15px 0;
            position: sticky;
            top: 0;
            z-index: 100;
        }

        .header-content {
            max-width: 1400px;
            margin: 0 auto;
            padding: 0 20px;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .logo {
            display: flex;
            align-items: center;
            gap: 10px;
            font-size: 20px;
            font-weight: 600;
            color: #667eea;
        }

        .logo i {
            font-size: 24px;
        }

        .nav-buttons {
            display: flex;
            gap: 15px;
        }

        .nav-btn {
            background: #667eea;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 14px;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            gap: 5px;
        }

        .nav-btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.3);
        }

        .nav-btn.logout {
            background: #f44336;
        }

        .nav-btn.logout:hover {
            background: #d32f2f;
        }

        /* Main Content */
        .main-content {
            flex: 1;
            max-width: 1400px;
            margin: 0 auto;
            padding: 30px 20px;
            width: 100%;
        }

        /* Status Panel */
        .status-panel {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }

        .status-card {
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(20px);
            border-radius: 15px;
            padding: 25px;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.1);
            border: 1px solid rgba(255, 255, 255, 0.2);
            display: flex;
            align-items: center;
            gap: 20px;
            transition: all 0.3s ease;
        }

        .status-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 15px 40px rgba(0, 0, 0, 0.15);
        }

        .status-icon {
            font-size: 40px;
            color: #667eea;
            min-width: 60px;
            text-align: center;
        }

        .status-info h3 {
            font-size: 16px;
            color: #666;
            margin-bottom: 10px;
        }

        .temp-value {
            font-size: 32px;
            font-weight: 700;
            color: #333;
            margin-bottom: 5px;
        }

        .temp-status {
            font-size: 14px;
            color: #4CAF50;
            font-weight: 500;
        }

        .connection-status {
            font-size: 16px;
            font-weight: 600;
            color: #4CAF50;
            margin-bottom: 5px;
        }

        .connection-status.disconnected {
            color: #f44336;
        }

        .last-update {
            font-size: 12px;
            color: #999;
        }

        .stats-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 10px;
        }

        .stat-item {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .stat-label {
            font-size: 14px;
            color: #666;
        }

        .stat-value {
            font-size: 16px;
            font-weight: 600;
            color: #333;
        }

        /* Chart Section */
        .chart-section {
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(20px);
            border-radius: 20px;
            padding: 30px;
            margin-bottom: 30px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            border: 1px solid rgba(255, 255, 255, 0.2);
        }

        .chart-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 25px;
            flex-wrap: wrap;
            gap: 15px;
        }

        .chart-header h2 {
            font-size: 24px;
            font-weight: 600;
            color: #333;
        }

        .chart-controls {
            display: flex;
            gap: 15px;
            align-items: center;
            flex-wrap: wrap;
        }

        .control-btn {
            background: #667eea;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 14px;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            gap: 5px;
        }

        .control-btn:hover {
            background: #5a6fd8;
            transform: translateY(-2px);
        }

        .control-btn.paused {
            background: #ff9800;
        }

        .time-range {
            padding: 8px 12px;
            border: 2px solid #e1e1e1;
            border-radius: 8px;
            background: white;
            font-size: 14px;
            cursor: pointer;
            transition: all 0.3s ease;
        }

        .time-range:focus {
            border-color: #667eea;
            outline: none;
        }

        .chart-container {
            position: relative;
            height: 400px;
            margin-bottom: 20px;
        }

        /* Data Section */
        .data-section {
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(20px);
            border-radius: 20px;
            padding: 30px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            border: 1px solid rgba(255, 255, 255, 0.2);
        }

        .data-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
        }

        .data-header h3 {
            font-size: 20px;
            font-weight: 600;
            color: #333;
        }

        .data-table-container {
            max-height: 300px;
            overflow-y: auto;
            border-radius: 10px;
            border: 1px solid #e1e1e1;
        }

        .data-table {
            width: 100%;
            border-collapse: collapse;
            background: white;
        }

        .data-table th {
            background: #f8f9fa;
            padding: 12px 15px;
            text-align: left;
            font-weight: 600;
            color: #333;
            border-bottom: 2px solid #e1e1e1;
            position: sticky;
            top: 0;
        }

        .data-table td {
            padding: 10px 15px;
            border-bottom: 1px solid #f0f0f0;
            color: #666;
        }

        .data-table tbody tr:hover {
            background: #f8f9fa;
        }

        .temp-normal {
            color: #4CAF50;
            font-weight: 500;
        }

        .temp-high {
            color: #ff9800;
            font-weight: 500;
        }

        .temp-critical {
            color: #f44336;
            font-weight: 500;
        }

        /* Modal Styles */
        .modal {
            display: none;
            position: fixed;
            z-index: 1000;
            left: 0;
            top: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.5);
            backdrop-filter: blur(5px);
        }

        .modal-content {
            background: white;
            margin: 10% auto;
            padding: 0;
            border-radius: 15px;
            width: 90%;
            max-width: 500px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.2);
        }

        .modal-header {
            padding: 20px 25px;
            border-bottom: 1px solid #e1e1e1;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .modal-header h3 {
            font-size: 18px;
            font-weight: 600;
            color: #333;
        }

        .close-btn {
            background: none;
            border: none;
            font-size: 24px;
            cursor: pointer;
            color: #999;
            padding: 0;
            width: 30px;
            height: 30px;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .close-btn:hover {
            color: #333;
        }

        .modal-body {
            padding: 25px;
        }

        .form-group {
            margin-bottom: 20px;
        }

        .form-group label {
            display: block;
            margin-bottom: 8px;
            font-weight: 500;
            color: #333;
        }

        .form-group input,
        .form-group select {
            width: 100%;
            padding: 10px 15px;
            border: 2px solid #e1e1e1;
            border-radius: 8px;
            font-size: 14px;
            transition: all 0.3s ease;
        }

        .form-group input:focus,
        .form-group select:focus {
            border-color: #667eea;
            outline: none;
            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
        }

        .modal-footer {
            padding: 20px 25px;
            border-top: 1px solid #e1e1e1;
            display: flex;
            justify-content: flex-end;
            gap: 15px;
        }

        .btn-primary {
            background: #667eea;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 14px;
            font-weight: 500;
            transition: all 0.3s ease;
        }

        .btn-primary:hover {
            background: #5a6fd8;
            transform: translateY(-2px);
        }

        .btn-secondary {
            background: #f8f9fa;
            color: #666;
            border: 2px solid #e1e1e1;
            padding: 8px 18px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 14px;
            font-weight: 500;
            transition: all 0.3s ease;
        }

        .btn-secondary:hover {
            background: #e9ecef;
            border-color: #d1d1d1;
        }

        /* Responsive Design */
        @media (max-width: 768px) {
            .main-content {
                padding: 20px 10px;
            }
            
            .status-panel {
                grid-template-columns: 1fr;
            }
            
            .chart-section,
            .data-section {
                padding: 20px;
            }
            
            .chart-header {
                flex-direction: column;
                align-items: flex-start;
            }
            
            .chart-controls {
                width: 100%;
                justify-content: flex-start;
            }
            
            .chart-container {
                height: 300px;
            }
            
            .data-header {
                flex-direction: column;
                gap: 15px;
                align-items: flex-start;
            }
            
            .nav-buttons {
                flex-direction: column;
                gap: 10px;
            }
        }

        /* Animation for temperature changes */
        @keyframes tempPulse {
            0% { transform: scale(1); }
            50% { transform: scale(1.05); }
            100% { transform: scale(1); }
        }

        .temp-update {
            animation: tempPulse 0.5s ease-in-out;
        }

        /* Connection status indicators */
        .status-icon.connected {
            color: #4CAF50;
        }

        .status-icon.disconnected {
            color: #f44336;
        }

        .status-icon.connecting {
            color: #ff9800;
            animation: pulse 1s infinite;
        }

        @keyframes pulse {
            0% { opacity: 1; }
            50% { opacity: 0.5; }
            100% { opacity: 1; }
        }
    </style>
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/date-fns@2.29.3/index.min.js"></script>
</head>
<body>
    <div class="container">
        <!-- Header -->
        <header class="header">
            <div class="header-content">
                <div class="logo">
                    <i class="fas fa-thermometer-half"></i>
                    <span>Temperature Monitor</span>
                </div>
                <div class="nav-buttons">
                    <button class="nav-btn" onclick="goToUpload()">
                        <i class="fas fa-upload"></i>
                        Upload Files
                    </button>
                    <button class="nav-btn logout" onclick="logout()">
                        <i class="fas fa-sign-out-alt"></i>
                        Đăng xuất
                    </button>
                </div>
            </div>
        </header>

        <!-- Main Content -->
        <main class="main-content">
            <!-- Status Panel -->
            <div class="status-panel">
                <div class="status-card current-temp">
                    <div class="status-icon">
                        <i class="fas fa-thermometer-half"></i>
                    </div>
                    <div class="status-info">
                        <h3>Nhiệt độ hiện tại</h3>
                        <div class="temp-value" id="currentTemp">--°C</div>
                        <div class="temp-status" id="tempStatus">Đang kết nối...</div>
                    </div>
                </div>

                <div class="status-card connection">
                    <div class="status-icon">
                        <i class="fas fa-wifi" id="connectionIcon"></i>
                    </div>
                    <div class="status-info">
                        <h3>Kết nối</h3>
                        <div class="connection-status" id="connectionStatus">Đang kết nối...</div>
                        <div class="last-update" id="lastUpdate">Chưa có dữ liệu</div>
                    </div>
                </div>

                <div class="status-card stats">
                    <div class="status-icon">
                        <i class="fas fa-chart-line"></i>
                    </div>
                    <div class="status-info">
                        <h3>Thống kê</h3>
                        <div class="stats-grid">
                            <div class="stat-item">
                                <span class="stat-label">Cao nhất:</span>
                                <span class="stat-value" id="maxTemp">--°C</span>
                            </div>
                            <div class="stat-item">
                                <span class="stat-label">Thấp nhất:</span>
                                <span class="stat-value" id="minTemp">--°C</span>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Chart Section -->
            <div class="chart-section">
                <div class="chart-header">
                    <h2>Biểu Đồ Nhiệt Độ Theo Thời Gian</h2>
                    <div class="chart-controls">
                        <button class="control-btn" id="pauseBtn" onclick="togglePause()">
                            <i class="fas fa-pause"></i>
                            Tạm dừng
                        </button>
                        <button class="control-btn" onclick="clearChart()">
                            <i class="fas fa-trash"></i>
                            Xóa dữ liệu
                        </button>
                        <select class="time-range" id="timeRange" onchange="changeTimeRange()">
                            <option value="30">30 giây</option>
                            <option value="60">1 phút</option>
                            <option value="300" selected>5 phút</option>
                            <option value="600">10 phút</option>
                            <option value="1800">30 phút</option>
                            <option value="3600">1 giờ</option>
                        </select>
                    </div>
                </div>

                <div class="chart-container">
                    <canvas id="temperatureChart"></canvas>
                </div>
            </div>

            <!-- Data Table -->
            <div class="data-section">
                <div class="data-header">
                    <h3>Dữ liệu gần đây</h3>
                    <button class="control-btn" onclick="exportData()">
                        <i class="fas fa-download"></i>
                        Xuất CSV
                    </button>
                </div>
                <div class="data-table-container">
                    <table class="data-table" id="dataTable">
                        <thead>
                            <tr>
                                <th>Thời gian</th>
                                <th>Nhiệt độ (°C)</th>
                                <th>Trạng thái</th>
                            </tr>
                        </thead>
                        <tbody id="dataTableBody">
                            <!-- Data will be populated here -->
                        </tbody>
                    </table>
                </div>
            </div>
        </main>
    </div>

    <!-- Connection Modal -->
    <div class="modal" id="connectionModal">
        <div class="modal-content">
            <div class="modal-header">
                <h3>Cấu hình kết nối</h3>
                <button class="close-btn" onclick="closeModal()">&times;</button>
            </div>
            <div class="modal-body">
                <div class="form-group">
                    <label>WebSocket URL:</label>
                    <input type="text" id="wsUrl" value="ws://localhost:3000/temperature" placeholder="ws://server:port/endpoint">
                </div>
                <div class="form-group">
                    <label>Tần suất cập nhật:</label>
                    <select id="updateFreq">
                        <option value="1000" selected>1 giây</option>
                        <option value="2000">2 giây</option>
                        <option value="5000">5 giây</option>
                    </select>
                </div>
            </div>
            <div class="modal-footer">
                <button class="btn-secondary" onclick="closeModal()">Hủy</button>
                <button class="btn-primary" onclick="reconnect()">Kết nối</button>
            </div>
        </div>
    </div>

    <script>
        document.addEventListener('DOMContentLoaded', function() {
            // Chart configuration
            const ctx = document.getElementById('temperatureChart').getContext('2d');
            let temperatureChart;
            let websocket;
            let isPaused = false;
            let maxDataPoints = 300; // 5 minutes at 1 second intervals
            let temperatureData = [];
            let timeLabels = [];
            let stats = {
                min: null,
                max: null,
                current: null
            };

            // Initialize chart[6][9][12]
            function initChart() {
                temperatureChart = new Chart(ctx, {
                    type: 'line',
                    data: {
                        labels: timeLabels,
                        datasets: [{
                            label: 'Nhiệt độ (°C)',
                            data: temperatureData,
                            borderColor: 'rgba(102, 126, 234, 1)',
                            backgroundColor: 'rgba(102, 126, 234, 0.1)',
                            borderWidth: 2,
                            fill: true,
                            tension: 0.4,
                            pointRadius: 0,
                            pointHoverRadius: 6,
                            pointBackgroundColor: 'rgba(102, 126, 234, 1)',
                            pointBorderColor: '#fff',
                            pointBorderWidth: 2
                        }]
                    },
                    options: {
                        responsive: true,
                        maintainAspectRatio: false,
                        interaction: {
                            intersect: false,
                            mode: 'index'
                        },
                        plugins: {
                            legend: {
                                display: true,
                                position: 'top',
                                labels: {
                                    usePointStyle: true,
                                    padding: 20
                                }
                            },
                            tooltip: {
                                backgroundColor: 'rgba(0, 0, 0, 0.8)',
                                titleColor: '#fff',
                                bodyColor: '#fff',
                                borderColor: 'rgba(102, 126, 234, 1)',
                                borderWidth: 1,
                                cornerRadius: 8,
                                displayColors: false,
                                callbacks: {
                                    title: function(context) {
                                        return 'Thời gian: ' + context[0].label;
                                    },
                                    label: function(context) {
                                        return 'Nhiệt độ: ' + context.parsed.y.toFixed(1) + '°C';
                                    }
                                }
                            }
                        },
                        scales: {
                            x: {
                                display: true,
                                title: {
                                    display: true,
                                    text: 'Thời gian'
                                },
                                grid: {
                                    color: 'rgba(0, 0, 0, 0.1)'
                                }
                            },
                            y: {
                                display: true,
                                title: {
                                    display: true,
                                    text: 'Nhiệt độ (°C)'
                                },
                                grid: {
                                    color: 'rgba(0, 0, 0, 0.1)'
                                },
                                beginAtZero: false
                            }
                        },
                        animation: {
                            duration: 0 // Disable animation for real-time updates
                        }
                    }
                });
            }

            // WebSocket connection[7][10][13]
            function connectWebSocket() {
                const wsUrl = document.getElementById('wsUrl').value || 'ws://192.168.137.75/temperature';
                
                try {
                    websocket = new WebSocket(wsUrl);
                    
                    websocket.onopen = function(event) {
                        console.log('WebSocket connected');
                        updateConnectionStatus('connected');
                        showMessage('Kết nối thành công!', 'success');
                    };
                    
                    websocket.onmessage = function(event) {
                        if (!isPaused) {
                            try {
                                const data = JSON.parse(event.data);
                                updateTemperatureData(data.temperature, data.timestamp || new Date());
                            } catch (e) {
                                // If data is just a number (temperature only)
                                const temperature = parseFloat(event.data);
                                if (!isNaN(temperature)) {
                                    updateTemperatureData(temperature, new Date());
                                }
                            }
                        }
                    };
                    
                    websocket.onclose = function(event) {
                        console.log('WebSocket disconnected');
                        updateConnectionStatus('disconnected');
                        showMessage('Mất kết nối với server', 'error');
                        
                        // Attempt to reconnect after 5 seconds
                        setTimeout(() => {
                            if (websocket.readyState === WebSocket.CLOSED) {
                                connectWebSocket();
                            }
                        }, 5000);
                    };
                    
                    websocket.onerror = function(error) {
                        console.error('WebSocket error:', error);
                        updateConnectionStatus('error');
                        showMessage('Lỗi kết nối WebSocket', 'error');
                    };
                    
                } catch (error) {
                    console.error('Failed to create WebSocket:', error);
                    updateConnectionStatus('error');
                    showMessage('Không thể kết nối đến server', 'error');
                }
            }

            // Update temperature data
            function updateTemperatureData(temperature, timestamp) {
                const now = new Date(timestamp);
                const timeString = now.toLocaleTimeString('vi-VN');
                
                // Add new data point
                temperatureData.push(temperature);
                timeLabels.push(timeString);
                
                // Remove old data points if exceeding max
                if (temperatureData.length > maxDataPoints) {
                    temperatureData.shift();
                    timeLabels.shift();
                }
                
                // Update chart
                temperatureChart.update('none');
                
                // Update current temperature display
                updateCurrentTemperature(temperature);
                
                // Update statistics
                updateStats(temperature);
                
                // Update data table
                addDataToTable(timeString, temperature);
                
                // Update last update time
                document.getElementById('lastUpdate').textContent = 'Cập nhật: ' + timeString;
            }

            // Update current temperature display
            function updateCurrentTemperature(temperature) {
                const tempElement = document.getElementById('currentTemp');
                const statusElement = document.getElementById('tempStatus');
                
                tempElement.textContent = temperature.toFixed(1) + '°C';
                tempElement.classList.add('temp-update');
                
                // Remove animation class after animation completes
                setTimeout(() => {
                    tempElement.classList.remove('temp-update');
                }, 500);
                
                // Update status based on temperature
                if (temperature < 0) {
                    statusElement.textContent = 'Rất lạnh';
                    statusElement.className = 'temp-status temp-critical';
                } else if (temperature < 15) {
                    statusElement.textContent = 'Lạnh';
                    statusElement.className = 'temp-status temp-high';
                } else if (temperature < 25) {
                    statusElement.textContent = 'Bình thường';
                    statusElement.className = 'temp-status temp-normal';
                } else if (temperature < 35) {
                    statusElement.textContent = 'Ấm';
                    statusElement.className = 'temp-status temp-high';
                } else {
                    statusElement.textContent = 'Rất nóng';
                    statusElement.className = 'temp-status temp-critical';
                }
            }

            // Update statistics
            function updateStats(temperature) {
                if (stats.min === null || temperature < stats.min) {
                    stats.min = temperature;
                    document.getElementById('minTemp').textContent = temperature.toFixed(1) + '°C';
                }
                
                if (stats.max === null || temperature > stats.max) {
                    stats.max = temperature;
                    document.getElementById('maxTemp').textContent = temperature.toFixed(1) + '°C';
                }
                
                stats.current = temperature;
            }

            // Update connection status
            function updateConnectionStatus(status) {
                const statusElement = document.getElementById('connectionStatus');
                const iconElement = document.getElementById('connectionIcon');
                
                iconElement.className = 'fas fa-wifi';
                
                switch (status) {
                    case 'connected':
                        statusElement.textContent = 'Đã kết nối';
                        statusElement.className = 'connection-status';
                        iconElement.classList.add('connected');
                        break;
                    case 'disconnected':
                        statusElement.textContent = 'Mất kết nối';
                        statusElement.className = 'connection-status disconnected';
                        iconElement.classList.add('disconnected');
                        break;
                    case 'connecting':
                        statusElement.textContent = 'Đang kết nối...';
                        statusElement.className = 'connection-status';
                        iconElement.classList.add('connecting');
                        break;
                    case 'error':
                        statusElement.textContent = 'Lỗi kết nối';
                        statusElement.className = 'connection-status disconnected';
                        iconElement.classList.add('disconnected');
                        break;
                }
            }

            // Add data to table
            function addDataToTable(time, temperature) {
                const tableBody = document.getElementById('dataTableBody');
                const row = document.createElement('tr');
                
                let statusClass = 'temp-normal';
                let statusText = 'Bình thường';
                
                if (temperature < 0 || temperature > 35) {
                    statusClass = 'temp-critical';
                    statusText = temperature < 0 ? 'Rất lạnh' : 'Rất nóng';
                } else if (temperature < 15 || temperature > 25) {
                    statusClass = 'temp-high';
                    statusText = temperature < 15 ? 'Lạnh' : 'Ấm';
                }
                
                row.innerHTML = `
                    <td>${time}</td>
                    <td>${temperature.toFixed(1)}</td>
                    <td class="${statusClass}">${statusText}</td>
                `;
                
                // Insert at the beginning
                tableBody.insertBefore(row, tableBody.firstChild);
                
                // Keep only last 50 rows
                while (tableBody.children.length > 50) {
                    tableBody.removeChild(tableBody.lastChild);
                }
            }

            // Control functions
            window.togglePause = function() {
                isPaused = !isPaused;
                const pauseBtn = document.getElementById('pauseBtn');
                
                if (isPaused) {
                    pauseBtn.innerHTML = '<i class="fas fa-play"></i> Tiếp tục';
                    pauseBtn.classList.add('paused');
                    showMessage('Đã tạm dừng cập nhật dữ liệu', 'info');
                } else {
                    pauseBtn.innerHTML = '<i class="fas fa-pause"></i> Tạm dừng';
                    pauseBtn.classList.remove('paused');
                    showMessage('Tiếp tục cập nhật dữ liệu', 'info');
                }
            };

            window.clearChart = function() {
                if (confirm('Bạn có chắc muốn xóa tất cả dữ liệu?')) {
                    temperatureData.length = 0;
                    timeLabels.length = 0;
                    temperatureChart.update();
                    
                    // Clear table
                    document.getElementById('dataTableBody').innerHTML = '';
                    
                    // Reset stats
                    stats = { min: null, max: null, current: null };
                    document.getElementById('minTemp').textContent = '--°C';
                    document.getElementById('maxTemp').textContent = '--°C';
                    
                    showMessage('Đã xóa tất cả dữ liệu', 'success');
                }
            };

            window.changeTimeRange = function() {
                const timeRange = document.getElementById('timeRange').value;
                maxDataPoints = parseInt(timeRange);
                
                // Trim data if necessary
                if (temperatureData.length > maxDataPoints) {
                    temperatureData.splice(0, temperatureData.length - maxDataPoints);
                    timeLabels.splice(0, timeLabels.length - maxDataPoints);
                    temperatureChart.update();
                }
                
                showMessage(`Đã thay đổi khoảng thời gian hiển thị`, 'info');
            };

            window.exportData = function() {
                if (temperatureData.length === 0) {
                    showMessage('Không có dữ liệu để xuất', 'warning');
                    return;
                }
                
                let csvContent = 'Thời gian,Nhiệt độ (°C)\n';
                for (let i = 0; i < temperatureData.length; i++) {
                    csvContent += `${timeLabels[i]},${temperatureData[i]}\n`;
                }
                
                const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
                const link = document.createElement('a');
                const url = URL.createObjectURL(blob);
                link.setAttribute('href', url);
                link.setAttribute('download', `temperature_data_${new Date().toISOString().split('T')[0]}.csv`);
                link.style.visibility = 'hidden';
                document.body.appendChild(link);
                link.click();
                document.body.removeChild(link);
                
                showMessage('Đã xuất dữ liệu thành công', 'success');
            };

            // Modal functions
            window.closeModal = function() {
                document.getElementById('connectionModal').style.display = 'none';
            };

            window.reconnect = function() {
                if (websocket) {
                    websocket.close();
                }
                updateConnectionStatus('connecting');
                connectWebSocket();
                closeModal();
            };

            // Navigation functions
            window.goToUpload = function() {
                window.location.href = 'upload.html';
            };

            window.logout = function() {
                if (confirm('Bạn có chắc muốn đăng xuất?')) {
                    if (websocket) {
                        websocket.close();
                    }
                    showMessage('Đang đăng xuất...', 'info');
                    setTimeout(() => {
                        window.location.href = 'index.html';
                    }, 1500);
                }
            };

            // Utility function for messages
            function showMessage(message, type) {
                const existingMessage = document.querySelector('.message');
                if (existingMessage) {
                    existingMessage.remove();
                }
                
                const messageEl = document.createElement('div');
                messageEl.className = `message ${type}`;
                messageEl.textContent = message;
                
                messageEl.style.cssText = `
                    position: fixed;
                    top: 20px;
                    right: 20px;
                    padding: 15px 20px;
                    border-radius: 8px;
                    color: white;
                    font-weight: 500;
                    z-index: 1000;
                    animation: slideIn 0.3s ease-out;
                    max-width: 300px;
                `;
                
                switch(type) {
                    case 'success':
                        messageEl.style.background = '#4CAF50';
                        break;
                    case 'error':
                        messageEl.style.background = '#f44336';
                        break;
                    case 'info':
                        messageEl.style.background = '#2196F3';
                        break;
                    case 'warning':
                        messageEl.style.background = '#ff9800';
                        break;
                }
                
                document.body.appendChild(messageEl);
                
                setTimeout(() => {
                    messageEl.style.animation = 'slideOut 0.3s ease-out';
                    setTimeout(() => {
                        messageEl.remove();
                    }, 300);
                }, 3000);
            }

            // Initialize everything
            initChart();
            connectWebSocket();
            updateConnectionStatus('connecting');

            // Simulate data for demo (remove this in production)
            if (window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1') {
                setTimeout(() => {
                    simulateTemperatureData();
                }, 2000);
            }

            // Demo data simulation (remove in production)
            function simulateTemperatureData() {
                let baseTemp = 22;
                setInterval(() => {
                    if (!isPaused) {
                        const variation = (Math.random() - 0.5) * 2;
                        baseTemp += variation * 0.1;
                        baseTemp = Math.max(15, Math.min(35, baseTemp)); // Keep within reasonable range
                        
                        const simulatedData = {
                            temperature: baseTemp,
                            timestamp: new Date()
                        };
                        
                        updateTemperatureData(simulatedData.temperature, simulatedData.timestamp);
                    }
                }, 1000);
                
                updateConnectionStatus('connected');
            }
        });

        // Add CSS for message animations
        const style = document.createElement('style');
        style.textContent = `
            @keyframes slideIn {
                from {
                    opacity: 0;
                    transform: translateX(100%);
                }
                to {
                    opacity: 1;
                    transform: translateX(0);
                }
            }
            
            @keyframes slideOut {
                from {
                    opacity: 1;
                    transform: translateX(0);
                }
                to {
                    opacity: 0;
                    transform: translateX(100%);
                }
            }
        `;
        document.head.appendChild(style);
    </script>
</body>
</html>
)=====";
