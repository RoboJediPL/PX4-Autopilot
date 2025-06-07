# Quadruped Simulation

This directory contains resources for simulating a simple quadruped robot using Gazebo Harmonic.

```
PX4_GZ_WORLD=quadruped make px4_sitl gz_quadruped
```

To run the built-in gait generator start the gait target which enables leg mode
and publishes a simple trot pattern:

```
PX4_GZ_WORLD=quadruped make px4_sitl gz_quadruped_gait
```

Ensure that the Gazebo models submodule is initialized and that the `gz-harmonic` package is installed.
On macOS install the package with Homebrew:

```bash
brew install gz-harmonic
```

On Ubuntu install the dependencies with:

```bash
sudo apt-get update && sudo apt-get install gz-harmonic libunwind-dev libgz-msgs10-dev libgz-transport13-dev libgz-math7-dev libgz-utils2-dev
=======
```
If the submodule becomes detached after running `make distclean`, reinitialize it recursively:

```
git submodule update --init Tools/simulation/gz
git submodule update --init --recursive
export GZ_DISTRO=harmonic
```

If the build fails with `px4_gz_plugins` missing, run the above commands to
ensure the models and plugins are downloaded.

The `model` and `world` files included here provide a minimal environment with a ground plane, sun light source, and the quadruped model using the `WheelEncoderSystem` plugin.

See [docs/en/frames_rover/quadruped.md](../../../docs/en/frames_rover/quadruped.md) for full details about the quadruped control module and hardware integration.
