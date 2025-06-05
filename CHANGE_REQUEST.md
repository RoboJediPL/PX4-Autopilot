# Change Request: Unitree B2-W Quadruped Integration

This document outlines the required changes to operate the Unitree B2-W quadruped robot with a Pixhawk 4 running PX4. The goal is to support both wheel-based driving and leg-based walking modes.

## 1. Quadruped Support Module
- Add `src/modules/quadruped_control` for managing wheel or leg mode via the `QD_MODE` parameter.
- In wheel mode, convert leg joint velocities into rover throttle and steering setpoints.
- In leg mode, generate joint trajectories for a basic gait and publish leg status.

## 2. Wheel Drive Integration
- Reuse existing rover modules (differential, mecanum, etc.) to drive the wheel motors.
- Ensure the motor driver publishes `wheel_encoders` uORB messages for feedback.

## 3. Board Configuration
- Provide a new board config `px4_fmu-v5_quadruped` enabling the quadruped module and rover drivers.
- Keep the roboclaw driver enabled if used for wheel control.

## 4. Telemetry and MAVLink
- Add a `QUADRUPED_LEG_STATUS` MAVLink stream to report joint positions and velocities.
- Forward wheel encoder data over MAVLink as well.

## 5. Documentation
- Document how to build and flash the quadruped firmware and warn that rover support is experimental.
- Link the new page from the rover documentation index.

## 6. Simulation and Testing
- Add a Gazebo plugin (`WheelEncoderSystem`) that publishes wheel encoder data so the quadruped can be tested in SITL.
- Run `make px4_sitl_default` after running `Tools/setup/ubuntu.sh` to verify builds.

## License and Disclaimer
This repository is distributed under the BSD-3-Clause license. The software is provided "AS IS" without warranty of any kind. Quadruped and rover features rely on the experimental rover stack, so use this functionality at your own risk.
=======
This document outlines the proposed updates for running a Unitree B2-W quadruped with wheels using a Pixhawk 4 loaded with this PX4 firmware. The goal is to support both wheel-based drive and leg-based walking modes.

## 1. Quadruped Support Module
- Implement a new module under `src/modules/quadruped_control` that can switch between wheel mode and leg mode using the `QD_MODE` parameter.
- In wheel mode, translate leg joint velocity commands into rover throttle and steering setpoints. Publish leg status messages based on wheel encoder feedback.
- In leg mode, add gait-generation logic to command the joints directly and publish leg status accordingly.

## 2. Wheel Drive Integration
- Reuse existing rover modules (e.g. differential or mecanum drive) to actuate the wheel motors.
- Ensure a wheel encoder driver (such as Roboclaw) publishes `wheel_encoders` uORB messages for feedback.

## 3. Board Configuration
- Enable the quadruped module and necessary rover modules in the Pixhawk 4 rover board config (`boards/px4/fmu-v5/rover.px4board`).
- Keep support for the Roboclaw driver or other motor controllers required by the hardware.

## 4. Telemetry and MAVLink
- Publish leg status via a new `QUADRUPED_LEG_STATUS` MAVLink stream so joint positions and velocities can be monitored in real time.
- Continue to forward wheel encoder data over existing topics and MAVLink messages.

## 5. Documentation
- Document build instructions for the quadruped firmware, describe `QD_MODE`, and emphasize that rover functionality is experimental.
- Add a dedicated page under `docs/en` and link it from the rover index.

## 6. Testing
- Provide Gazebo simulation support for the quadruped. A new `WheelEncoderSystem` plugin under `src/modules/simulation/gz_plugins/wheel_encoder` publishes wheel encoder data so PX4 can receive feedback like on real hardware.
- Run `make px4_sitl_default` and other standard tests to verify the build.


## Development Setup
To run format and build tests locally, install dependencies using `Tools/setup/ubuntu.sh`. This script installs `astyle`, `kconfiglib`, and required Python packages so that `make check_format` and `make px4_sitl_default` succeed.

## Current Status
The quadruped control module and board configuration have been added. Format checks pass and the firmware builds successfully using `make px4_sitl_default` after running the setup script.

## License and Disclaimer
This repository is distributed under the BSD 3-Clause license. The software is provided
"AS IS" without warranty of any kind. Quadruped and rover features rely on the
experimental rover stack, so use this functionality at your own risk.

