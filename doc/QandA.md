# Q and A

## Q1. Runtime sysfs tree (typical)?

```text
/sys/devices/platform/mfd-demo-soc-uart-parent.X.auto/demo_value
/sys/devices/platform/mfd-demo-soc-uart-parent.X.auto/mfd-demo-sensor-a.Y.auto/demo_value
/sys/devices/platform/mfd-demo-soc-uart-parent.X.auto/mfd-demo-sensor-b.Z.auto/demo_value
```

## Q2. Why do node names use variable `X`, `Y`, and `Z`?

`X`, `Y`, and `Z` are runtime-assigned instance IDs added by the Linux device core when platform/MFD devices are created with automatic IDs (`PLATFORM_DEVID_AUTO`).

- `X` is the parent SoC-UART device instance number.
- `Y` is the sensor-a child instance number.
- `Z` is the sensor-b child instance number.

These numbers are **not fixed** and can change across boots or module reloads depending on probe/load order and what other platform devices already exist.

So in scripts and docs, treat `X/Y/Z` as wildcards and discover real paths at runtime, for example:

```bash
find /sys/devices -path "*mfd-demo*/demo_value" 2>/dev/null | sort
```
