/* 
 * File:   GraspStatusFilter.hpp
 * Author: dagothar
 *
 * Created on October 10, 2015, 9:16 PM
 */

#pragma once

#include "GraspFilter.hpp"

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class GraspStatusFilter
             * @brief
             */
            class GraspStatusFilter : public GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GraspStatusFilter> Ptr;

            public:
                GraspStatusFilter(const std::vector<rwlibs::task::GraspResult::TestStatus>& allowedStatuses = std::vector<rwlibs::task::GraspResult::TestStatus>());

                virtual ~GraspStatusFilter();

                virtual Grasps filter(Grasps grasps);

                void setAllowedStatuses(const std::vector<rwlibs::task::GraspResult::TestStatus>& allowedStatuses) {
                    this->_allowedStatuses = allowedStatuses;
                }

                std::vector<rwlibs::task::GraspResult::TestStatus> getAllowedStatuses() const {
                    return _allowedStatuses;
                }

                void addAllowedStatus(rwlibs::task::GraspResult::TestStatus status);
                
                bool isAllowed(rwlibs::task::GraspResult::TestStatus status) const;

            private:
                std::vector<rwlibs::task::GraspResult::TestStatus> _allowedStatuses;
            };

        }
    }
}

