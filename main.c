#include "lvgl/lvgl.h"
#include <unistd.h>
#include <stdio.h>

/* UI Component References for Callbacks */
static lv_obj_t *chart;
static lv_chart_series_t *ser1;
static lv_chart_series_t *ser2;
static lv_obj_t *val_label;

/* Timer callback to simulate real-time data streaming */
static void chart_update_cb(lv_timer_t *timer)
{
    LV_UNUSED(timer);
    /* Push new random data points into the chart */
    lv_chart_set_next_value(chart, ser1, lv_rand(20, 85));
    lv_chart_set_next_value(chart, ser2, lv_rand(40, 65));
}

/* Slider event callback */
static void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t val = lv_slider_get_value(slider);
    
    char buf[16];
    lv_snprintf(buf, sizeof(buf), "%d °C", (int)val);
    lv_label_set_text(val_label, buf);
}

int main(void)
{
    lv_init();

    /* 1. Create Display (800x480) using SDL driver */
    lv_display_t *disp = lv_sdl_window_create(800, 480);
    if (!disp) {
        fprintf(stderr, "Failed to create SDL display window!\n");
        return 1;
    }

    lv_indev_t *mouse = lv_sdl_mouse_create();

    /* Active Screen Configuration */
    lv_obj_t *scr = lv_display_get_screen_active(disp);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x0F172A), 0); /* Dark slate background */
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(scr, 8, 0);
    lv_obj_set_style_pad_gap(scr, 8, 0);

    /* Prevent root screen from scrolling vertically */
    lv_obj_set_scroll_dir(scr, LV_DIR_NONE);
    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);

    /* ------------------------------------------------------------------ */
    /* 2. Top Header Bar (800x40 equivalent)                               */
    /* ------------------------------------------------------------------ */
    lv_obj_t *header = lv_obj_create(scr);
    lv_obj_set_size(header, LV_PCT(100), 40);
    lv_obj_set_style_bg_color(header, lv_color_hex(0x1E293B), 0);
    lv_obj_set_style_border_width(header, 0, 0);
    lv_obj_set_style_radius(header, 6, 0);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_hor(header, 16, 0);

    lv_obj_t *title = lv_label_create(header);
    lv_label_set_text(title, "SYSTEM MONITOR");
    lv_obj_set_style_text_color(title, lv_color_hex(0xF8FAFC), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);

    lv_obj_t *status = lv_label_create(header);
    lv_label_set_text(status, "Status: RUNNING | Wayland 800x480");
    lv_obj_set_style_text_color(status, lv_color_hex(0x22C55E), 0); /* Green indicator */

    /* ------------------------------------------------------------------ */
    /* 3. Main Content Container (800x416 - Flex Row Split)               */
    /* ------------------------------------------------------------------ */
    lv_obj_t *main_cnt = lv_obj_create(scr);
    lv_obj_set_width(main_cnt, LV_PCT(100));
    lv_obj_set_flex_grow(main_cnt, 1); /* Takes remaining Y-space cleanly */
    lv_obj_set_style_bg_opa(main_cnt, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(main_cnt, 0, 0);
    lv_obj_set_style_pad_all(main_cnt, 0, 0);
    lv_obj_set_style_pad_gap(main_cnt, 8, 0);
    lv_obj_set_flex_flow(main_cnt, LV_FLEX_FLOW_ROW);

    /* ------------------------------------------------------------------ */
    /* 4. Left Panel: Real-time Chart (500px wide)                        */
    /* ------------------------------------------------------------------ */
    lv_obj_t *chart_card = lv_obj_create(main_cnt);
    lv_obj_set_size(chart_card, 500, LV_PCT(100));
    lv_obj_set_style_bg_color(chart_card, lv_color_hex(0x1E293B), 0);
    lv_obj_set_style_border_width(chart_card, 0, 0);
    lv_obj_set_style_radius(chart_card, 6, 0);

    chart = lv_chart_create(chart_card);
    lv_obj_set_size(chart, LV_PCT(100), LV_PCT(100));
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, 20);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Style the Chart */
    lv_obj_set_style_bg_color(chart, lv_color_hex(0x0F172A), 0);
    lv_obj_set_style_border_color(chart, lv_color_hex(0x334155), 0);
    lv_obj_set_style_line_color(chart, lv_color_hex(0x334155), LV_PART_MAIN);

    /* Data Series */
    ser1 = lv_chart_add_series(chart, lv_color_hex(0x38BDF8), LV_CHART_AXIS_PRIMARY_Y); /* Sky blue */
    ser2 = lv_chart_add_series(chart, lv_color_hex(0xF43F5E), LV_CHART_AXIS_PRIMARY_Y); /* Rose red */

    /* ------------------------------------------------------------------ */
    /* 5. Right Panel: Control Panel (276px wide)                         */
    /* ------------------------------------------------------------------ */
    lv_obj_t *ctrl_card = lv_obj_create(main_cnt);
    lv_obj_set_size(ctrl_card, 276, LV_PCT(100));
    lv_obj_set_style_bg_color(ctrl_card, lv_color_hex(0x1E293B), 0);
    lv_obj_set_style_border_width(ctrl_card, 0, 0);
    lv_obj_set_style_radius(ctrl_card, 6, 0);
    lv_obj_set_flex_flow(ctrl_card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_gap(ctrl_card, 16, 0);

    /* Card Title */
    lv_obj_t *ctrl_title = lv_label_create(ctrl_card);
    lv_label_set_text(ctrl_title, "CONTROLS");
    lv_obj_set_style_text_color(ctrl_title, lv_color_hex(0x94A3B8), 0);

    /* Toggle Switch Row */
    lv_obj_t *sw_row = lv_obj_create(ctrl_card);
    lv_obj_set_size(sw_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(sw_row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(sw_row, 0, 0);
    lv_obj_set_style_pad_all(sw_row, 0, 0);
    lv_obj_set_flex_flow(sw_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(sw_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *sw_label = lv_label_create(sw_row);
    lv_label_set_text(sw_label, "Pump Power");
    lv_obj_set_style_text_color(sw_label, lv_color_hex(0xF8FAFC), 0);

    lv_obj_t *sw = lv_switch_create(sw_row);
    lv_obj_add_state(sw, LV_STATE_CHECKED);

    /* Target Setpoint Slider */
    lv_obj_t *slider_lbl = lv_label_create(ctrl_card);
    lv_label_set_text(slider_lbl, "Set Temperature");
    lv_obj_set_style_text_color(slider_lbl, lv_color_hex(0xF8FAFC), 0);

    lv_obj_t *slider = lv_slider_create(ctrl_card);
    lv_obj_set_width(slider, LV_PCT(100));
    lv_slider_set_range(slider, 10, 90);
    lv_slider_set_value(slider, 45, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    val_label = lv_label_create(ctrl_card);
    lv_label_set_text(val_label, "45 °C");
    lv_obj_set_style_text_color(val_label, lv_color_hex(0x38BDF8), 0);
    lv_obj_set_style_text_font(val_label, &lv_font_montserrat_14, 0);

    /* ------------------------------------------------------------------ */
    /* 6. Register Timers & Run Event Loop                                */
    /* ------------------------------------------------------------------ */
    lv_timer_create(chart_update_cb, 500, NULL); /* Refresh chart every 500ms */

    while (1) {
        uint32_t time_till_next = lv_timer_handler();
        usleep(time_till_next * 1000);
    }

    return 0;
}
