/****************************************************************************
 *
 *   Copyright (c) 2025 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include "QuadrupedControl.hpp"
// Experimental quadruped module supporting both wheel drive and leg control.
// Wheel mode converts leg velocity commands into rover throttle and steering
// setpoints, while leg mode simply republishes the commanded joint states.


QuadrupedControl::QuadrupedControl() :
	ModuleParams(nullptr),
	ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::rate_ctrl)
{
}

bool QuadrupedControl::init()
{
	ScheduleOnInterval(10_ms);
	return true;
}

void QuadrupedControl::updateParams()
{
	ModuleParams::updateParams();
}

void QuadrupedControl::Run()
{
	if (should_exit()) {
		ScheduleClear();
		exit_and_cleanup();
		return;
	}

	if (_parameter_update_sub.updated()) {
		parameter_update_s p{};
		_parameter_update_sub.copy(&p);
		updateParams();
	}

	quadruped_leg_command_s cmd{};

	if (_param_qd_mode.get() == 0) { // wheel mode
		if (_leg_command_sub.update(&cmd)) {
			const float right = 0.5f * (cmd.joint_velocity[7] + cmd.joint_velocity[15]);
			const float left  = 0.5f * (cmd.joint_velocity[3] + cmd.joint_velocity[11]);
			const float avg = (right + left) * 0.5f;

			rover_throttle_setpoint_s thr{};
			thr.timestamp = hrt_absolute_time();
			thr.throttle_body_x = avg * _param_qd_thr_gain.get();
			thr.throttle_body_y = 0.f;
			_rover_throttle_pub.publish(thr);

			rover_steering_setpoint_s steer{};
			steer.timestamp = thr.timestamp;
			steer.normalized_speed_diff = (right - left) * _param_qd_str_gain.get();
			_rover_steering_pub.publish(steer);
		}

		wheel_encoders_s wheel{};

		if (_wheel_encoder_sub.update(&wheel)) {
			quadruped_leg_status_s status{};
			status.timestamp = wheel.timestamp;
			memset(status.joint_position, 0, sizeof(status.joint_position));
			memset(status.joint_velocity, 0, sizeof(status.joint_velocity));
			// map wheel encoder data to WM joints
			status.joint_position[3]  = wheel.wheel_angle[0]; // FL_WM
			status.joint_position[7]  = wheel.wheel_angle[1]; // FR_WM
			status.joint_position[11] = wheel.wheel_angle[2]; // RL_WM
			status.joint_position[15] = wheel.wheel_angle[3]; // RR_WM
			status.joint_velocity[3]  = wheel.wheel_speed[0];
			status.joint_velocity[7]  = wheel.wheel_speed[1];
			status.joint_velocity[11] = wheel.wheel_speed[2];
			status.joint_velocity[15] = wheel.wheel_speed[3];
			_leg_status_pub.publish(status);
		}

	} else {
		if (_leg_command_sub.update(&cmd)) {
			quadruped_leg_status_s status{};
			status.timestamp = hrt_absolute_time();
			memcpy(status.joint_position, cmd.joint_position, sizeof(status.joint_position));
			memcpy(status.joint_velocity, cmd.joint_velocity, sizeof(status.joint_velocity));
			_leg_status_pub.publish(status);
		}
	}
}

int QuadrupedControl::task_spawn(int argc, char *argv[])
{
	QuadrupedControl *instance = new QuadrupedControl();

	if (instance && instance->init()) {
		_object.store(instance);
		_task_id = task_id_is_work_queue; // task id for scheduled work item
		return 0;
	}

	delete instance;
	return -1;
}

int QuadrupedControl::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

int QuadrupedControl::print_usage(const char *reason)
{
	if (reason) {
		PX4_INFO("%s", reason);
	}

	PRINT_MODULE_DESCRIPTION(
		R"DESCR(
### Description
Simple quadruped control module example. It republishes leg commands as status.
)DESCR");

        PRINT_MODULE_USAGE_NAME("quadruped_control", "controller");
        return 0;
}

extern "C" __EXPORT int quadruped_control_main(int argc, char *argv[])
{
        return QuadrupedControl::main(argc, argv);
}
