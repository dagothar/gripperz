/* 
 * File:   SurfaceSampleGraspSampler.hpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 12:58 PM
 */

#pragma once

#include "GraspSampler.hpp"
#include <grasps/SurfaceSample.hpp>

namespace gripperz {
    namespace grasps {
        namespace planners {

            /**
             * @class SurfaceSampleGraspSampler
             * @brief
             */
            class SurfaceSampleGraspSampler : public GraspSampler {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<SurfaceSampleGraspSampler> Ptr;
            public:
                //! Constructor.
                SurfaceSampleGraspSampler(const std::vector<SurfaceSample>& ssamples);

                //! Destructor.
                virtual ~SurfaceSampleGraspSampler();

                virtual rw::math::Transform3D<> sample();

                void setSurfaceSamples(const std::vector<SurfaceSample>& ssamples) {
                    _ssamples = ssamples;
                    _idx = _ssamples.begin();
                }

               const std::vector<SurfaceSample>& getSurfaceSamples() const {
                    return _ssamples;
                }
                
            private:
                std::vector<SurfaceSample> _ssamples;
                std::vector<SurfaceSample>::iterator _idx;
            };
        }
    }
}