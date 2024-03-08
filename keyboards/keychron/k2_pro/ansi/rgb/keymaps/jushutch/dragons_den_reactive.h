// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(DRAGONS_DEN_REACTIVE)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// e.g: A simple effect, self-contained within a single method
static bool DRAGONS_DEN_REACTIVE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV      dragonHSV = rgb_matrix_config.hsv;
    uint16_t time      = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);
    dragonHSV.v        = scale8(abs8(sin8(time) - 128) * 2, dragonHSV.v);
    RGB     dragonRGB  = rgb_matrix_hsv_to_rgb(dragonHSV);
    uint8_t count      = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        if (i != DRAGON_KEY_INDEX) {
            HSV hsv = rgb_matrix_config.hsv;
            hsv.v   = 0;
            for (uint8_t j = 0; j < count; j++) {
                int16_t  dx     = g_led_config.point[i].x - g_last_hit_tracker.x[j];
                int16_t  dy     = g_led_config.point[i].y - g_last_hit_tracker.y[j];
                uint8_t  dist   = sqrt16(dx * dx + dy * dy);
                uint16_t tick   = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));
                uint16_t effect = tick + dist * 5;
                if (effect > 255) effect = 255;
                hsv.v = qadd8(hsv.v, 255 - effect);
            }
            hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
            RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } else {
            rgb_matrix_set_color(i, dragonRGB.r, dragonRGB.g, dragonRGB.b);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS