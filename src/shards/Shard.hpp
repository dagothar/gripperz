/* 
 * File:   Shard.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 6:57 PM
 */

#pragma once

#include <boost/any.hpp>

namespace gripperz {
    namespace shards {

        /**
         * @class Shard
         * @brief Represents a program module
         */
        class Shard {
        public:
            //! Type for the the class stored by the shard
            typedef std::string ShardClass;

            //! Type for the shard id
            typedef std::string ShardQualifier;
            
        public:
            Shard(const ShardClass& cls, const ShardQualifier& qual, boost::any object);

            virtual ~Shard();

            template<class T>
            T cast() {
                return boost::any_cast<T>(_object);
            }

            ShardQualifier getQualifier() const {
                return _qualifier;
            }

            void Qualifier(const ShardQualifier& qual) {
                _qualifier = qual;
            }

            void setClass(const ShardClass& _cls) {
                this->_cls = _cls;
            }

            ShardClass getClass() const {
                return _cls;
            }

        private:
            ShardClass _cls;
            ShardQualifier _qualifier;
            boost::any _object;
        };

    }
}


