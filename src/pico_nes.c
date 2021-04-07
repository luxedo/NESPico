/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board.h"
#include "hardware/gpio.h"
#include "tusb.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

enum {
  LOW = 0,
  HIGH = 1,
  READ_SLEEP_MS = 1,
  PIN_CLOCK = 4,
  PIN_LATCH = 5,
  PIN_DATA = 6,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

/*------------- MAIN -------------*/
int main(void) {
  board_init();
  tusb_init();

  gpio_init(PIN_CLOCK);
  gpio_set_dir(PIN_CLOCK, GPIO_OUT);
  gpio_init(PIN_LATCH);
  gpio_set_dir(PIN_LATCH, GPIO_OUT);
  gpio_init(PIN_DATA);
  gpio_set_dir(PIN_DATA, GPIO_IN);

  while (1) {
    hid_task();
    tud_task(); // tinyusb device task
    led_blinking_task();
  }

  return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) { blink_interval_ms = BLINK_MOUNTED; }

// Invoked when device is unmounted
void tud_umount_cb(void) { blink_interval_ms = BLINK_NOT_MOUNTED; }

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
  (void)remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) { blink_interval_ms = BLINK_MOUNTED; }

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

uint8_t read_nes_controller(void) {
  uint8_t input = 0;
  gpio_put(PIN_LATCH, LOW);
  gpio_put(PIN_CLOCK, LOW);

  gpio_put(PIN_LATCH, HIGH);
  sleep_ms(READ_SLEEP_MS);
  gpio_put(PIN_LATCH, LOW);
  for (uint32_t i = 0; i < 7; i++) {
    gpio_put(PIN_CLOCK, HIGH);
    sleep_ms(READ_SLEEP_MS);
    gpio_put(PIN_CLOCK, LOW);
    sleep_ms(READ_SLEEP_MS);
    input |= gpio_get(PIN_DATA) << i;
  }

  return input;
  //    latch.value = True
  //    sleep(delaytime)
  //    latch.value = False
  //    sleep(delaytime)
  //    button_status[buttons[0]] = data.value
  //    for x in range(0, 7, 1):
  //        clock.value = True
  //        sleep(delaytime)
  //        clock.value = False
  //        sleep(delaytime)
  //        button_status[buttons[x + 1]] = data.value

  //   byte controllerRead() {
  //   byte controller_data = 0;
  //   digitalWrite(latch,LOW);
  //   digitalWrite(clock,LOW);
  //
  //   digitalWrite(latch,HIGH);
  //   delayMicroseconds(2);
  //   digitalWrite(latch,LOW);
  //
  //   controller_data = digitalRead(data);
  //   for (int i = 1; i <= 7; i++) {
  //     digitalWrite(clock, HIGH);
  //     delayMicroseconds(2);
  //     controller_data = controller_data << 1;
  //     controller_data = controller_data + digitalRead(data);
  //     delayMicroseconds(4);
  //     digitalWrite(clock, LOW);
  //   }
  //   return 255 - controller_data;
  // }
  return 0;
}
void update_nes_report(hid_gamepad_report_t *report, uint8_t input) {
  // Get button data
  if ("button A")
    report->buttons |= GAMEPAD_BUTTON_A;
  if ("button B")
    report->buttons |= GAMEPAD_BUTTON_B;
  if ("button start")
    report->buttons |= GAMEPAD_BUTTON_START;
  if ("button select")
    report->buttons |= GAMEPAD_BUTTON_SELECT;

  // Get hat data
  if ("hat up")
    if ("hat left")
      report->hat = GAMEPAD_HAT_UP_LEFT;
    else if ("hat right")
      report->hat = GAMEPAD_HAT_UP_RIGHT;
    else
      report->hat = GAMEPAD_HAT_UP;
  else if ("hat down")
    if ("hat left")
      report->hat = GAMEPAD_HAT_DOWN_LEFT;
    else if ("hat right")
      report->hat = GAMEPAD_HAT_DOWN_RIGHT;
    else
      report->hat = GAMEPAD_HAT_DOWN;
  else if ("hat left")
    report->hat = GAMEPAD_HAT_LEFT;
  else if ("hat right")
    report->hat = GAMEPAD_HAT_RIGHT;
}
// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc
// ..) tud_hid_report_complete_cb() is used to send the next report after
// previous one is complete
void hid_task(void) {
  // Poll every 10ms
  const uint32_t interval_ms = 1000;
  static uint32_t start_ms = 0;
  static hid_gamepad_report_t report;
  static hid_gamepad_report_t current_state = {
      .buttons = 0,
      .x = 0,
      .y = 0,
      .z = 0,
      .rx = 0,
      .ry = 0,
      .rz = 0,
      .hat = 0,
  };

  if (board_millis() - start_ms < interval_ms)
    return; // not enough time
  start_ms += interval_ms;

  // Remote wakeup
  if (tud_suspended()) {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  }
  if (tud_hid_ready()) {
    report.buttons = 0;
    report.x = 0;
    report.y = 0;
    report.z = 0;
    report.rx = 0;
    report.ry = 0;
    report.rz = 0;
    report.hat = 0;

    uint8_t input = read_nes_controller();
    update_nes_report(&report, input);

    // use to avoid send multiple consecutive zero report for keyboard
    if (current_state.hat != report.hat ||
        current_state.buttons != report.buttons) {
      // Send report
      tud_hid_n_gamepad_report(0, 1, report.x, report.y, report.z, report.rz,
                               report.rx, report.ry, report.hat,
                               report.buttons);
      memcpy(&current_state, &report, sizeof(report));
    }
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
  // TODO not Implemented
  (void)itf;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
  // TODO set LED based on CAPLOCK, NUMLOCK etc...
  (void)itf;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)bufsize;

  // echo back anything we received from host
  tud_hid_report(0, buffer, bufsize);
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void) {
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if (board_millis() - start_ms < blink_interval_ms)
    return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
