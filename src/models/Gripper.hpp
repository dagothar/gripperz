/* 
 * File:   Gripper.hpp
 * Author: dagothar
 *
 * Created on October 12, 2015, 2:20 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <rw/models/WorkCell.hpp>
#include <rw/kinematics/State.hpp>
#include <rw/kinematics/MovableFrame.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>
#include "GripperQuality.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class Gripper
         * @brief
         */
        class Gripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Gripper> Ptr;

        public:
            Gripper(const std::string& name);

            virtual ~Gripper();

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
            virtual void registerWithContext(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state);

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

            void setName(const std::string& name) {
                this->_name = name;
            }

            std::string getName() const {
                return _name;
            }

            void setQuality(GripperQuality::Ptr quality) {
                this->_quality = quality;
            }

            GripperQuality::Ptr getQuality() const {
                return _quality;
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

            void setGraspControllerId(const std::string& _graspControllerId) {
                this->_graspControllerId = _graspControllerId;
            }

            std::string getGraspControllerId() const {
                return _graspControllerId;
            }

            void setMovableFrameId(const std::string& _movableFrameId) {
                this->_movableFrameId = _movableFrameId;
            }

            std::string getMovableFrameId() const {
                return _movableFrameId;
            }

            void setTCPFrameId(const std::string& _TCPFrameId) {
                this->_TCPFrameId = _TCPFrameId;
            }

            std::string getTCPFrameId() const {
                return _TCPFrameId;
            }

            void setDynamicDeviceId(const std::string& _dynamicDeviceId) {
                this->_dynamicDeviceId = _dynamicDeviceId;
            }

            std::string getDynamicDeviceId() const {
                return _dynamicDeviceId;
            }

            void setDeviceId(const std::string& _deviceId) {
                this->_deviceId = _deviceId;
            }

            std::string getDeviceId() const {
                return _deviceId;
            }

            void setRightFingerId(const std::string& _rightFingerId) {
                this->_rightFingerId = _rightFingerId;
            }

            std::string getRightFingerId() const {
                return _rightFingerId;
            }

            void setLeftFingerId(const std::string& _leftFingerId) {
                this->_leftFingerId = _leftFingerId;
            }

            std::string getLeftFingerId() const {
                return _leftFingerId;
            }

            rwsim::dynamics::Body::Ptr getRightFingerBody() const {
                return _rightFingerBody;
            }

            rwsim::dynamics::Body::Ptr getLeftFingerBody() const {
                return _leftFingerBody;
            }

            rw::models::Object::Ptr getRightFingerObject() const {
                return _rightFingerObject;
            }

            rw::models::Object::Ptr getLeftFingerObject() const {
                return _leftFingerObject;
            }

        private:
            std::string _name;
            std::string _deviceId;
            std::string _dynamicDeviceId;
            std::string _TCPFrameId;
            std::string _movableFrameId;
            std::string _graspControllerId;
            std::string _leftFingerId;
            std::string _rightFingerId;
            GripperQuality::Ptr _quality;

            rw::models::Device::Ptr _device;
            rwsim::dynamics::DynamicDevice::Ptr _dynamicDevice;
            rw::kinematics::Frame* _TCPFrame;
            rw::kinematics::MovableFrame* _movableFrame;
            rw::models::Object::Ptr _leftFingerObject;
            rw::models::Object::Ptr _rightFingerObject;
            rwsim::dynamics::Body::Ptr _leftFingerBody;
            rwsim::dynamics::Body::Ptr _rightFingerBody;
        };

    }
}


