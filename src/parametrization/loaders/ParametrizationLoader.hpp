/* 
 * File:   ParametrizationLoader.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 1:13 PM
 */

#pragma once

#include "../Parametrization.hpp"
#include <loaders/Loader.hpp>

namespace gripperz {
    namespace parametrization {
        namespace loaders {

            /**
             * @class ParametrizationLoader
             * @brief
             */
            class ParametrizationLoader : public gripperz::loaders::Loader<Parametrization::Ptr> {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<ParametrizationLoader> Ptr;

            public:
                ParametrizationLoader();

                virtual ~ParametrizationLoader();

                /**
                 * Reads the Parametrization from a ptree
                 * @param tree
                 * @return 
                 */
                virtual Parametrization::Ptr read(const boost::property_tree::ptree& tree);

                /**
                 * Writes the Parametrization to a ptree
                 * @param tree
                 */
                virtual boost::property_tree::ptree write(Parametrization::Ptr parametrization);

            };

        }
    }
}

