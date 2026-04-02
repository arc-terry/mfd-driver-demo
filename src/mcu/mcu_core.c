#include "../../include/mfd_demo.h"
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>

struct mcu_core_state {
	struct mutex lock;
	int sensor_values[2];
};

static struct mcu_core_state mcu_state;

int mcu_core_init(void)
{
	mutex_init(&mcu_state.lock);
	mcu_state.sensor_values[MCU_SENSOR_A_ID] = 10;
	mcu_state.sensor_values[MCU_SENSOR_B_ID] = 20;
	pr_info("mfd_demo: MCU core initialized (simulated UART<->I2C bridge)\n");
	return 0;
}

void mcu_core_exit(void)
{
	pr_info("mfd_demo: MCU core exited\n");
}

int mcu_core_sensor_read(int sensor_id, int *value)
{
	if (!value)
		return -EINVAL;
	if (sensor_id != MCU_SENSOR_A_ID && sensor_id != MCU_SENSOR_B_ID)
		return -EINVAL;

	mutex_lock(&mcu_state.lock);
	*value = mcu_state.sensor_values[sensor_id];
	mutex_unlock(&mcu_state.lock);

	return 0;
}

int mcu_core_sensor_write(int sensor_id, int value)
{
	if (sensor_id != MCU_SENSOR_A_ID && sensor_id != MCU_SENSOR_B_ID)
		return -EINVAL;

	mutex_lock(&mcu_state.lock);
	mcu_state.sensor_values[sensor_id] = value;
	mutex_unlock(&mcu_state.lock);

	return 0;
}

MODULE_LICENSE("GPL");
