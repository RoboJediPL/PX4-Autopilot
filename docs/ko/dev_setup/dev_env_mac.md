# MacOS 개발 환경

아래에서 macOS용 PX4 개발 환경 설정 방법을 설명합니다.
PX4 빌드에 사용되어 집니다.

- Pixhawk와 기타 NuttX 기반 하드웨어
- [Gazebo Classic Simulation](../sim_gazebo_classic/index.md)

:::tip
This setup is supported by the PX4 dev team.
To build other targets you will need to use a [different OS](../dev_setup/dev_env.md#supported-targets) (or an [unsupported development environment](../advanced/community_supported_dev_env.md)).
:::

## 영상 가이드

<lite-youtube videoid="tMbMGiMs1cQ" title="Setting up your PX4 development environment on macOS"/>

## Base Setup

The "base" macOS setup installs the tools needed for building firmware, and includes the common tools that will be needed for installing/using the simulators.

### Environment Setup

:::details
Apple Silicon Macbook users!
If you have an Apple M1, M2 etc. Macbook, make sure to run the terminal as x86 by setting up an x86 terminal:

1. Locate the Terminal application within the Utilities folder (**Finder > Go menu > Utilities**)
2. Select _Terminal.app_ and right-click on it, then choose **Duplicate**.
3. Rename the duplicated Terminal app, e.g. to _x86 Terminal_
4. Now select the renamed _x86 Terminal_ app and right-click and choose \*_Get Info_
5. Check the box for **Open using Rosetta**, then close the window
6. Run the _x86 Terminal_ as usual, which will fully support the current PX4 toolchain

:::

First set up the environment

1. Enable more open files by appending the following line to the `~/.zshenv` file (creating it if necessary):

  ```sh
  echo ulimit -S -n 2048 >> ~/.zshenv
  ```

  ::: info
  If you don't do this, the build toolchain may report the error: `"LD: too many open files"`

:::

2. Enforce Python 3 by appending the following lines to `~/.zshenv`

  ```sh
  # Point pip3 to MacOS system python 3 pip
  alias pip3=/usr/bin/pip3
  ```

### 공통 도구

To setup the environment to be able to build for Pixhawk/NuttX hardware (and install the common tools for using simulators):

1. Install Homebrew by following these [installation instructions](https://brew.sh).

2. Run these commands in your shell to install the common tools:

  ```sh
  brew tap PX4/px4
  brew install px4-dev
  ```

3. Install the required Python packages:

  ```sh
  # install required packages using pip3
  python3 -m pip install --user pyserial empty toml numpy pandas jinja2 pyyaml pyros-genmsg packaging kconfiglib future jsonschema
  # if this fails with a permissions error, your Python install is in a system path - use this command instead:
  sudo -H python3 -m pip install --user pyserial empty toml numpy pandas jinja2 pyyaml pyros-genmsg packaging kconfiglib future jsonschema
  ```

## Gazebo Classic Simulation

To setup the environment for [Gazebo Classic](../sim_gazebo_classic/index.md) simulation:

1. 다음 명령을 실행하여 Intel Threading Building Blocks (TBB)를 설치합니다:

  ```sh
  brew install tbb
  ```

2. To install SITL simulation with Gazebo Classic:

  ```sh
  brew install --cask temurin
  brew install --cask xquartz
  brew install px4-sim-gazebo
  ```

3. Run the macOS setup script: `PX4-Autopilot/Tools/setup/macos.sh`
  The easiest way to do this is to clone the PX4 source, and then run the script from the directory, as shown:

  ```sh
  git clone https://github.com/PX4/PX4-Autopilot.git --recursive
  cd PX4-Autopilot/Tools/setup
  sh macos.sh
  ```

## 다음 단계

명령줄 도구 모음 설정후, 다음을 수행합니다.

- Install [VSCode](../dev_setup/vscode.md) (if you prefer using an IDE to the command line).

- Install the [QGroundControl Daily Build](../dev_setup/qgc_daily_build.md)

  :::tip
  The _daily build_ includes development tools that are hidden in release builds.
  또한, 릴리스 빌드에서 아직 지원되지 않는 새로운 PX4 기능에 대한 액세스를 제공할 수도 있습니다.

:::

- Continue to the [build instructions](../dev_setup/building_px4.md).
