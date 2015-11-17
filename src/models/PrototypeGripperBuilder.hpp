/* 
 * File:   PrototypeGripperBuilder.hpp
 * Author: dagothar
 *
 * Created on November 17, 2015, 10:32 AM
 */

#pragma once

#include "GripperBuilder.hpp"
#include <parametrization/ParametrizationTranslator.hpp>

namespace gripperz {
    namespace models {

        /**
         * @class PrototypeGripperBuilder
         * @brief
         */
        class PrototypeGripperBuilder : public GripperBuilder {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<PrototypeGripperBuilder> Ptr;
        public:
            //! Constructor.
            PrototypeGripperBuilder(ParametrizedGripper::Ptr prototype, parametrization::ParametrizationTranslator::Ptr translator);

            //! Destructor.
            virtual ~PrototypeGripperBuilder();
            
            virtual ParametrizedGripper::Ptr vectorToGripper(const math::Vector& params);

            virtual math::Vector gripperToVector(ParametrizedGripper::Ptr gripper);

            void setTranslator(parametrization::ParametrizationTranslator::Ptr _translator) {
                this->_translator = _translator;
            }

            parametrization::ParametrizationTranslator::Ptr getTranslator() const {
                return _translator;
            }

            void setPrototype(ParametrizedGripper::Ptr _prototype) {
                this->_prototype = _prototype;
            }

            ParametrizedGripper::Ptr getPrototype() const {
                return _prototype;
            }
            
        private:
            ParametrizedGripper::Ptr _prototype;
            parametrization::ParametrizationTranslator::Ptr _translator;
        };
    }
}
