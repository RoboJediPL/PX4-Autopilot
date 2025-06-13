# Quadruped Rovers

<Badge type="tip" text="PX4 v1.16" /> <Badge type="warning" text="Experimental"/>

A quadruped rover is a legged robot that walks using four individually actuated legs. The PX4 quadruped module converts throttle and steering setpoints into leg wheel and rotation commands published on the `quadruped_leg_command` topic.

Simulation of the quadruped rover is supported with Gazebo using the `gz_quadruped` target. See [Simulation > Gazebo](../sim_gazebo_gz/vehicles.md#quadruped-rover) for details.

See [Configuration/Tuning](../config_rover/quadruped.md) to set up your rover and [Drive Modes](../flight_modes_rover/quadruped.md) for the supported drive modes.
