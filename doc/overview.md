# Overview: SoC UART -> MCU -> I2C Sensors

This demo models:
- SoC side driver communicating with MCU over UART semantics
- MCU core mediating requests to two downstream sensors over I2C semantics
- Two sensor child drivers exposed via MFD cells

## Build-time folder layout

```text
mfd-driver-demo/
├── Makefile
├── include/
│   └── mfd_demo.h
├── src/
│   ├── soc/
│   │   └── soc_uart_parent.c
│   ├── mcu/
│   │   └── mcu_core.c
│   └── sensor/
│       ├── sensor_a.c
│       └── sensor_b.c
└── doc/
    └── overview.md
```

Role mapping:
- `src/soc/soc_uart_parent.c`
  - SoC-facing parent
  - owns parent platform driver/device
  - registers MFD child functions for sensor A/B
  - exposes parent `demo_value` sysfs node (UART link state)
- `src/mcu/mcu_core.c`
  - MCU core/protocol abstraction
  - simulates UART command handling and routes read/write to sensor states
  - shared API consumed by sensor drivers
- `src/sensor/sensor_a.c` and `src/sensor/sensor_b.c`
  - child drivers (logical sensors behind MCU I2C)
  - expose per-sensor `demo_value` sysfs read/write nodes
- `include/mfd_demo.h`
  - shared IDs and function contracts across layers

## Run-time topology layout

At module load:
1. MCU core initializes internal sensor values.
2. Sensor A/B child drivers register.
3. SoC UART parent driver registers and parent device is created.
4. Parent probe adds MFD child devices for sensor A/B.
5. Child probes bind and create sysfs nodes.

Runtime sysfs tree (typical):

```text
/sys/devices/platform/mfd-demo-soc-uart-parent.X.auto/demo_value
/sys/devices/platform/mfd-demo-soc-uart-parent.X.auto/mfd-demo-sensor-a.Y.auto/demo_value
/sys/devices/platform/mfd-demo-soc-uart-parent.X.auto/mfd-demo-sensor-b.Z.auto/demo_value
```

Data path semantics:
- Read/write on sensor sysfs node
  -> sensor child driver handler
  -> MCU core API
  -> simulated MCU I2C access to target sensor register/value
- Read/write on parent sysfs node
  -> SoC UART bridge local link-state control

This keeps SoC/UART and MCU/I2C responsibilities separated while remaining easy to demo on one host.
