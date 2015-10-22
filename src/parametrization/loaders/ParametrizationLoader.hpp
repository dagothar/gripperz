/* 
 * File:   ParametrizationLoader.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 1:13 PM
 */

#pragma once

#include "../Parametrization.hpp"
#include <boost/property_tree/ptree.hpp>

namespace gripperz {
    namespace parametrization {
        namespace loaders {

            /**
             * @class ParametrizationLoader
             * @brief
             */
            class ParametrizationLoader {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<ParametrizationLoader> Ptr;

            public:
                ParametrizationLoader();

                virtual ~ParametrizationLoader();

                /**
                 * Loads the Parametrization from an XML file
                 * @param filename
                 * @return 
                 */
                virtual Parametrization::Ptr load(const std::string& filename);

                /**
                 * Reads the Parametrization from a ptree
                 * @param tree
                 * @return 
                 */
                virtual Parametrization::Ptr read(boost::property_tree::ptree& tree);

                /**
                 * Saves the Parametrization to an XML file
                 * @param filename
                 * @param parametrization
                 */
                virtual void save(const std::string& filename, Parametrization::Ptr parametrization);

                /**
                 * Writes the Parametrization to a ptree
                 * @param tree
                 */
                virtual void write(boost::property_tree::ptree& tree);

            };

        }
    }
}

