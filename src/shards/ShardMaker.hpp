/* 
 * File:   ShardMaker.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 8:48 PM
 */

#pragma once

#include "ShardContainer.hpp"

namespace gripperz {
    namespace shards {

        /**
         * @class ShardMaker
         * @brief Base for classes creating their Shards
         */
        template<class T>
        class ShardMaker {
        public:

            virtual ~ShardMaker() {
            }

            static Shard makeShard(ShardContainer& container) {
                T* parent = new T();
                Shard shard = parent->doMakeShard(container);
                delete parent;
                
                return shard;
            }

            virtual Shard doMakeShard(ShardContainer& container) const = 0;

        };

    }
}


