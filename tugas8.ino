#include <Arduino.h>

// ==========================================
// DUMMY TFLITE (Agar kode bisa dikompilasi)
// ==========================================
struct DummyData { float f[10]; };
struct DummyTensor { DummyData data; };
struct DummyInterpreter { int Invoke() { return 0; } }; 
const int kTfLiteOk = 0;

DummyTensor* input = new DummyTensor();
DummyTensor* output = new DummyTensor();
DummyInterpreter* interpreter = new DummyInterpreter();

enum SensorState { SENSOR_INIT, SENSOR_READ, SENSOR_IDLE };
enum InferenceState { INFER_WAIT, INFER_RUN };

SensorState sensorState = SENSOR_INIT;
InferenceState inferenceState = INFER_WAIT;

float sensorBuffer[10];
int sampleCount = 0;
QueueHandle_t dataQueue;

void TaskSensorFSM(void* pvParameters);
void TaskInferenceFSM(void* pvParameters);

void setup() {
  Serial.begin(115200);
  
  dataQueue = xQueueCreate(2, sizeof(float) * 10);

  if (dataQueue == NULL) {
    Serial.println("Gagal membuat Queue!");
    while(1);
  }

  xTaskCreatePinnedToCore(TaskSensorFSM, "SensorTask", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskInferenceFSM, "InferenceTask", 8192, NULL, 1, NULL, 1);
}

void loop() {
}

void TaskSensorFSM(void* pvParameters) {
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 100 / portTICK_PERIOD_MS; 

  xLastWakeTime = xTaskGetTickCount();

  while (1) {
    switch (sensorState) {
      case SENSOR_INIT:
        Serial.println("[SENSOR] Inisialisasi...");
        sensorState = SENSOR_READ;
        break;

      case SENSOR_READ:
        sensorBuffer[sampleCount] = random(350, 400) / 10.0; 
        
        Serial.print("[SENSOR] Data ke-"); 
        Serial.print(sampleCount + 1);
        Serial.print(": "); 
        Serial.println(sensorBuffer[sampleCount]);

        sampleCount++;

        if (sampleCount == 10) {
          xQueueSend(dataQueue, &sensorBuffer, 0);
          sampleCount = 0;
        }
        
        sensorState = SENSOR_READ;
        break;

      case SENSOR_IDLE:

        break;
    }
    
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void TaskInferenceFSM(void* pvParameters) {
  float receivedData[10];

  while (1) {
    switch (inferenceState) {
      case INFER_WAIT:
        if (xQueueReceive(dataQueue, &receivedData, portMAX_DELAY) == pdTRUE) {
          inferenceState = INFER_RUN;
        }
        break;

      case INFER_RUN:
        Serial.println("\n[INFERENSI] Memulai inferensi AI...");
        
        float sum = 0;
        for (int i = 0; i < 10; i++) {
          input->data.f[i] = receivedData[i];
          sum += receivedData[i]; 
        }

        Serial.print("[INFERENSI] Total Penjumlahan Array: ");
        Serial.println(sum);

        if (interpreter->Invoke() == kTfLiteOk) {
          float result = sum / 10.0; 
          output->data.f[0] = result;

          Serial.print("[INFERENSI] Hasil (Suhu Rata-rata): "); 
          Serial.println(result);
          
          if (result < 37.0) {
            Serial.println(">> DIAGNOSA: NORMAL <<\n");
          } else if (result < 38.5) {
            Serial.println(">> DIAGNOSA: DEMAM RINGAN <<\n");
          } else {
            Serial.println(">> DIAGNOSA: DEMAM BERAT <<\n");
          }
        }
        
        inferenceState = INFER_WAIT;
        break;
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}
