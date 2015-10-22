/* 
 * File:   ShardContainer.cpp
 * Author: dagothar
 * 
 * Created on October 22, 2015, 7:31 PM
 */

#include "ShardContainer.hpp"
#include "ShardExceptions.hpp"
#include <iterator>
#include <algorithm>
#include <boost/foreach.hpp>

using namespace std;
using namespace gripperz::shards;

ShardContainer::ShardContainer() {
}

ShardContainer::~ShardContainer() {
}

void ShardContainer::addShard(const Shard& shard) {
    _shards.insert(make_pair(shard.getClass(), shard));
}

Shard ShardContainer::getShard(const Shard::ShardClass& cls, const Shard::ShardQualifier& qual) const {
    typedef multimap<Shard::ShardClass, Shard>::const_iterator ShardIterator;
    typedef pair<ShardIterator, ShardIterator> ShardRange;

    ShardRange range = _shards.equal_range(cls);
    int range_size = distance(range.first, range.second);

    if (range_size == 0) {
        
        throw unresolved_shard_exception(cls);
        
    } else if (range_size == 1) {
        
        return range.first->second;
        
    } else {
        if (!qual.empty()) {
            
            auto hasId = [&](const ShardPair & p) {
                
                return p.second.getQualifier() == qual;
            };
            
            if (count_if(range.first, range.second, hasId) == 1) {
                
                return find_if(range.first, range.second, hasId)->second;
            }
        }
        
        throw ambiguous_shard_exception(cls);
    }

    // should never reach this spot!
    throw unresolved_shard_exception(cls);
    return Shard("", "", 0);
}

ShardContainer::ShardPairList ShardContainer::getShards() const {
    ShardPairList list;

    BOOST_FOREACH(const ShardPair& shard, _shards) {
        list.push_back(shard);
    }

    return list;
}
