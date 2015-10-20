/**
 * @file Context.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include <rw/models/WorkCell.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>
#include <rw/kinematics/State.hpp>

namespace gripperz {
    namespace context {

        /**
         * @class Context
         * @brief Base class for storing task context.
         */
        class Context {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Context> Ptr;

        public:
            Context(rwsim::dynamics::DynamicWorkCell::Ptr dwc);

            virtual ~Context();

            void setInitState(const rw::kinematics::State& _initState) {
                this->_initState = _initState;
            }

            rw::kinematics::State& getInitState() {
                return _initState;
            }

            void setDynamicWorkCell(rwsim::dynamics::DynamicWorkCell::Ptr _dynamicWorkCell) {
                this->_dynamicWorkCell = _dynamicWorkCell;
            }

            rwsim::dynamics::DynamicWorkCell::Ptr getDynamicWorkCell() const {
                return _dynamicWorkCell;
            }

            void setWorkCell(rw::models::WorkCell::Ptr _workCell) {
                this->_workCell = _workCell;
            }

            rw::models::WorkCell::Ptr getWorkCell() const {
                return _workCell;
            }

        private:
            rw::models::WorkCell::Ptr _workCell;
            rwsim::dynamics::DynamicWorkCell::Ptr _dynamicWorkCell;
            rw::kinematics::State _initState;
        };

    } /* context */
} /* gripperz */
