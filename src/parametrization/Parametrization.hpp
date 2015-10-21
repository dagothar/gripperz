/* 
 * File:   Parametrization.hpp
 * Author: dagothar
 *
 * Created on October 20, 2015, 10:32 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <vector>
#include <map>
#include <string>
#include "Types.hpp"

namespace gripperz {
    namespace parametrization {

        /**
         * @class Parametrization
         * @brief
         */
        class Parametrization {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Parametrization> Ptr;

            typedef std::vector<Parameter> ParameterList;

            typedef std::vector<ParameterName> ParameterNameList;

        public:
            /**
             * Constructor.
             */
            Parametrization();
            
            /**
             * Constructor. 
             * @param list [in] list of parameters
             */
            Parametrization(const ParameterList& list);
            
            /**
             * Constructor.
             * @param nameList [in] list of parameter names
             */
            Parametrization(const ParameterNameList& nameList);

            virtual ~Parametrization();

            void addParameter(const Parameter& p);

            void addParameter(const ParameterName& name, const ParameterValue& value);

            void addParameter(const ParameterName& name);
            
            ParameterList getParameterList() const;
            
            ParameterNameList getParameterNameList() const;
            
            bool hasParameter(const ParameterName& name) const;
            
            ParameterValue getParameter(const ParameterName& name) const;
            
            void setParameter(const Parameter& p);
            
            void setParameter(const ParameterName& name, const ParameterValue& value);

        private:
            std::map<ParameterName, ParameterValue> _parameters;
        };

    }
}


