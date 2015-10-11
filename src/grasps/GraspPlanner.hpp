/* 
 * File:   GraspPlanner.hpp
 * Author: dagothar
 *
 * Created on October 8, 2015, 3:48 PM
 */

#pragma once

#include "GraspSource.hpp"

namespace gripperz {
    namespace grasps {

        /**
         * @class GraspPlanner
         * @brief
         */
        class GraspPlanner : public GraspSource {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GraspPlanner> Ptr;

        public:
            GraspPlanner(unsigned nGrasps);

            virtual ~GraspPlanner();

            virtual Grasps getGrasps();

            virtual Grasps planGrasps(unsigned nGrasps) = 0;

            void setNumberOfGraspsToPlan(unsigned _numberOfGraspsToPlan) {
                this->_numberOfGraspsToPlan = _numberOfGraspsToPlan;
            }

            unsigned getNumberOfGraspsToPlan() const {
                return _numberOfGraspsToPlan;
            }

        private:
            unsigned _numberOfGraspsToPlan;
            Grasps _grasps;
        };

    }
}


