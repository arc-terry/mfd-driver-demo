#ifndef _MFD_DEMO_H
#define _MFD_DEMO_H

#define MCU_SENSOR_A_ID 0
#define MCU_SENSOR_B_ID 1

int mcu_core_init(void);
void mcu_core_exit(void);
int mcu_core_sensor_read(int sensor_id, int *value);
int mcu_core_sensor_write(int sensor_id, int value);

int mfd_demo_sensor_a_register(void);
void mfd_demo_sensor_a_unregister(void);
int mfd_demo_sensor_b_register(void);
void mfd_demo_sensor_b_unregister(void);

#endif
