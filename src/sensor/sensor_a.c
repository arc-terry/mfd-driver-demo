#include "../../include/mfd_demo.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static ssize_t demo_value_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	int value;
	int ret;

	ret = mcu_core_sensor_read(MCU_SENSOR_A_ID, &value);
	if (ret)
		return ret;

	return sysfs_emit(buf, "%d\n", value);
}

static ssize_t demo_value_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	int value;
	int ret;

	ret = kstrtoint(buf, 10, &value);
	if (ret)
		return ret;

	ret = mcu_core_sensor_write(MCU_SENSOR_A_ID, value);
	if (ret)
		return ret;

	return count;
}

static DEVICE_ATTR_RW(demo_value);

static int mfd_demo_sensor_a_probe(struct platform_device *pdev)
{
	int ret;

	ret = device_create_file(&pdev->dev, &dev_attr_demo_value);
	if (ret) {
		dev_err(&pdev->dev, "sensor-a failed to create sysfs node: %d\n", ret);
		return ret;
	}

	dev_info(&pdev->dev, "sensor-a probe: MCU I2C channel ready\n");
	return 0;
}

static int mfd_demo_sensor_a_remove(struct platform_device *pdev)
{
	device_remove_file(&pdev->dev, &dev_attr_demo_value);
	dev_info(&pdev->dev, "sensor-a remove\n");
	return 0;
}

static struct platform_driver mfd_demo_sensor_a_driver = {
	.probe = mfd_demo_sensor_a_probe,
	.remove = mfd_demo_sensor_a_remove,
	.driver = {
		.name = "mfd-demo-sensor-a",
	},
};

int mfd_demo_sensor_a_register(void)
{
	return platform_driver_register(&mfd_demo_sensor_a_driver);
}

void mfd_demo_sensor_a_unregister(void)
{
	platform_driver_unregister(&mfd_demo_sensor_a_driver);
}

MODULE_LICENSE("GPL");
