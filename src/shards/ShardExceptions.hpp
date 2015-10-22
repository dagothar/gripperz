/* 
 * File:   ShardExceptions.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 8:00 PM
 */

#pragma once

#include <exception>
#include <sstream>

namespace gripperz {
    namespace shards {

        /**
         * Base shard exception
         */
        class shard_exception : public std::exception {
        public:

            virtual ~shard_exception() {

            }

            virtual const char* what() const throw () = 0;
        };

        /**
         * Thrown when Shard is not found
         * @param cls
         */
        class unresolved_shard_exception : public shard_exception {
        public:

            unresolved_shard_exception(const std::string& cls) :
            _cls(cls) {
            }

            virtual ~unresolved_shard_exception() {

            }

            virtual const char* what() const throw () {
                std::stringstream sstr;
                sstr << "Cannot resolve shard '" << _cls << "'";
                return sstr.str().c_str();
            }

        private:
            std::string _cls;
        };
        
        /**
         * Thrown when Shard is ambiguous
         * @param cls
         */
        class ambiguous_shard_exception : public shard_exception {
        public:

            ambiguous_shard_exception(const std::string& cls) :
            _cls(cls) {
            }

            virtual ~ambiguous_shard_exception() {

            }

            virtual const char* what() const throw () {
                std::stringstream sstr;
                sstr << "Ambiguous resolution for shard '" << _cls << "'";
                return sstr.str().c_str();
            }

        private:
            std::string _cls;
        };

    }
}


