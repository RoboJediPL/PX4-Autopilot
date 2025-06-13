#pragma once

#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/SubscriptionInterval.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/Publication.hpp>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/quadruped_leg_command.h>
#include <uORB/topics/rover_throttle_setpoint.h>
#include <uORB/topics/rover_steering_setpoint.h>
#include <uORB/topics/rover_velocity_setpoint.h>
#include <mathlib/mathlib.h>
#include <cmath>

using namespace time_literals;

class QuadrupedGait : public ModuleBase<QuadrupedGait>, public ModuleParams, public px4::ScheduledWorkItem
{
public:
    QuadrupedGait();
    ~QuadrupedGait() override = default;

    bool init();
    void Run() override;

private:
    uORB::Publication<quadruped_leg_command_s> _cmd_pub{ORB_ID(quadruped_leg_command)};
    uORB::SubscriptionInterval _parameter_update_sub{ORB_ID(parameter_update), 1_s};
    uORB::Subscription _throttle_sub{ORB_ID(rover_throttle_setpoint)};
    uORB::Subscription _steering_sub{ORB_ID(rover_steering_setpoint)};
    uORB::Subscription _velocity_sub{ORB_ID(rover_velocity_setpoint)};

    hrt_abstime _start_time{0};
    float _throttle_body_x{0.f};
    float _steering_diff{0.f};

    DEFINE_PARAMETERS(
        (ParamInt<px4::params::QDP_PERIOD_MS>) _param_qdp_period_ms,
        (ParamFloat<px4::params::QDP_STEP_AMP>) _param_qdp_step_amp,
        (ParamFloat<px4::params::QDP_ROTATE_AMP>) _param_qdp_rotate_amp,
        (ParamFloat<px4::params::QDP_MAX_SPEED>) _param_qdp_max_speed
    )
};

int QuadrupedGait_main(int argc, char *argv[]);
