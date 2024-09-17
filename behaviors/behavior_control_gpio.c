#define DT_DRV_COMPAT zmk_control_gpio

// Dependencies
#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include <zmk/behavior.h>

#include <dt-bindings/zmk/control_gpio.h>
#include <zmk/control_gpio.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

#define CHOSEN_DEVICE DT_CHOSEN(DT_DRV_COMPAT)
#define DT_GPIO_SPEC_GPIOS(n) GPIO_DT_SPEC_GET(n, gpios)

static const struct gpio_dt_spec gpio_devices[] = {
    DT_FOREACH_CHILD_SEP(CHOSEN_DEVICE, DT_GPIO_SPEC_GPIOS , (,))
};

#define NUM_CONTROLLED_GPIO ARRAY_SIZE(gpio_devices)


// The functions that do work

int zmk_gpio_on(int idx){
    if (idx >= NUM_CONTROLLED_GPIO){
        return -1;
    } 
    gpio_pin_set_dt(&gpio_devices[idx], 1);
    return 0;
}

int zmk_gpio_off(int idx){
    if (idx >= NUM_CONTROLLED_GPIO){
        return -1;
    } 
    gpio_pin_set_dt(&gpio_devices[idx], 0);
    return 0;
}

int zmk_gpio_toggle(int idx){
    if (idx >= NUM_CONTROLLED_GPIO){
        return -1;
    }
    return gpio_pin_toggle_dt(&gpio_devices[idx]);
}

// Event Listener Things

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    switch (binding->param1) {
    case GPIO_ON_CMD:
        return zmk_gpio_on(binding->param2);
    case GPIO_OFF_CMD:
        return zmk_gpio_off(binding->param2);
    case GPIO_TOGG_CMD:
        return zmk_gpio_toggle(binding->param2);
    default:
        LOG_ERR("Unknown control gpio command: %d", binding->param1);
    }

    return -ENOTSUP;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}



// Driver Code
// Initialization Function
static int control_gpio_init(const struct device *dev) {
    //hardcode - first GPIO turns on
    if(NUM_CONTROLLED_GPIO > 0){
        gpio_pin_set_dt(&gpio_devices[0], 1);
    }
    //hardcode- other GPIOs turn off
    for(uint8_t i=1;i<NUM_CONTROLLED_GPIO;i++){
        gpio_pin_set_dt(&gpio_devices[i], 0);
    }
    return 0;
};

// API Structure
static const struct behavior_driver_api control_gpio_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    .locality = BEHAVIOR_LOCALITY_GLOBAL
};

BEHAVIOR_DT_INST_DEFINE(0,                                                    // Instance Number (Equal to 0 for behaviors that don't require multiple instances,
                                                                              //                  Equal to n for behaviors that do make use of multiple instances)
                        control_gpio_init, NULL,                           // Initialization Function, Power Management Device Pointer
                        NULL, NULL,       // Behavior Data Pointer, Behavior Configuration Pointer (Both Optional)
                        POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,     // Initialization Level, Device Priority
                        &control_gpio_driver_api);                         // API Structure

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
