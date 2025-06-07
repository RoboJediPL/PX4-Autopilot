#!/usr/bin/env bash
# Simple helper script to launch the quadruped Gazebo simulation
# Ensures the models submodule is initialized and the Gazebo
# distribution is configured before calling make.
set -e

# Initialize submodule if required
git submodule update --init Tools/simulation/gz
git submodule update --init --recursive

export GZ_DISTRO=${GZ_DISTRO:-harmonic}

PX4_GZ_WORLD=quadruped make px4_sitl gz_quadruped "$@"
