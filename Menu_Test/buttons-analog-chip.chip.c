// Wokwi Custom Chip - For information and examples see:
// https://link.wokwi.com/custom-chips-alpha
//https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright (C) 2022 Uri Shaked / wokwi.com

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin_in[5];
  pin_t pin_out;
} chip_state_t;

static void chip_pin_change(void *user_data, pin_t pin, uint32_t value);

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  chip->pin_in[0] = pin_init("IN1", INPUT_PULLUP);
  chip->pin_in[1] = pin_init("IN2", INPUT_PULLUP);
  chip->pin_in[2] = pin_init("IN3", INPUT_PULLUP);
  chip->pin_in[3] = pin_init("IN4", INPUT_PULLUP);
  chip->pin_in[4] = pin_init("IN5", INPUT_PULLUP);
  chip->pin_out = pin_init("OUT", ANALOG);

  const pin_watch_config_t config = {
    .edge = BOTH,
    .pin_change = chip_pin_change,
    .user_data = chip,
  };
  for (int i = 0; i < 5; i++) {
    pin_watch(chip->pin_in[i], &config);
  }
  pin_dac_write(chip->pin_out, 5);

  // The following message will appear in the browser's DevTools console:
  //printf("Hello from buttons-to-analog chip!\n");
}

void chip_pin_change(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t*)user_data;
  if (pin_read(chip->pin_in[0]) == LOW) {
    pin_dac_write(chip->pin_out, 0);
  } else if (pin_read(chip->pin_in[1]) == LOW) {
    pin_dac_write(chip->pin_out, 144. / 1024 * 5);
  } else if (pin_read(chip->pin_in[2]) == LOW) {
    pin_dac_write(chip->pin_out, 329. / 1024 * 5);
  } else if (pin_read(chip->pin_in[3]) == LOW) {
    pin_dac_write(chip->pin_out, 504. / 1024 * 5);
  } else if (pin_read(chip->pin_in[4]) == LOW) {
    pin_dac_write(chip->pin_out, 741. / 1024 * 5);
  } else {
    pin_dac_write(chip->pin_out, 5);
  }
}
