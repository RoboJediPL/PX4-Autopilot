# Quadruped with Wheels

PX4 provides an experimental module for controlling quadruped robots that can walk or drive using wheels.

The `quadruped_control` module converts leg commands to wheel setpoints in *wheel mode* and republishes joint states when in *leg mode*. Use the `QD_MODE` parameter to switch between modes.

This feature is experimental and may require additional integration for specific hardware like the Unitree B2-W. Contributions are welcome.
