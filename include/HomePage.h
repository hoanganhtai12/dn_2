const char homeCode[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Giám sát nhiệt độ</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
  <style>
    body { font-family: sans-serif; text-align: center; background-color: #f2f2f2; padding: 10px; }
    h1 { color: #333; }
    #tempValue { font-size: 2em; margin-bottom: 20px; }
    canvas { max-width: 100%; }
  </style>
</head>
<body>
  <h1>Giám sát nhiệt độ LM35</h1>
  <div id="tempValue">Đang tải...</div>
  <canvas id="tempChart"></canvas>

  <script>
    let tempDisplay = document.getElementById("tempValue");
    const ctx = document.getElementById('tempChart').getContext('2d');

    const tempData = {
      labels: [],
      datasets: [{
        label: 'Nhi\u{1EC7}t \u{111}\u{1ED9} (\u00B0C)',
        data: [],
        borderColor: 'rgb(255, 99, 132)',
        fill: false,
        tension: 0.1
      }]
    };

    const tempChart = new Chart(ctx, {
      type: 'line',
      data: tempData,
      options: {
        responsive: true,
        scales: {
          x: { display: true, title: { display: true, text: 'Th\u{1EDD}i gian' }},
          y: { beginAtZero: true, title: { display: true, text: '\u00B0C' } }
        }
      }
    });

    function updateChart(temp) {
      const now = new Date().toLocaleTimeString();
      tempData.labels.push(now);
      tempData.datasets[0].data.push(temp);

      if (tempData.labels.length > 20) {
        tempData.labels.shift();
        tempData.datasets[0].data.shift();
      }

      tempChart.update();
    }

    async function fetchTemp() {
      try {
        const res = await fetch('/temperature');
        const temp = await res.text();
        const floatTemp = parseFloat(temp);
        tempDisplay.innerText = `Nhi\u{1EC7}t \u{111}\u{1ED9} hi\u{1EC7}n t\u{1EA1}i: ${floatTemp.toFixed(1)} \u00B0C`;
        updateChart(floatTemp);
      } catch (e) {
        tempDisplay.innerText = "L\u{1ED7}i k\u{1EBF}t n\u{1ED1}i";
      }
    }

    setInterval(fetchTemp, 1000);
  </script>
</body>
</html>
)=====";