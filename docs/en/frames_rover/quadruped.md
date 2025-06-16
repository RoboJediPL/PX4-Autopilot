# Quadruped Rovers

<Badge type="tip" text="PX4 v1.16" /> <Badge type="warning" text="Experimental"/>

A quadruped rover is a legged robot that walks using four individually actuated legs. Each leg has four motors:
Turn Motor (TM) and Spin Motor (SM) provide rover-style driving, while Rotate Motor (RM) and Pulley Motor (PM) generate the walking gait. The PX4 quadruped module publishes all motor setpoints on the `quadruped_leg_command` topic.

Simulation of the quadruped rover is supported with Gazebo using the `gz_quadruped` target. See [Simulation > Gazebo](../sim_gazebo_gz/vehicles.md#quadruped-rover) for details.

::: tip
If the build or simulation fails with an error that the `quadruped` model cannot be found, the submodule containing the Gazebo models may not have been initialized. Fetch the models using:

```sh
git submodule update --init --recursive Tools/simulation/gz
```
:::

::: tip
If you see `ERROR: Gazebo simulation dependencies not found!` when running `make px4_sitl gz_quadruped`, install Gazebo using:

```sh
bash Tools/setup/ubuntu.sh
```
:::

See [Configuration/Tuning](../config_rover/quadruped.md) to set up your rover and [Drive Modes](../flight_modes_rover/quadruped.md) for the supported drive modes.
