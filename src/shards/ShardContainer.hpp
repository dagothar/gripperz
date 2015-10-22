/* 
 * File:   ShardContainer.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 7:27 PM
 */

#pragma once

#include "Shard.hpp"
#include <map>

namespace gripperz {
    namespace shards {

        /**
         * @class ShardContainer
         * @brief 
         */
        class ShardContainer {
        public:
            //! Type for internal shard storage
            typedef std::pair<Shard::ShardClass, Shard> ShardPair;

            //! List of stored ShardPairs
            typedef std::vector<ShardPair> ShardPairList;

        public:
            ShardContainer();

            virtual ~ShardContainer();

            void addShard(const Shard& shard);

            Shard getShard(const Shard::ShardClass& cls, const Shard::ShardQualifier& qual = "") const;

            ShardPairList getShards() const;

        private:
            std::multimap<Shard::ShardClass, Shard> _shards;
        };

    }
}


