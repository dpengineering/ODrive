#ifndef __CYCLETRIGGER_HPP
#define __CYCLETRIGGER_HPP

class CycleTrigger {
public:
    struct Config_t {
        bool enabled = false;
        uint16_t gpio_pin_num = 0xFFFF;
        bool invert_pin = true;
    };

    struct Hit_t {
        int32_t hit_location = 0;
        bool has_hit = false;
    };

    CycleTrigger(CycleTrigger::Config_t &config);

    CycleTrigger::Config_t &config_;
    Axis *axis_ = nullptr;

    void set_enabled(bool enabled);
    void reload_enabled();

    void trigger();
    bool get_state();

    bool trigger_state_ = false;

    Hit_t last_edge_hit_;

    auto make_protocol_definitions() {
        return make_protocol_member_list(
                   make_protocol_ro_property("trigger_state", &trigger_state_),
                   make_protocol_object("config",
                       make_protocol_property("enabled", &config_.enabled,
                           [](void *ctx) { static_cast<CycleTrigger *>(ctx)->reload_enabled(); }, this),
                       make_protocol_property("gpio_pin_num", &config_.gpio_pin_num),
                       make_protocol_property("invert_pin", &config_.invert_pin)),
                   make_protocol_object("last_edge_hit",
                       make_protocol_ro_property("hit_location", &last_edge_hit_.hit_location),
                       make_protocol_ro_property("has_hit", &last_edge_hit_.has_hit)));
    }
};
#endif
