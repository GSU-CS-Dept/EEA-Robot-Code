//重要配置：
// #define HTTPD_DEFAULT_CONFIG()             \
//   {                                        \
//     .task_priority = tskIDLE_PRIORITY + 5, \
//     .stack_size = 4096,                    \
//     .server_port = 80,                     \
//     .ctrl_port = 32768,                    \
//     .max_open_sockets = 7,                 \
//     .max_uri_handlers = 8,                 \
//     .max_resp_headers = 8,                 \
//     .backlog_conn = 5,                     \
//     .lru_purge_enable = false,             \
//     .recv_wait_timeout = 5,                \
//     .send_wait_timeout = 5,                \
//     .global_user_ctx = NULL,               \
//     .global_user_ctx_free_fn = NULL,       \
//     .global_transport_ctx = NULL,          \
//     .global_transport_ctx_free_fn = NULL,  \
//     .open_fn = NULL,                       \
//     .close_fn = NULL,                      \
//   }

// Select camera model
// #define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM

//#define CAMERA_MODEL_M5STACK_WIDE

//#define CAMERA_MODEL_ESP32S3_hezhou
#define CAMERA_MODEL_ESP32S3_EYE
//#define CAMERA_MODEL_AI_THINKER

#include "CameraWebServer_AP.h"
#include "camera_pins.h"
#include "esp_system.h"

// #include "BLEAdvertisedDevice.h"
// BLEAdvertisedDevice _BLEAdvertisedDevice;

void startCameraServer();
void CameraWebServer_AP::CameraWebServer_AP_Init(void)
{

  Serial.setDebugOutput(true);
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
 // config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  //init with high specs to pre-allocate larger buffers
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();

    if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if(config.pixel_format == PIXFORMAT_JPEG){
   // s->set_framesize(s, FRAMESIZE_QVGA);
    s->set_framesize(s, FRAMESIZE_SVGA);
   //  s->set_framesize(s, FRAMESIZE_SXGA);
  // s->set_framesize(s, FRAMESIZE_UXGA);
   // s->set_framesize(s, FRAMESIZE_QSXGA);


  }


#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  //  s->set_vflip(s, 1);
  //  s->set_hmirror(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif


  uint64_t chipid = ESP.getEfuseMac();
  char string[10];
  sprintf(string, "%04X", (uint16_t)(chipid >> 32));
  String mac0_default = String(string);
  sprintf(string, "%08X", (uint32_t)chipid);
  String mac1_default = String(string);
  String url = ssid + mac0_default + mac1_default;
  const char *mac_default = url.c_str();


  Serial.println(":----------------------------:");
  Serial.print("wifi_name:");
  Serial.println(mac_default);
  Serial.println(":----------------------------:");
  wifi_name = mac0_default + mac1_default;

  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(mac_default, password, 9);
  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.softAPIP());
  Serial.println("' to connect");
}
