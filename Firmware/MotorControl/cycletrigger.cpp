#include <odrive_main.h>

CycleTrigger::CycleTrigger(CycleTrigger::Config_t &config) : config_(config) {
    reload_enabled();
}

void CycleTrigger::set_enabled(bool enable) {
    uint16_t gpio_pin = get_gpio_pin_by_pin(config_.gpio_pin_num);
    GPIO_TypeDef* gpio_port = get_gpio_port_by_pin(config_.gpio_pin_num);
    if (enable) {
        last_edge_hit_.has_hit = false;
        last_edge_hit_.hit_location = 0;
        HAL_GPIO_DeInit(gpio_port, gpio_pin);
        GPIO_subscribe(gpio_port, gpio_pin,
                       config_.invert_pin ? GPIO_PULLUP : GPIO_PULLDOWN,
                       [](void *ctx) { static_cast<CycleTrigger *>(ctx)->trigger(); }, this);
    }
    else {
        GPIO_unsubscribe(gpio_port, gpio_pin);
    }
    config_.enabled = enable;
}

void CycleTrigger::reload_enabled() {
    set_enabled(config_.enabled);
}

void CycleTrigger::trigger() {
    last_edge_hit_.has_hit = true;
    last_edge_hit_.hit_location = axis_->encoder_.shadow_count_;
}
