# Quadruped with Wheels

PX4 provides an experimental module for controlling quadruped robots that can walk or drive using wheels.

The `quadruped_control` module converts leg commands to wheel setpoints in *wheel mode* and republishes joint states when in *leg mode*. Use the `QD_MODE` parameter to switch between modes.

In *wheel mode* the commanded joint velocities are translated into `rover_throttle_setpoint` and `rover_steering_setpoint` messages. These setpoints can then be processed by the existing rover controllers to drive the wheel motors.

This feature is experimental and may require additional integration for specific hardware like the Unitree B2-W. Contributions are welcome.

## Building for Pixhawk 4 Rover

To build the quadruped firmware for Pixhawk 4, run:

```sh
make px4_fmu-v5_quadruped
```

Flash the generated firmware onto your controller using [custom firmware installation instructions](../config/firmware.md#installing-px4-main-beta-or-custom-firmware).

After boot, set `QD_MODE` to `0` for wheel mode or `1` for leg mode.
