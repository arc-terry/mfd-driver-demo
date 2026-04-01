#include <linux/init.h>
#include <linux/mfd/core.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include "../include/mfd_demo.h"

static const struct mfd_cell mfd_demo_cells[] = {
	{ .name = "mfd-demo-child-one" },
	{ .name = "mfd-demo-child-two" },
};

static struct platform_device *mfd_demo_pdev;

static int mfd_demo_parent_probe(struct platform_device *pdev)
{
	int ret;

	dev_info(&pdev->dev, "parent probe: registering child devices\n");

	ret = mfd_add_devices(&pdev->dev, PLATFORM_DEVID_AUTO,
			      mfd_demo_cells, ARRAY_SIZE(mfd_demo_cells),
			      NULL, 0, NULL);
	if (ret) {
		dev_err(&pdev->dev, "failed to add child devices: %d\n", ret);
		return ret;
	}

	return 0;
}

static void mfd_demo_parent_remove(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "parent remove: unregistering child devices\n");
	mfd_remove_devices(&pdev->dev);
}

static struct platform_driver mfd_demo_parent_driver = {
	.probe = mfd_demo_parent_probe,
	.remove_new = mfd_demo_parent_remove,
	.driver = {
		.name = "mfd-demo-parent",
	},
};

static int __init mfd_demo_init(void)
{
	int ret;

	ret = mfd_demo_child_one_register();
	if (ret)
		return ret;

	ret = mfd_demo_child_two_register();
	if (ret)
		goto err_child_two;

	ret = platform_driver_register(&mfd_demo_parent_driver);
	if (ret)
		goto err_parent_drv;

	mfd_demo_pdev = platform_device_register_simple("mfd-demo-parent",
							PLATFORM_DEVID_AUTO,
							NULL, 0);
	if (IS_ERR(mfd_demo_pdev)) {
		ret = PTR_ERR(mfd_demo_pdev);
		goto err_parent_dev;
	}

	pr_info("mfd_demo: module loaded\n");
	return 0;

err_parent_dev:
	platform_driver_unregister(&mfd_demo_parent_driver);
err_parent_drv:
	mfd_demo_child_two_unregister();
err_child_two:
	mfd_demo_child_one_unregister();
	return ret;
}

static void __exit mfd_demo_exit(void)
{
	platform_device_unregister(mfd_demo_pdev);
	platform_driver_unregister(&mfd_demo_parent_driver);
	mfd_demo_child_two_unregister();
	mfd_demo_child_one_unregister();
	pr_info("mfd_demo: module unloaded\n");
}

module_init(mfd_demo_init);
module_exit(mfd_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mfd-driver-demo");
MODULE_DESCRIPTION("Demo MFD module with one parent and two child drivers");
