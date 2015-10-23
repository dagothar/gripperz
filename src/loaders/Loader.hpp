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
               boost::property_tree::ptree tree = util::XMLHelpers::parseXMLFile(filename);
                
                T object = read(tree.begin()->second);
                
                return object;
            }

            virtual T read(const boost::property_tree::ptree& tree) = 0;

            virtual void save(const std::string& filename, T object) {
                std::pair<std::string, boost::property_tree::ptree> trunk = write(object);
                
                boost::property_tree::ptree root;
                root.put_child(trunk.first, trunk.second);
                
                util::XMLHelpers::saveXMLFile(filename, root);
            }

            /**
             * Writes the object to the property tree.
             * @param object
             * @return pair (suggested root tag; ptree)
             */
            virtual std::pair<std::string, boost::property_tree::ptree> write(T object) = 0;

        private:

        };

    }
}


