#include "../../include/mfd_demo.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mfd/core.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static const struct mfd_cell soc_uart_mcu_cells[] = {
	{ .name = "mfd-demo-sensor-a" },
	{ .name = "mfd-demo-sensor-b" },
};

static struct platform_device *soc_uart_pdev;

struct soc_uart_parent_data {
	int uart_link_state;
};

static ssize_t demo_value_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct soc_uart_parent_data *data = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n", data->uart_link_state);
}

static ssize_t demo_value_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct soc_uart_parent_data *data = dev_get_drvdata(dev);
	int value;
	int ret;

	ret = kstrtoint(buf, 10, &value);
	if (ret)
		return ret;

	data->uart_link_state = value;
	return count;
}

static DEVICE_ATTR_RW(demo_value);

static int soc_uart_parent_probe(struct platform_device *pdev)
{
	struct soc_uart_parent_data *data;
	int ret;

	dev_info(&pdev->dev, "SoC UART bridge probe: creating MCU sensor functions\n");

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;
	data->uart_link_state = 1;
	platform_set_drvdata(pdev, data);

	ret = mfd_add_devices(&pdev->dev, PLATFORM_DEVID_AUTO,
			      soc_uart_mcu_cells, ARRAY_SIZE(soc_uart_mcu_cells),
			      NULL, 0, NULL);
	if (ret) {
		dev_err(&pdev->dev, "failed to add sensor child devices: %d\n", ret);
		return ret;
	}

	ret = device_create_file(&pdev->dev, &dev_attr_demo_value);
	if (ret) {
		mfd_remove_devices(&pdev->dev);
		dev_err(&pdev->dev, "failed to create SoC UART sysfs node: %d\n", ret);
		return ret;
	}

	return 0;
}

static void soc_uart_parent_remove(struct platform_device *pdev)
{
	device_remove_file(&pdev->dev, &dev_attr_demo_value);
	dev_info(&pdev->dev, "SoC UART bridge remove: deleting MCU sensor functions\n");
	mfd_remove_devices(&pdev->dev);
}

static struct platform_driver soc_uart_parent_driver = {
	.probe = soc_uart_parent_probe,
	.remove_new = soc_uart_parent_remove,
	.driver = {
		.name = "mfd-demo-soc-uart-parent",
	},
};

static int __init mfd_demo_init(void)
{
	int ret;

	ret = mcu_core_init();
	if (ret)
		return ret;

	ret = mfd_demo_sensor_a_register();
	if (ret)
		goto err_sensor_a;

	ret = mfd_demo_sensor_b_register();
	if (ret)
		goto err_sensor_b;

	ret = platform_driver_register(&soc_uart_parent_driver);
	if (ret)
		goto err_parent_drv;

	soc_uart_pdev = platform_device_register_simple("mfd-demo-soc-uart-parent",
							PLATFORM_DEVID_AUTO,
							NULL, 0);
	if (IS_ERR(soc_uart_pdev)) {
		ret = PTR_ERR(soc_uart_pdev);
		goto err_parent_dev;
	}

	pr_info("mfd_demo: SoC-UART-MCU-I2C demo module loaded\n");
	return 0;

err_parent_dev:
	platform_driver_unregister(&soc_uart_parent_driver);
err_parent_drv:
	mfd_demo_sensor_b_unregister();
err_sensor_b:
	mfd_demo_sensor_a_unregister();
err_sensor_a:
	mcu_core_exit();
	return ret;
}

static void __exit mfd_demo_exit(void)
{
	platform_device_unregister(soc_uart_pdev);
	platform_driver_unregister(&soc_uart_parent_driver);
	mfd_demo_sensor_b_unregister();
	mfd_demo_sensor_a_unregister();
	mcu_core_exit();
	pr_info("mfd_demo: SoC-UART-MCU-I2C demo module unloaded\n");
}

module_init(mfd_demo_init);
module_exit(mfd_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mfd-driver-demo");
MODULE_DESCRIPTION("SoC UART to MCU I2C sensor MFD demo");
