# MFD Driver Demo

This repository demonstrates a layered Linux MFD design for:

- **SoC -> UART -> MCU -> I2C -> Sensor A/Sensor B**

The module keeps SoC/UART, MCU core, and sensor-client responsibilities separated while remaining easy to run on a single Linux host.

Detailed architecture and folder mapping: **`doc/overview.md`**.

## Prerequisites

- Linux kernel headers for `uname -r`
- `make` and `gcc`
- root privileges for module load/unload

## Build

```bash
make clean && make
```

## Load and quick check

```bash
sudo insmod mfd_demo.ko
sudo dmesg | tail -n 30
```

Expected logs include:
- `mfd_demo: SoC-UART-MCU-I2C demo module loaded`
- `SoC UART bridge probe: creating MCU sensor functions`
- `sensor-a probe: MCU I2C channel ready`
- `sensor-b probe: MCU I2C channel ready`

## Sysfs read/write demo

Locate all demo nodes:

```bash
find /sys/devices -path "*mfd-demo*/demo_value" 2>/dev/null | sort
```

Typical runtime paths:
- parent (SoC UART bridge): `/sys/devices/platform/mfd-demo-soc-uart-parent.*/demo_value`
- sensor A: `/sys/devices/platform/mfd-demo-soc-uart-parent.*/mfd-demo-sensor-a.*/demo_value`
- sensor B: `/sys/devices/platform/mfd-demo-soc-uart-parent.*/mfd-demo-sensor-b.*/demo_value`

Read current values:

```bash
for f in $(find /sys/devices -path "*mfd-demo*/demo_value" 2>/dev/null | sort); do
  echo "$f=$(cat "$f")"
done
```

Write and verify:

```bash
PARENT=$(find /sys/devices -path "*mfd-demo-soc-uart-parent.*/demo_value" | grep -v "mfd-demo-sensor-" | head -n1)
SENSOR_A=$(find /sys/devices -path "*mfd-demo-sensor-a.*/demo_value" | head -n1)
SENSOR_B=$(find /sys/devices -path "*mfd-demo-sensor-b.*/demo_value" | head -n1)

echo 111 | sudo tee "$PARENT" >/dev/null
echo 222 | sudo tee "$SENSOR_A" >/dev/null
echo 333 | sudo tee "$SENSOR_B" >/dev/null

cat "$PARENT" "$SENSOR_A" "$SENSOR_B"
```

## Teardown

```bash
sudo rmmod mfd_demo
sudo dmesg | tail -n 30
```

Expected unload logs include:
- `sensor-a remove`
- `sensor-b remove`
- `SoC UART bridge remove: deleting MCU sensor functions`
- `mfd_demo: SoC-UART-MCU-I2C demo module unloaded`
