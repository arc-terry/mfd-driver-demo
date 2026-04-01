#include "../include/mfd_demo.h"
#include <linux/module.h>
#include <linux/platform_device.h>

static int mfd_demo_child_one_probe(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "child-one probe successful\n");
	return 0;
}

static int mfd_demo_child_one_remove(struct platform_device *pdev)
{
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
