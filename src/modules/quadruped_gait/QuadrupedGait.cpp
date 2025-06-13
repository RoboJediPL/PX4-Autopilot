#include "QuadrupedGait.hpp"

QuadrupedGait::QuadrupedGait()
    : ModuleParams(nullptr), ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::nav_and_controllers)
{
    _start_time = hrt_absolute_time();
}

bool QuadrupedGait::init()
{
    ScheduleOnInterval(100_ms); // 10 Hz
    return true;
}

void QuadrupedGait::Run()
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

    const float period_s = math::max(0.001f, static_cast<float>(_param_qdp_period_ms.get()) / 1000.f);

    rover_throttle_setpoint_s thr{};
    if (_throttle_sub.update(&thr)) {
        _throttle_body_x = thr.throttle_body_x;
    }

    rover_velocity_setpoint_s vel{};
    if (_velocity_sub.update(&vel)) {
        _throttle_body_x = math::constrain(vel.speed / _param_qdp_max_speed.get(), -1.f, 1.f);
    }

    rover_steering_setpoint_s steer{};
    if (_steering_sub.update(&steer)) {
        _steering_diff = steer.normalized_speed_diff;
    }

    const float speed_amp = math::constrain(_param_qdp_step_amp.get() * _throttle_body_x, -1.f, 1.f);
    const float rot_amp = math::constrain(_param_qdp_rotate_amp.get() * _steering_diff, -1.f, 1.f);

    const float phase = fmodf((hrt_absolute_time() - _start_time) / 1e6f, period_s) / period_s;

    quadruped_leg_command_s cmd{};
    cmd.timestamp = hrt_absolute_time();

    for (int i = 0; i < 4; ++i) {
        float leg_phase = phase + ((i == 1 || i == 2) ? 0.5f : 0.f);

        if (leg_phase >= 1.f) { leg_phase -= 1.f; }

        cmd.wheel_setpoints[i] = (leg_phase < 0.5f) ? speed_amp : -speed_amp;
        cmd.rotate_setpoints[i] = rot_amp * sinf(leg_phase * M_PI_F * 2.f);
    }

    _cmd_pub.publish(cmd);
}

int QuadrupedGait_main(int argc, char *argv[])
{
    return QuadrupedGait::main(argc, argv);
}
