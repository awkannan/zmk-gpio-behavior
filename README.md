# zmk-gpio-behavior
GPIO toggle behavior in ZMK

Add `#include "control_gpio_behavior.dtsi"` to your keyboard's DTS file

Define a zmk.control-gpio compatible in your dts

gpios are mandatory for each child, starting-state-on defines the initial state of the GPIO at boot time

Example:
```
    leds: leds {
        compatible = "zmk,control-gpio";
        chg_led: led_0 {
            gpios = <&gpio1 12 GPIO_ACTIVE_HIGH>;
            starting-state-on;
        };
        blue_led: led_1 {
            gpios = <&gpio0 26 GPIO_ACTIVE_HIGH>;
        };
    };

```

This will enable the following keypress behaviors:

- &gpc GP_ON (index) - turn GPIO on
- &gpc GP_OFF (index) - turn GPIO off
- &gpc GP_TOG (index) - toggle GPIO

The index is defined by the order your GPIOS are defined in the devicetree entry

In this case, chg_led would be index 0 and blue_led would be index 1