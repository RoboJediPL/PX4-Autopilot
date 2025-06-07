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

## Simulation

The repository provides a basic Gazebo simulation model and world for testing
the quadruped control module. Make sure the `Tools/simulation/gz` submodule is
initialized and that Gazebo Harmonic is installed (the `Tools/setup/ubuntu.sh`
script will install the required `gz-harmonic` package):

On macOS the package can be installed using Homebrew or via the setup script:

```bash
brew install gz-harmonic
./Tools/setup/macos.sh --sim-tools
```

```bash
git submodule update --init Tools/simulation/gz
# rerun after `make distclean`
git submodule update --init --recursive
```

Export the Gazebo distribution before building:

```bash
export GZ_DISTRO=harmonic
```

Run the simulation with:

```bash
PX4_GZ_WORLD=quadruped make px4_sitl gz_quadruped
```

If CMake reports `unknown target 'gz_quadruped'` or that Gazebo dependencies are
missing, update the submodule and (re)install the `gz-harmonic` packages:

```bash
git submodule update --init --recursive
brew install gz-harmonic  # macOS
# Ubuntu
sudo apt-get update && sudo apt-get install gz-harmonic libunwind-dev
# or run Tools/setup/ubuntu.sh on Linux
```

If you instead see an error that `px4_gz_plugins` is missing, the Gazebo models
submodule was not initialized correctly. Run the same commands above to clone it
again.

This launches Gazebo with the `quadruped` world and model, including the
`WheelEncoderSystem` plugin that publishes wheel encoder data for the rover
controllers.

To watch the quadruped walk using the internal gait generator, run the gait
demo target instead:

```bash
PX4_GZ_WORLD=quadruped make px4_sitl gz_quadruped_gait
```

The gait demo loads an airframe configuration that sets `QD_MODE` to `1` and
uses the default gait frequency and amplitude.
