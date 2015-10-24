/* 
 * File:   GraspGridFilter.hpp
 * Author: dagothar
 *
 * Created on October 24, 2015, 8:19 PM
 */

#pragma once

#include "GraspFilter.hpp"
#include <rwlibs/task/GraspTarget.hpp>

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class GraspGridFilter
             * @brief
             */
            class GraspGridFilter : public GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GraspGridFilter> Ptr;

            public:
                /**
                 * Constructor.
                 * @param min [in] lower range for pose noise (6 numbers: x, y, z, roll, pitch, yaw)
                 * @param max [in] upper range for pose noise (6 numbers: x, y, z, roll, pitch, yaw)
                 * @param res [in] resolution for individual dimensions (6 numbers: x, y, z, roll, pitch, yaw)
                 */
                GraspGridFilter(const std::vector<double>& min, const std::vector<double>& max, const std::vector<int>& res);

                virtual ~GraspGridFilter();

                virtual Grasps filter(Grasps grasps);
                
            protected:
                std::vector<rwlibs::task::GraspTarget> makeGrid(const rwlibs::task::GraspTarget& seed) const;

            private:
                std::vector<double> _min;
                std::vector<double> _max;
                std::vector<int> _res;
            };

        }
    }
}

