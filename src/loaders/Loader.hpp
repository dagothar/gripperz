/* 
 * File:   Loader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 12:05 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <boost/property_tree/ptree.hpp>

namespace gripperz {
    namespace loaders {

        /**
         * @class Loader
         * @brief Loaders interface template
         */
        template<class T>
        class Loader {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Loader> Ptr;

        public:

            Loader() {
            }

            virtual ~Loader() {
            }

            virtual T load(const std::string& filename) = 0;

            virtual T read(boost::property_tree::ptree& tree) = 0;

            virtual void save(const std::string& filename, T object) = 0;

            virtual boost::property_tree::ptree write(T object) = 0;

        private:

        };

    }
}


