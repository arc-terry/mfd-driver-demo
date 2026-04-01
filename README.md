# MFD Driver Demo

A minimal out-of-tree Linux kernel module demo with:
- one parent MFD driver
- two child platform drivers created via MFD cells

## Prerequisites
- Linux kernel headers installed for `uname -r`
- build tools (`make`, `gcc`)
- root access for module load/unload

## Build
```bash
make
```

## Demo run
```bash
sudo insmod mfd_demo.ko
dmesg | tail -n 20
```

Expected dmesg includes:
- `mfd_demo: module loaded`
- `parent probe: registering child devices`
- `child-one probe successful`
- `child-two probe successful`

## Sysfs read/write demo
Find parent and child sysfs nodes:

```bash
find /sys/devices -path "*mfd-demo*/demo_value" 2>/dev/null | sort
```

Typical paths look like:
- parent: `/sys/devices/platform/mfd-demo-parent.*/demo_value`
- child one: `/sys/devices/platform/mfd-demo-parent.*/mfd-demo-child-one.*/demo_value`
- child two: `/sys/devices/platform/mfd-demo-parent.*/mfd-demo-child-two.*/demo_value`

Read current values:

```bash
for f in $(find /sys/devices -path "*mfd-demo*/demo_value" 2>/dev/null | sort); do echo "$f=$(cat "$f")"; done
```

Write and verify:

```bash
PARENT=$(find /sys/devices -path "*mfd-demo-parent.*/demo_value" | grep -v "mfd-demo-child-" | head -n1)
CH1=$(find /sys/devices -path "*mfd-demo-child-one.*/demo_value" | head -n1)
CH2=$(find /sys/devices -path "*mfd-demo-child-two.*/demo_value" | head -n1)
echo 111 | sudo tee "$PARENT" >/dev/null
echo 222 | sudo tee "$CH1" >/dev/null
echo 333 | sudo tee "$CH2" >/dev/null
cat "$PARENT" "$CH1" "$CH2"
```

## Teardown
```bash
sudo rmmod mfd_demo
dmesg | tail -n 20
```

Expected teardown logs include:
- `child-one remove`
- `child-two remove`
- `parent remove: unregistering child devices`
- `mfd_demo: module unloaded`
