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
            //! Type for the the class stored by the shard
            typedef std::string ShardClass;

            //! Type for the shard id
            typedef std::string ShardId;

            //! Type for internal shard storage
            typedef std::pair<ShardClass, Shard> ShardPair;

            //! List of stored ShardPairs
            typedef std::vector<ShardPair> ShardPairList;

        public:
            ShardContainer();

            virtual ~ShardContainer();

            void addShard(const ShardClass& cls, const Shard& shard);

            Shard getShard(const ShardClass& cls, const ShardId& id = "") const;

            ShardPairList getShards() const;

        private:
            std::multimap<ShardClass, Shard> _shards;
        };

    }
}


