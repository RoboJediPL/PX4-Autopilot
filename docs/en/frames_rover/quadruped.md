# Quadruped with Wheels
<LinkedBadge type="warning" text="Experimental" url="index.md#experimental-vehicles"/>

:::warning
Quadruped support uses the PX4 rover stack, which is [experimental](../airframes/index.md#experimental-vehicles).
Use this firmware at your own risk and consider contributing improvements.
:::


PX4 provides an experimental module for controlling quadruped robots that can walk or drive using wheels.

The `quadruped_control` module converts leg commands to wheel setpoints in *wheel mode* and republishes joint states when in *leg mode*. Use the `QD_MODE` parameter to switch between modes.

Each leg has four motors:

* **TM** - Turn motor adjusts the wheel heading (\-45° to +45°).
* **RM** - Rotates the entire leg forward/backward (\-45° to +45°).
* **LM** - Lever motor actuates the lower leg.
* **WM** - Wheel motor drives the wheel.

The 16‑element joint arrays used by the module follow this order:

```
[FL_TM, FL_RM, FL_LM, FL_WM,
 FR_TM, FR_RM, FR_LM, FR_WM,
 RL_TM, RL_RM, RL_LM, RL_WM,
 RR_TM, RR_RM, RR_LM, RR_WM]
```

In *wheel mode* the commanded joint velocities are translated into `rover_throttle_setpoint` and `rover_steering_setpoint` messages. These setpoints can then be processed by the existing rover controllers to drive the wheel motors.

This feature is experimental and may require additional integration for specific hardware like the Unitree B2-W. Contributions are welcome.

## Telemetry

Wheel angles and velocities from all four wheels must be published on the
`wheel_encoders` topic so the module can translate them into joint status in
wheel mode. The resulting
joint positions and velocities are sent over MAVLink via the
`QUADRUPED_LEG_STATUS` stream for monitoring.

## Building and Flashing for Pixhawk 4

Build the quadruped firmware using the dedicated board target:

```sh
make px4_fmu-v5_quadruped
```

Flash the resulting firmware using the [custom firmware installation instructions](../config/firmware.md#installing-px4-main-beta-or-custom-firmware).

After boot, the `quadruped_control` module starts automatically.

### Parameters

* `QD_MODE` - 0: wheel mode, 1: leg mode.
* `QD_THR_GAIN` - wheel throttle gain in wheel mode.
* `QD_STR_GAIN` - wheel steering gain in wheel mode.
* `QD_GAIT_FREQ` - gait frequency in Hz when using the internal gait generator.
* `QD_GAIT_AMP`  - leg rotation amplitude in radians for the gait generator.

In leg mode the module runs a simple trot gait based on these parameters and publishes joint states.
