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

#include "WheelEncoderSystem.hpp"

#include <gz/msgs/actuators.pb.h>
#include <gz/sim/Joint.hh>
#include <gz/sim/Model.hh>
#include <gz/sim/Util.hh>

using namespace custom;

void WheelEncoderSystem::Configure(const gz::sim::Entity &entity,
				   const std::shared_ptr<const sdf::Element> &sdf,
				   gz::sim::EntityComponentManager &ecm,
				   gz::sim::EventManager &)
{
	gz::sim::Model model(entity);
	_model_name = model.Name(ecm);

	if (sdf->HasElement("joint")) {
		sdf::ElementPtr joint_elem = sdf->GetElement("joint");

		while (joint_elem) {
			std::string joint_name = joint_elem->Get<std::string>();
			auto joint_entity = model.JointByName(ecm, joint_name);

			if (joint_entity != gz::sim::kNullEntity) {
				_joints.emplace_back(joint_entity);

			} else {
				gzerr << "WheelEncoderSystem: joint " << joint_name << " not found" << std::endl;
			}

			joint_elem = joint_elem->GetNextElement("joint");
		}
	}

	std::string topic = "/model/" + _model_name + "/command/motor_speed";
	_wheel_pub = _node.Advertise<gz::msgs::Actuators>(topic);
}

void WheelEncoderSystem::PreUpdate(const gz::sim::UpdateInfo &info,
				   gz::sim::EntityComponentManager &ecm)
{
	if (_joints.empty() || !_wheel_pub.Valid() || info.paused) {
		return;
	}

	gz::msgs::Actuators msg;
	msg.mutable_velocity()->Resize(_joints.size(), 0);

	for (size_t i = 0; i < _joints.size(); ++i) {
		auto velComp = ecm.Component<gz::sim::components::JointVelocity>(_joints[i].Entity());

		if (velComp && !velComp->Data().empty()) {
			msg.set_velocity(i, velComp->Data()[0]);
		}
	}

	_wheel_pub.Publish(msg);
}

// Register plugin with gazebo
GZ_ADD_PLUGIN(WheelEncoderSystem, gz::sim::System,
	      WheelEncoderSystem::ISystemConfigure,
	      WheelEncoderSystem::ISystemPreUpdate)

GZ_ADD_PLUGIN_ALIAS(WheelEncoderSystem, "custom::WheelEncoderSystem")

