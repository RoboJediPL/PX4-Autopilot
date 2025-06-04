# Change Request: Unitree B2-W Quadruped Integration

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
- Use existing MAVLink streams to send leg status and wheel encoder data. Add new MAVLink messages if more detailed telemetry is required.

## 5. Documentation
- Document build instructions for the quadruped firmware, describe `QD_MODE`, and emphasize that rover functionality is experimental.
- Add a dedicated page under `docs/en` and link it from the rover index.

## 6. Testing
- Provide Gazebo simulation support for the quadruped, including wheel encoder simulation.
- Run `make px4_sitl_default` and other standard tests to verify the build.


## Development Setup
To run format and build tests locally, install dependencies using `Tools/setup/ubuntu.sh`. This script installs `astyle`, `kconfiglib`, and required Python packages so that `make check_format` and `make px4_sitl_default` succeed.

## Current Status
The quadruped control module and board configuration have been added. Format checks pass and the firmware builds successfully using `make px4_sitl_default` after running the setup script.

