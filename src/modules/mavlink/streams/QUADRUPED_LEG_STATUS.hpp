#ifndef QUADRUPED_LEG_STATUS_HPP
#define QUADRUPED_LEG_STATUS_HPP

#include <uORB/topics/quadruped_leg_status.h>
#include <uORB/topics/debug_array.h>
#include <math.h>
#include <cstring>

class MavlinkStreamQuadrupedLegStatus : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink) { return new MavlinkStreamQuadrupedLegStatus(mavlink); }

	static constexpr const char *get_name_static() { return "QUADRUPED_LEG_STATUS"; }
	static constexpr uint16_t get_id_static() { return MAVLINK_MSG_ID_DEBUG_FLOAT_ARRAY; }

	const char *get_name() const override { return get_name_static(); }
	uint16_t get_id() override { return get_id_static(); }

	unsigned get_size() override
	{
		return _leg_status_sub.advertised() ? MAVLINK_MSG_ID_DEBUG_FLOAT_ARRAY_LEN * 2 + MAVLINK_NUM_NON_PAYLOAD_BYTES * 2 : 0;
	}

private:
	explicit MavlinkStreamQuadrupedLegStatus(Mavlink *mavlink) : MavlinkStream(mavlink) {}

	static constexpr size_t NUM_JOINTS = 12;
	uORB::Subscription _leg_status_sub{ORB_ID(quadruped_leg_status)};
	debug_array_s _msg{}; // reused buffer

	bool send() override
	{
		quadruped_leg_status_s status{};

		if (_leg_status_sub.update(&status)) {
			// Publish joint positions
			_msg.timestamp = status.timestamp;
			_msg.id = 0;
			strncpy(_msg.name, "QD_J_POS", sizeof(_msg.name));

			for (size_t i = 0; i < NUM_JOINTS; ++i) {
				_msg.data[i] = status.joint_position[i];
			}

			for (size_t i = NUM_JOINTS; i < debug_array_s::ARRAY_SIZE; ++i) {
				_msg.data[i] = NAN;
			}

			mavlink_msg_debug_float_array_send_struct(_mavlink->get_channel(),
					reinterpret_cast<mavlink_debug_float_array_t *>(&_msg));

			// Publish joint velocities
			_msg.id = 1;
			strncpy(_msg.name, "QD_J_VEL", sizeof(_msg.name));

			for (size_t i = 0; i < NUM_JOINTS; ++i) {
				_msg.data[i] = status.joint_velocity[i];
			}

			for (size_t i = NUM_JOINTS; i < debug_array_s::ARRAY_SIZE; ++i) {
				_msg.data[i] = NAN;
			}

			mavlink_msg_debug_float_array_send_struct(_mavlink->get_channel(),
					reinterpret_cast<mavlink_debug_float_array_t *>(&_msg));

			return true;
		}

		return false;
	}
};

#endif // QUADRUPED_LEG_STATUS_HPP
