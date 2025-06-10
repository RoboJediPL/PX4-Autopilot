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
#include "QuadrupedGait.hpp"

using namespace time_literals;

QuadrupedGait::QuadrupedGait() :
	ModuleParams(nullptr),
	ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::att_ctrl)
{
}

bool QuadrupedGait::init()
{
	ScheduleOnInterval(20_ms); // 50 Hz
	updateParams();
	_freq = _param_qg_freq.get();
	return true;
}

void QuadrupedGait::Run()
{
	if (should_exit()) {
		ScheduleClear();
		exit_and_cleanup();
		return;
	}

	actuator_motors_s motors{};
	motors.timestamp = hrt_absolute_time();
	motors.timestamp_sample = motors.timestamp;
	motors.reversible_flags = 0;

	parameter_update_s param_upd{};

	if (_parameter_update_sub.update(&param_upd)) {
		updateParams();
		_freq = _param_qg_freq.get();
	}

	quadruped_gait_command_s cmd{};

	if (_gait_cmd_sub.update(&cmd)) {
		if (PX4_ISFINITE(cmd.frequency)) {
			_freq = cmd.frequency;
		}

		if (PX4_ISFINITE(cmd.amplitude)) {
			_amplitude = cmd.amplitude;
		}
	}

	const float dt = 0.02f; // 20 ms
	_phase += dt * _freq * 2.f * M_PI;

	if (_phase > 2.f * M_PI) {
		_phase -= 2.f * M_PI;
	}

	const float a = _amplitude;

	motors.control[0] = a * sinf(_phase);
	motors.control[1] = a * cosf(_phase);
	motors.control[2] = a * sinf(_phase + M_PI);
	motors.control[3] = a * cosf(_phase + M_PI);
	motors.control[4] = a * sinf(_phase + M_PI_2);
	motors.control[5] = a * cosf(_phase + M_PI_2);
	motors.control[6] = a * sinf(_phase + 3.f * M_PI_2);
	motors.control[7] = a * cosf(_phase + 3.f * M_PI_2);

	for (int i = 8; i < actuator_motors_s::NUM_CONTROLS; i++) {
		motors.control[i] = NAN;
	}

	_actuator_motors_pub.publish(motors);
}

int QuadrupedGait::task_spawn(int argc, char *argv[])
{
	QuadrupedGait *instance = new QuadrupedGait();

	if (instance) {
		_object.store(instance);
		_task_id = task_id_is_work_queue;

		if (instance->init()) {
			return PX4_OK;
		}

	} else {
		PX4_ERR("alloc failed");
	}

	delete instance;
	_object.store(nullptr);
	_task_id = -1;

	return PX4_ERROR;
}

int QuadrupedGait::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

int QuadrupedGait::print_usage(const char *reason)
{
	if (reason) {
		PX4_WARN("%s\n", reason);
	}

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
Quadruped gait generation example.
Controls Rotate and Pulley motors of a four legged robot.
)DESCR_STR");

    PRINT_MODULE_USAGE_NAME("quadruped_gait", "controller");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

    return 0;
}

extern "C" __EXPORT int quadruped_gait_main(int argc, char *argv[])
{
    return QuadrupedGait::main(argc, argv);
}
