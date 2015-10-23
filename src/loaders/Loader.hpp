/* 
 * File:   Loader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 12:05 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <util/XMLHelpers.hpp>

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

            virtual T load(const std::string& filename) {
                rw::common::Ptr<boost::property_tree::ptree> tree = util::XMLHelpers::parseXMLFile(filename);
                
                T object = read(*tree);
                
                return object;
            }

            virtual T read(const boost::property_tree::ptree& tree) = 0;

            virtual void save(const std::string& filename, T object) {
                boost::property_tree::ptree tree = write(object);
                
                util::XMLHelpers::saveXMLFile(filename, &tree);
            }

            virtual boost::property_tree::ptree write(T object) = 0;

        private:

        };

    }
}


