#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/entity_base.h"

namespace esphome {
namespace homeassistant_media_player {

class MediaPlayerCommand : public EntityBase {
 public:
  void on_command() { this->on_command_callbacks_.call(); }
  void add_on_command_callback(std::function<void()>&& callback) {
    this->on_command_callbacks_.add(std::move(callback));
  }

  std::string get_name() const { return name_; }

  void set_name(std::string val) { this->name_ = val; }

 private:
  CallbackManager<void()> on_command_callbacks_{};
  std::string name_;
};

class ServiceCalledTrigger : public Trigger<> {
 public:
  explicit ServiceCalledTrigger(MediaPlayerCommand* parent) {
    parent->add_on_command_callback([this, parent]() { trigger(); });
  }
};

}  // namespace homeassistant_media_player
}  // namespace esphome
