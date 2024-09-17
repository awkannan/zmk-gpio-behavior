# zmk-gpio-behavior
GPIO toggle behavior in ZMK

Add `#include "control_gpio_behavior.dtsi"` to your keyboard's DTS file

Define a gpio-leds compatible in your dts
Mark it as chosen zmk,control-gpio

Example:
```
    chosen {
        zmk,control-gpio = &leds;
    };

    leds: leds {
        compatible = "gpio-leds";
        chg_led: led_0 {
            gpios = <&gpio1 12 GPIO_ACTIVE_HIGH>;
        };
        blue_led: led_1 {
            gpios = <&gpio0 26 GPIO_ACTIVE_HIGH>;
        };
    };

```

This will enable the following keypress behaviors:

&gpc GP_ON (index)
&gpc GP_OFF (index)
&gpc GP_TOG (index)

The index is defined by the order your leds are defined in the chosen devicetree entry
In this case, chg_led would be index 0 and blue_led would be index 1