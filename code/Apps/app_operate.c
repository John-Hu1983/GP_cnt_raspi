#include "../inc/includes.h"
static event_t pitch = {
    .words = 0,
};
const btn_io Btn_IO[] = {
    {{GPCE_PA, 0}, io_input_pulldown, TRUE, 10, 50},
    {{GPCE_PA, 1}, io_input_pulldown, TRUE, 10, 50},
    // {{GPCE_PA, 2}, io_input_pulldown, TRUE, 10, 50},
    // {{GPCE_PA, 3}, io_input_pulldown, TRUE, 10, 50},
    {{GPCE_PA, 4}, io_input_pulldown, TRUE, 10, 50},
    {{GPCE_PA, 5}, io_input_pulldown, TRUE, 10, 50},
    {{GPCE_PA, 6}, io_input_pulldown, TRUE, 10, 50},
    {{GPCE_PA, 7}, io_input_pulldown, TRUE, 10, 50},
};
const u16 Btn_Amount = sizeof(Btn_IO) / sizeof(Btn_IO[0]);
btn_obj bt[sizeof(Btn_IO) / sizeof(Btn_IO[0])];

/*
 * argument in  :
 * argument out :
 * description  :
 */
void init_operate_app(void)
{
  u16 i;
  for (i = 0; i < Btn_Amount; i++)
  {
    gpio_init_pattern(Btn_IO[i].io.port, Btn_IO[i].io.pin, Btn_IO[i].patt);
    memset((void *)&bt[i], 0, sizeof(bt[i]));
    bt[i].pin = (btn_io *)&Btn_IO[i];
  }
}

/*
 * argument in  :
 * argument out :
 * description  :
 */
enum
{
  _init_ = 0,
};
void task_operate_event(osvar_t ms)
{
  btnval_t btn_v[sizeof(Btn_IO) / sizeof(Btn_IO[0])];
  u8 buf[64];
  u16 i;

  if (pitch.bits.init_1b == 0)
  {
    pitch.bits.init_1b = 1;
    init_operate_app();
    return;
  }

  for (i = 0; i < Btn_Amount; i++)
  {
    btn_v[i] = scan_btn_object((btn_obj *)&bt[i], ms);
    if (btn_v[i] != BTN_IDLE)
    {
      memset(buf, '\0', sizeof(buf));
      sprintf(buf, "AT+IOA%d:%s", i, (btn_v[i] == BTN_CLICK) ? "click\n" : "hold\n");
      insert_tx_fifo(buf);
    }
  }
  pitch.bits.ms_8b = (pitch.bits.ms_8b < ms) ? 0 : pitch.bits.ms_8b - ms;
}
