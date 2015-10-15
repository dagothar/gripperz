/* 
 * File:   WorkcellGripper.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 10:58 PM
 */

#pragma once

#include "Gripper.hpp"
#include <rw/models/WorkCell.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>

namespace gripperz {
    namespace models {

        /**
         * @class WorkcellGripper
         * @brief A gripper pre-existing in the workcell
         */
        class WorkcellGripper : public Gripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<WorkcellGripper> Ptr;

        public:
            WorkcellGripper(const std::string& name);

            virtual ~WorkcellGripper();

            virtual void registerWithContext(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state);

            virtual bool isRegistered();

            rw::kinematics::MovableFrame* getMovableFrame() const {
                return _movableFrame;
            }

            rw::kinematics::Frame* getTCPFrame() const {
                return _TCPFrame;
            }

            rwsim::dynamics::DynamicDevice::Ptr getDynamicDevice() const {
                return _dynamicDevice;
            }

            rw::models::Device::Ptr getDevice() const {
                return _device;
            }

            void setGraspControllerId(std::string _graspControllerId) {
                this->_graspControllerId = _graspControllerId;
            }

            std::string getGraspControllerId() const {
                return _graspControllerId;
            }

            void setMovableFrameId(std::string _movableFrameId) {
                this->_movableFrameId = _movableFrameId;
            }

            std::string getMovableFrameId() const {
                return _movableFrameId;
            }

            void setTCPFrameId(std::string _TCPFrameId) {
                this->_TCPFrameId = _TCPFrameId;
            }

            std::string getTCPFrameId() const {
                return _TCPFrameId;
            }

            void setDynamicDeviceId(std::string _dynamicDeviceId) {
                this->_dynamicDeviceId = _dynamicDeviceId;
            }

            std::string getDynamicDeviceId() const {
                return _dynamicDeviceId;
            }

            void setDeviceId(std::string _deviceId) {
                this->_deviceId = _deviceId;
            }

            std::string getDeviceId() const {
                return _deviceId;
            }

        private:
            std::string _deviceId;
            std::string _dynamicDeviceId;
            std::string _TCPFrameId;
            std::string _movableFrameId;
            std::string _graspControllerId;

            rw::models::Device::Ptr _device;
            rwsim::dynamics::DynamicDevice::Ptr _dynamicDevice;
            rw::kinematics::Frame* _TCPFrame;
            rw::kinematics::MovableFrame* _movableFrame;
        };

    }
}


