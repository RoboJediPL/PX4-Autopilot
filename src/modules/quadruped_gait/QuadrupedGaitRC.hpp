#pragma once

#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>

#include <uORB/SubscriptionCallback.hpp>
#include <uORB/SubscriptionInterval.hpp>
#include <uORB/Publication.hpp>
#include <uORB/topics/manual_control_setpoint.h>
#include <uORB/topics/quadruped_gait_command.h>
#include <uORB/topics/parameter_update.h>

using namespace time_literals;

class QuadrupedGaitRC : public ModuleBase<QuadrupedGaitRC>, public ModuleParams, public px4::ScheduledWorkItem
{
public:
	QuadrupedGaitRC();
	~QuadrupedGaitRC() override = default;

	static int task_spawn(int argc, char *argv[]);
	static int custom_command(int argc, char *argv[]);
	static int print_usage(const char *reason = nullptr);

	bool init();

private:
	void Run() override;

	uORB::SubscriptionCallbackWorkItem _manual_control_sub{this, ORB_ID(manual_control_setpoint)};
	uORB::SubscriptionInterval _parameter_update_sub{ORB_ID(parameter_update), 1_s};

	uORB::Publication<quadruped_gait_command_s> _gait_cmd_pub{ORB_ID(quadruped_gait_command)};
};

