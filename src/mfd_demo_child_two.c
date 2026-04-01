#include "../include/mfd_demo.h"
#include <linux/module.h>
#include <linux/platform_device.h>

static int mfd_demo_child_two_probe(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "child-two probe successful\n");
	return 0;
}

static int mfd_demo_child_two_remove(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "child-two remove\n");
	return 0;
}

static struct platform_driver mfd_demo_child_two_driver = {
	.probe = mfd_demo_child_two_probe,
	.remove = mfd_demo_child_two_remove,
	.driver = {
		.name = "mfd-demo-child-two",
	},
};

int mfd_demo_child_two_register(void)
{
	return platform_driver_register(&mfd_demo_child_two_driver);
}

void mfd_demo_child_two_unregister(void)
{
	platform_driver_unregister(&mfd_demo_child_two_driver);
}

MODULE_LICENSE("GPL");
