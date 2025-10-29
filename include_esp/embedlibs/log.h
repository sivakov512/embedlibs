#pragma once

#include "sugar.h"
#include <esp_log.h>

EXTERN_C_BEGIN

// Direct aliases to ESP-IDF macros for compatibility
#define LOG_E(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#define LOG_W(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#define LOG_I(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define LOG_D(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#define LOG_V(tag, format, ...) ESP_LOGV(tag, format, ##__VA_ARGS__)

// Short macros for local tag (define TAG before use)
#define LOGE(format, ...) ESP_LOGE(TAG, format, ##__VA_ARGS__)
#define LOGW(format, ...) ESP_LOGW(TAG, format, ##__VA_ARGS__)
#define LOGI(format, ...) ESP_LOGI(TAG, format, ##__VA_ARGS__)
#define LOGD(format, ...) ESP_LOGD(TAG, format, ##__VA_ARGS__)
#define LOGV(format, ...) ESP_LOGV(TAG, format, ##__VA_ARGS__)

// Logging helpers
#define RETURN_ON_ERROR(expr, fmt, ...)                                                            \
  do {                                                                                             \
    esp_err_t err = (expr);                                                                        \
    if (err != ESP_OK) {                                                                           \
      LOGE(fmt " Err: %s", ##__VA_ARGS__, esp_err_to_name(err));                                   \
      return err;                                                                                  \
    }                                                                                              \
  } while (0)

EXTERN_C_END
