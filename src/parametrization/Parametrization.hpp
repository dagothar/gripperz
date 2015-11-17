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
#include <iostream>
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
            
            virtual Parametrization::Ptr clone() const;

            void addParameter(const Parameter& p);

            void addParameter(const ParameterName& name, const ParameterValue& value);

            void addParameter(const ParameterName& name);

            ParameterList getParameterList() const;

            ParameterNameList getParameterNameList() const;

            bool hasParameter(const ParameterName& name) const;

            ParameterValue getParameter(const ParameterName& name) const;

            void setParameter(const Parameter& p);

            void setParameter(const ParameterName& name, const ParameterValue& value);

            /**
             * Merges other parametrization into this one.
             * Skips parameters that already exist.
             * @param parametrization
             */
            void merge(Parametrization::Ptr parametrization);
            
            /**
             * Patches other parametrization into this one.
             * Same as merge, but updates parameters that do already exist with new values.
             * @param parametrization
             */
            void patch(Parametrization::Ptr parametrization);

        private:
            ParameterNameList _parameterNameList;
            std::map<ParameterName, ParameterValue> _parameters;
        };

        template<class T>
        T& operator<<(T& stream, const Parametrization& parametrization) {

            stream << "Parametrization:\n";

            Parametrization::ParameterList params = parametrization.getParameterList();

            for (
                    Parametrization::ParameterList::iterator i = params.begin();
                    i != params.end();
                    ++i
                    ) {
                stream << " - " << i->first << "= " << i->second << "\n";
            }
            stream << std::endl;

            return stream;
        }

    }
}


