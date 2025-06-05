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

