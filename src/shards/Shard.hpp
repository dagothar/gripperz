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
            Shard(const std::string& cls, const std::string& id, boost::any object);

            virtual ~Shard();

            template<class T>
            T cast() {
                return boost::any_cast<T>(_object);
            }

            std::string getId() const {
                return _id;
            }

            void setId(const std::string& id) {
                _id = id;
            }

            void setClass(const std::string& _cls) {
                this->_cls = _cls;
            }

            std::string getClass() const {
                return _cls;
            }

        private:
            std::string _cls;
            std::string _id;
            boost::any _object;
        };

    }
}


