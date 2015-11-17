/* 
 * File:   ParametrizedGeometryGripper.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 11:01 AM
 */

#pragma once

#include "ParametrizedGripper.hpp"
#include <geometry/ParametrizedMeshGenerator.hpp>

namespace gripperz {
    namespace models {

        /**
         * @class ParametrizedGeometryGripper
         * @brief
         */
        class ParametrizedGeometryGripper : public ParametrizedGripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizedGeometryGripper> Ptr;

        public:
            ParametrizedGeometryGripper(const std::string& name);
            
            ParametrizedGeometryGripper(const ParametrizedGripper& gripper);

            virtual ~ParametrizedGeometryGripper();
            
            virtual ParametrizedGeometryGripper* clone() const;
            
            virtual void applyModifications(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state);

            void setMeshGenerator(geometry::ParametrizedMeshGenerator::Ptr _meshGenerator) {
                this->_meshGenerator = _meshGenerator;
            }

            geometry::ParametrizedMeshGenerator::Ptr getMeshGenerator() const {
                return _meshGenerator;
            }

        private:
            geometry::ParametrizedMeshGenerator::Ptr _meshGenerator;

        };

    }
}


