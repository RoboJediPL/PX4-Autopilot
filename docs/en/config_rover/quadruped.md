# Quadruped Rover Configuration

This guide provides basic information for configuring the quadruped rover model.

Set the `SYS_AUTOSTART` parameter to the quadruped Gazebo vehicle (`4022_gz_quadruped`) or select the airframe in QGroundControl.

Tune the quadruped gait parameters (`QDP_*`) to achieve stable walking. Each leg uses TM/SM motors for steering and throttle, and RM/PM motors for gait control.

The default configuration starts the `quadruped` control module and sets rover parameters for legged locomotion. You can further tune steering behaviour and walking speed using the `QDP_*` parameters.

## See Also

- [Quadruped Rovers](../frames_rover/quadruped.md)
- [Drive Modes](../flight_modes_rover/quadruped.md)
