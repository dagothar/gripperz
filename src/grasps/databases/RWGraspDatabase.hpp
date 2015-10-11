/* 
 * File:   RWGraspDatabase.hpp
 * Author: dagothar
 *
 * Created on October 9, 2015, 9:49 AM
 */

#pragma once

#include "../GraspDatabase.hpp"

namespace gripperz {
    namespace grasps {
        namespace databases {

            /**
             * @class RWGraspDatabase
             * @brief
             */
            class RWGraspDatabase : public GraspDatabase {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<RWGraspDatabase> Ptr;

            public:
                RWGraspDatabase(const std::string& filename);

                virtual ~RWGraspDatabase();

                virtual Grasps loadGrasps();

                void setFilename(std::string _filename) {
                    this->_filename = _filename;
                }

                std::string getFilename() const {
                    return _filename;
                }

            private:
                std::string _filename;
                Grasps _grasps;
            };

        }
    }
}


