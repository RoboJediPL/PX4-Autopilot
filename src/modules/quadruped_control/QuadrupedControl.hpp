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

#pragma once

// Header for experimental quadruped control module supporting both wheels and legs.
// Each leg has four motors: Turn (TM), Rotate (RM), Lever (LM) and Wheel (WM).
// The joint arrays are ordered as:
// [FL_TM, FL_RM, FL_LM, FL_WM, FR_TM, FR_RM, FR_LM, FR_WM,
//  RL_TM, RL_RM, RL_LM, RL_WM, RR_TM, RR_RM, RR_LM, RR_WM].
// Wheel mode converts WM velocities into rover throttle/steering setpoints
// while leg mode forwards commanded joint states directly.

#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>

#include <uORB/Subscription.hpp>
#include <uORB/Publication.hpp>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/vehicle_control_mode.h>
#include <uORB/topics/quadruped_leg_command.h>
#include <uORB/topics/quadruped_leg_status.h>
#include <uORB/topics/wheel_encoders.h>
#include <uORB/topics/rover_throttle_setpoint.h>
#include <uORB/topics/rover_steering_setpoint.h>

using namespace time_literals;

class QuadrupedControl : public ModuleBase<QuadrupedControl>, public ModuleParams,
	public px4::ScheduledWorkItem
{
public:
	QuadrupedControl();
	~QuadrupedControl() override = default;

	static int task_spawn(int argc, char *argv[]);
	static int custom_command(int argc, char *argv[]);
	static int print_usage(const char *reason = nullptr);

	bool init();

private:
	void Run() override;
	void updateParams() override;

	uORB::Subscription _parameter_update_sub{ORB_ID(parameter_update)};
	uORB::Subscription _vehicle_control_mode_sub{ORB_ID(vehicle_control_mode)};
	uORB::Subscription _leg_command_sub{ORB_ID(quadruped_leg_command)};
	uORB::Subscription _wheel_encoder_sub{ORB_ID(wheel_encoders)};

	uORB::Publication<quadruped_leg_status_s> _leg_status_pub{ORB_ID(quadruped_leg_status)};
	uORB::Publication<rover_throttle_setpoint_s> _rover_throttle_pub{ORB_ID(rover_throttle_setpoint)};
	uORB::Publication<rover_steering_setpoint_s> _rover_steering_pub{ORB_ID(rover_steering_setpoint)};

	DEFINE_PARAMETERS(
		(ParamInt<px4::params::QD_MODE>)      _param_qd_mode,
		(ParamFloat<px4::params::QD_THR_GAIN>) _param_qd_thr_gain,
		(ParamFloat<px4::params::QD_STR_GAIN>) _param_qd_str_gain,
		(ParamFloat<px4::params::QD_GAIT_FREQ>) _param_qd_gait_freq,
		(ParamFloat<px4::params::QD_GAIT_AMP>)  _param_qd_gait_amp
	)

	static constexpr uint8_t rm_index[4] {1, 5, 9, 13};
	static constexpr uint8_t lm_index[4] {2, 6, 10, 14};
};
