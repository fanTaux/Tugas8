<div align="center">
  <h1>🍂 Tugas 8: Embedded Artificial Intelligence</h1>
  <p><b>Implementasi FSM dan FreeRTOS untuk Inferensi TFLite Micro pada ESP32</b></p>

  <img src="https://img.shields.io/badge/ESP32-4E3822?style=for-the-badge&logo=espressif&logoColor=white" alt="ESP32" />
  <img src="https://img.shields.io/badge/FreeRTOS-6B4E3D?style=for-the-badge&logo=freertos&logoColor=white" alt="FreeRTOS" />
  <img src="https://img.shields.io/badge/C++-8B5A2B?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++" />
  <img src="https://img.shields.io/badge/TensorFlow_Lite-A0522D?style=for-the-badge&logo=tensorflow&logoColor=white" alt="TFLite" />
</div>

<br>

<h2>📌 Tentang Proyek</h2>
<p>
  Proyek ini merupakan submisi tugas untuk mata kuliah <b>Embedded Artificial Intelligence</b>. Pada repositori ini, kita membangun sebuah sistem <i>real-time</i> pada mikrokontroler ESP32 yang bertugas melakukan akuisisi data sensor dan menjalankan inferensi <i>Machine Learning</i>. 
</p>
<p>
  Untuk memenuhi standar industri dan memastikan bahwa waktu <i>sampling</i> data sensor tidak terganggu oleh lamanya waktu komputasi AI, kita mengadopsi arsitektur <b>FreeRTOS</b> yang dipadukan dengan konsep <b>Finite State Machine (FSM)</b>.
</p>

<h2>✨ Fitur Utama</h2>
<ul>
  <li><b>Presisi Sampling Tingkat Tinggi:</b> Menggunakan fungsi <code>vTaskDelayUntil</code> untuk mengunci interval pembacaan sensor tepat di 100ms tanpa adanya <i>time-drift</i>.</li>
  <li><b>Asynchronous Processing (Dual-Core):</b> Pembacaan sensor berjalan pada <b>Core 0</b>, sementara inferensi AI dieksekusi pada <b>Core 1</b> setiap 1 detik sekali (setelah 10 data terkumpul).</li>
  <li><b>Thread-Safe Data Transfer:</b> Implementasi <code>xQueue</code> untuk menjembatani komunikasi data antar <i>Task</i> secara aman tanpa risiko <i>race condition</i>.</li>
  <li><b>Simulasi TFLite Ready:</b> Struktur memori (Tensor Arena) dan interpreter telah disiapkan, sehingga siap digunakan dengan model <code>.tflite</code> sungguhan.</li>
</ul>

<h2>⚙️ Arsitektur Sistem</h2>
<table width="100%">
  <tr align="center" bgcolor="#D2B48C">
    <th>Task Name</th>
    <th>Core</th>
    <th>Fungsi FSM Utama</th>
  </tr>
  <tr align="center">
    <td><b>TaskSensorFSM</b></td>
    <td>0</td>
    <td>Inisialisasi sensor, pembacaan data setiap 100ms, dan pengiriman <i>array</i> ke antrean (Queue).</td>
  </tr>
  <tr align="center">
    <td><b>TaskInferenceFSM</b></td>
    <td>1</td>
    <td>Menunggu data masuk ke antrean, mengeksekusi <code>interpreter->Invoke()</code>, dan mencetak hasil diagnosis.</td>
  </tr>
</table>

<h2>🚀 Panduan Eksekusi</h2>
<ol>
  <li>Pastikan kita sudah menginstal <b>Arduino IDE</b> dan menambahkan <i>Board Manager</i> untuk ESP32.</li>
  <li>Lakukan <i>clone</i> repositori ini ke dalam direktori lokal kita.</li>
  <li>Buka file <code>.ino</code> atau file sumber utama proyek ini.</li>
  <li>Hubungkan <i>board</i> ESP32 ke komputer, lalu lakukan proses kompilasi dan <i>upload</i>.</li>
  <li>Buka <b>Serial Monitor</b> dengan <i>Baud Rate</i> <code>115200</code>.</li>
  <li>Kita akan melihat log siklus pembacaan data sebanyak 10 kali yang disusul oleh hasil inferensi "NORMAL", "DEMAM RINGAN", atau "DEMAM BERAT".</li>
</ol>

<br>

<div align="center">
  <hr style="border: 1px solid #8B5D33; width: 50%;">
  <p><b>Faris Arinanta</b><br>
  Teknik Komputer | Universitas Brawijaya</p>
</div>
