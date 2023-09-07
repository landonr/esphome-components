#include "MediaPlayerSourceBase.h"
#include <algorithm>
#include "esphome/components/json/json_util.h"

namespace esphome {
namespace media_player_source {

static const char* const TAG = "media.player.source.base";

void MediaPlayerSourceBase::parse_json_array(const std::string& data,
                                             const json_parse_array_t& f) {
  // Here we are allocating 1.5 times the data size,
  // with the heap size minus 2kb to be safe if less than that
  // as we can not have a true dynamic sized document.
  // The excess memory is freed below with `shrinkToFit()`
#ifdef USE_ESP8266
  const size_t free_heap = ESP.getMaxFreeBlockSize();
#elif defined(USE_ESP32)
  const size_t free_heap = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
#elif defined(USE_RP2040)
  const size_t free_heap = rp2040.getFreeHeap();
#endif
  bool pass = false;
  size_t request_size = std::min(free_heap, (size_t)(data.size() * 1.5));
  do {
    DynamicJsonDocument json_document(request_size);
    if (json_document.capacity() == 0) {
      ESP_LOGE(TAG,
               "Could not allocate memory for JSON document! Requested %u "
               "bytes, free heap: %u",
               request_size, free_heap);
      return;
    }
    DeserializationError err = deserializeJson(json_document, data);
    json_document.shrinkToFit();

    JsonArray root = json_document.as<JsonArray>();

    if (err == DeserializationError::Ok) {
      pass = true;
      f(root);
    } else if (err == DeserializationError::NoMemory) {
      if (request_size * 2 >= free_heap) {
        ESP_LOGE(TAG,
                 "Can not allocate more memory for deserialization. Consider "
                 "making source string smaller");
        return;
      }
      ESP_LOGV(TAG, "Increasing memory allocation.");
      request_size *= 2;
      continue;
    } else {
      ESP_LOGE(TAG, "JSON parse error: %s", err.c_str());
      return;
    }
  } while (!pass);
}

}  // namespace media_player_source
}  // namespace esphome
