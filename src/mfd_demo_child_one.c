#include "../include/mfd_demo.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

struct mfd_demo_child_one_data {
	int demo_value;
};

static ssize_t demo_value_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct mfd_demo_child_one_data *data = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n", data->demo_value);
}

static ssize_t demo_value_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct mfd_demo_child_one_data *data = dev_get_drvdata(dev);
	int value;
	int ret;

	ret = kstrtoint(buf, 10, &value);
	if (ret)
		return ret;

	data->demo_value = value;
	return count;
}

static DEVICE_ATTR_RW(demo_value);

static int mfd_demo_child_one_probe(struct platform_device *pdev)
{
	struct mfd_demo_child_one_data *data;
	int ret;

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;
	data->demo_value = 10;
	platform_set_drvdata(pdev, data);

	ret = device_create_file(&pdev->dev, &dev_attr_demo_value);
	if (ret) {
		dev_err(&pdev->dev, "child-one failed to create sysfs node: %d\n", ret);
		return ret;
	}

	dev_info(&pdev->dev, "child-one probe successful\n");
	return 0;
}

static int mfd_demo_child_one_remove(struct platform_device *pdev)
{
	device_remove_file(&pdev->dev, &dev_attr_demo_value);
	dev_info(&pdev->dev, "child-one remove\n");
	return 0;
}

static struct platform_driver mfd_demo_child_one_driver = {
	.probe = mfd_demo_child_one_probe,
	.remove = mfd_demo_child_one_remove,
	.driver = {
		.name = "mfd-demo-child-one",
	},
};

int mfd_demo_child_one_register(void)
{
	return platform_driver_register(&mfd_demo_child_one_driver);
}

void mfd_demo_child_one_unregister(void)
{
	platform_driver_unregister(&mfd_demo_child_one_driver);
}

MODULE_LICENSE("GPL");
