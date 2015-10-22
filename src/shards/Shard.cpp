/* 
 * File:   Shard.cpp
 * Author: dagothar
 * 
 * Created on October 22, 2015, 7:01 PM
 */

#include "Shard.hpp"

using namespace gripperz::shards;
using namespace boost;

Shard::Shard(const std::string& cls, const std::string& id, boost::any object) :
_cls(cls),
_qualifier(id),
_object(object) {

}

Shard::~Shard() {
}