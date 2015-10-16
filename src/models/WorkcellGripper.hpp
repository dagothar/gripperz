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
         * @brief A basic gripper pre-existing in the workcell with RobWork implementation 
         */
        class WorkcellGripper : public Gripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<WorkcellGripper> Ptr;

        public:
            WorkcellGripper(const std::string& name);

            virtual ~WorkcellGripper();

            /**
             * @brief Registers the gripper with the RW context
             * Based on the data IDs (frame name etc.) the references to RW data structures are initialized. This has to be done
             * before the gripper is used!
             * @note All of the deriving classes are supposed to call the super class initialize()
             * when overriding the method! Only the base implementation sets the isInitialized() flag.
             * @param wc
             * @param dwc
             * @param state
             */
            virtual void initialize(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state);

            bool isInitialized() const {
                return _initialized;
            }

            /**
             * @brief Applies RW context modification
             * If necessary (e.g. a parametrized gripper), a gripper can modify the RW context. The base implementation of this method does nothing.
             * @note Call super::applyModifications() when overriding this method!
             * @param wc
             * @param dwc
             * @param state
             */
            virtual void applyModifications(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state) {
            }

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
            bool _initialized;
            bool _applied;

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


