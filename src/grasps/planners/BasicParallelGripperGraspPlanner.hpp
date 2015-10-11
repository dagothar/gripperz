/* 
 * File:   BasicParallelGripperGraspPlanner.hpp
 * Author: dagothar
 *
 * Created on October 8, 2015, 4:25 PM
 */

#pragma once

#include "../GraspPlanner.hpp"
#include <grasps/SurfaceSample.hpp>
#include <context/TaskDescription.hpp>
#include <rw/kinematics/State.hpp>

namespace gripperz {
    namespace grasps {
        namespace planners {

            /**
             * @class BasicParallelGripperGraspPlanner
             * @brief
             */
            class BasicParallelGripperGraspPlanner : public GraspPlanner {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<BasicParallelGripperGraspPlanner> Ptr;

            public:
                BasicParallelGripperGraspPlanner(unsigned nGrasps, const rw::kinematics::State& state, context::TaskDescription::Ptr td);

                virtual ~BasicParallelGripperGraspPlanner();

                virtual Grasps planGrasps(unsigned nGrasps);

                void setSurfaceSamples(const std::vector<SurfaceSample>& _ssamples) {
                    this->_ssamples = _ssamples;
                }

                std::vector<SurfaceSample> getSurfaceSamples() const {
                    return _ssamples;
                }

                void setTaskDescription(context::TaskDescription::Ptr _td) {
                    this->_td = _td;
                }

                context::TaskDescription::Ptr getTaskDescription() const {
                    return _td;
                }

                void setState(const rw::kinematics::State& _state) {
                    this->_state = _state;
                }

                rw::kinematics::State getState() const {
                    return _state;
                }

            private:

                rw::kinematics::State _state;
                context::TaskDescription::Ptr _td;
                std::vector<SurfaceSample> _ssamples;
            };

        }
    }

}