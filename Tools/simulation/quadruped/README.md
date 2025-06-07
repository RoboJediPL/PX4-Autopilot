# Quadruped Simulation

This directory contains resources for simulating a simple quadruped robot using Gazebo Harmonic.

```
PX4_GZ_WORLD=quadruped make px4_sitl gz_quadruped
```

Ensure that the Gazebo models submodule is initialized and that the `gz-harmonic` package is installed.
If the submodule becomes detached after running `make distclean`, reinitialize it recursively:

```
git submodule update --init Tools/simulation/gz
git submodule update --init --recursive
export GZ_DISTRO=harmonic
```

The `model` and `world` files included here provide a minimal environment with a ground plane, sun light source, and the quadruped model using the `WheelEncoderSystem` plugin.

See [docs/en/frames_rover/quadruped.md](../../../docs/en/frames_rover/quadruped.md) for full details about the quadruped control module and hardware integration.
