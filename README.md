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
