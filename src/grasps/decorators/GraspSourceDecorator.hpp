/* 
 * File:   GraspSourceDecorator.hpp
 * Author: dagothar
 *
 * Created on October 11, 2015, 8:14 PM
 */

#pragma once

#include "../GraspSource.hpp"

namespace gripperz {
    namespace grasps {
        namespace decorators {

            /**
             * @class GraspSourceDecorator
             * @brief
             */
            class GraspSourceDecorator : public GraspSource {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GraspSourceDecorator> Ptr;

            public:
                GraspSourceDecorator(GraspSource::Ptr graspSource);

                virtual ~GraspSourceDecorator();

                virtual Grasps getGrasps();

            private:
                GraspSource::Ptr _graspSource;
            };

        }
    }

}
