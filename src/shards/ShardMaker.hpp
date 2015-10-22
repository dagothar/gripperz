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
         * @brief Base for classes creating their Shards.
         * Important: the class should have a default constructor!
         */
        template<class T>
        class ShardMaker {
        public:

            virtual ~ShardMaker() {
            }

            static Shard makeShard(ShardContainer& container, const Shard::ShardQualifier& qual) {
                T* parent = new T();
                Shard shard = parent->doMakeShard(container, qual);
                delete parent;
                
                return shard;
            }

            virtual Shard doMakeShard(ShardContainer& container, const Shard::ShardQualifier& qual) const = 0;

        };
        
        /**
         * Enables simple syntax: Shard shard = makeShard<TYPE>(container);
         * Can be specialized to allow making shards of classes without access to the code
         * @param container
         * @return 
         */
        template<class T>
        Shard makeShard(ShardContainer& container, const Shard::ShardQualifier& qual) {
            return T::makeShard(container, qual);
        }
        
        /**
         * Enables making shards out of custom objects.
         * Class ID is mangled!
         * @param container
         * @param qual
         * @param data
         * @return 
         */
        template<class T>
        Shard makeShard(ShardContainer& container, const Shard::ShardQualifier& qual, T& object) {
            return Shard(typeid(T).name(), qual, object);
        }
        
        template<class T>
        Shard makeShard(ShardContainer& container, const Shard::ShardQualifier& qual, const T& object) {
            return Shard(typeid(T).name(), qual, object);
        }
        
        //! Specialized template for strings.
        template<>
        Shard makeShard(ShardContainer& container, const Shard::ShardQualifier& qual, const std::string& object) {
            return Shard("string", qual, object);
        }
        
        //! Specialized template for int.
        template<>
        Shard makeShard(ShardContainer& container, const Shard::ShardQualifier& qual, const int& object) {
            return Shard("int", qual, object);
        }
        
        //! Specialized template for doubles.
        template<>
        Shard makeShard(ShardContainer& container, const Shard::ShardQualifier& qual, const double& object) {
            return Shard("double", qual, object);
        }

    }
}


