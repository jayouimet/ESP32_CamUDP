#pragma once

#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "esp32-hal-ledc.h"
#include "sdkconfig.h"
#include "camera_index.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#endif

// Enable LED FLASH setting
#define CONFIG_LED_ILLUMINATOR_ENABLED 1

// LED FLASH setup
#if CONFIG_LED_ILLUMINATOR_ENABLED

#define LED_LEDC_GPIO            22  //configure LED pin
#define CONFIG_LED_MAX_INTENSITY 255

#endif

#define PART_BOUNDARY "123456789000000000000987654321"

typedef struct {
  size_t size;   //number of values used for filtering
  size_t index;  //current value index
  size_t count;  //value count
  int sum;
  int *values;  //array to be filled with values
} ra_filter_t;

typedef struct {
  httpd_req_t *req;
  size_t len;
} jpg_chunking_t;

class WebServer {
public:
  WebServer();
  void start();

private: 
  httpd_config_t m_Config;

  httpd_handle_t m_StreamHttpd = NULL;
  httpd_handle_t m_CameraHttpd = NULL;

  ra_filter_t m_RaFilter;
private:
  static const char* _STREAM_CONTENT_TYPE;
  static const char* _STREAM_BOUNDARY;
  static const char* _STREAM_PART;


  ra_filter_t* ra_filter_init(ra_filter_t *filter, size_t sample_size);
  size_t jpg_encode_stream(void *arg, size_t index, const void *data, size_t len);

  static esp_err_t index_handler(httpd_req_t *req);
  static esp_err_t stream_handler(httpd_req_t *req);
  static esp_err_t status_handler(httpd_req_t *req);
  static esp_err_t health_handler(httpd_req_t *req);

  static int print_reg(char *p, sensor_t *s, uint16_t reg, uint32_t mask);

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
  int ra_filter_run(ra_filter_t* filter, int value);
#endif
};
