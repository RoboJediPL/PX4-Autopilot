#include "QuadrupedGaitRC.hpp"

#include <lib/mathlib/mathlib.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/log.h>

QuadrupedGaitRC::QuadrupedGaitRC() :
	ModuleParams(nullptr),
       ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::rate_ctrl)
{}

bool QuadrupedGaitRC::init()
{
	if (!_manual_control_sub.registerCallback()) {
		PX4_ERR("callback registration failed");
		return false;
	}

	return true;
}

void QuadrupedGaitRC::Run()
{
	if (should_exit()) {
		ScheduleClear();
		_manual_control_sub.unregisterCallback();
		exit_and_cleanup();
		return;
	}

	if (_parameter_update_sub.updated()) {
		parameter_update_s p{};
		_parameter_update_sub.copy(&p);
		updateParams();
	}

	manual_control_setpoint_s mc{};

	if (_manual_control_sub.update(&mc)) {
		quadruped_gait_command_s cmd{};
		cmd.timestamp = hrt_absolute_time();

		if (PX4_ISFINITE(mc.aux2)) {
			cmd.frequency = math::constrain(1.f + mc.aux2, 0.f, 5.f);

		} else {
			cmd.frequency = NAN;
		}

		if (PX4_ISFINITE(mc.aux1)) {
			cmd.amplitude = math::constrain(mc.aux1, 0.f, 1.f);

		} else {
			cmd.amplitude = NAN;
		}

		_gait_cmd_pub.publish(cmd);
	}
}

int QuadrupedGaitRC::task_spawn(int argc, char *argv[])
{
	QuadrupedGaitRC *instance = new QuadrupedGaitRC();

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

int QuadrupedGaitRC::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

int QuadrupedGaitRC::print_usage(const char *reason)
{
	if (reason) {
		PX4_WARN("%s\n", reason);
	}

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
Generate quadruped_gait_command from RC inputs. AUX1 controls amplitude and AUX2 controls frequency.
)DESCR_STR");

    PRINT_MODULE_USAGE_NAME("quadruped_gait_rc", "controller");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

    return 0;
}

extern "C" __EXPORT int quadruped_gait_rc_main(int argc, char *argv[])
{
    return QuadrupedGaitRC::main(argc, argv);
}

