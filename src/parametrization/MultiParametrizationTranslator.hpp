/* 
 * File:   MultiParametrizationTranslator.hpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 10:58 AM
 */

#pragma once

#include "ParametrizationTranslator.hpp"
#include <initializer_list>

namespace gripperz {
    namespace parametrization {

        /**
         * @class MultiParametrizationTranslator
         * @brief
         */
        class MultiParametrizationTranslator : public ParametrizationTranslator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<MultiParametrizationTranslator> Ptr;

            class Slot {
            public:
                Slot(const std::string& name) : _names{name} {
                }

                Slot(const std::vector<std::string>& names) : _names(names) {
                }

                Slot(std::initializer_list<std::string> names) : _names(names) {
                }

                ~Slot() {
                }

                const std::vector<std::string>& getParameterNames() const {
                    return _names;
                }

                int size() const {
                    return _names.size();
                }

            private:
                std::vector<std::string> _names;
            };

        public:
            //! Constructor.
            MultiParametrizationTranslator(const std::vector<Slot>& slots = std::vector<Slot>());
            
            MultiParametrizationTranslator(std::initializer_list<Slot> slots);

            //! Destructor.
            virtual ~MultiParametrizationTranslator();

            virtual ValueVector parametrizationToVector(Parametrization::Ptr parametrization);

            virtual Parametrization::Ptr vectorToParametrization(const ValueVector& vector);

            virtual Parametrization::Ptr applyVectorToParametrization(Parametrization::Ptr parametrization, const ValueVector& vector);

            void setSlots(const std::vector<Slot>& _slots) {
                this->_slots = _slots;
            }

            const std::vector<Slot>& getSlots() const {
                return _slots;
            }

        protected:
            std::vector<std::string> getParameterNameList() const;

        private:
            std::vector<Slot> _slots;
        };
    }
}
