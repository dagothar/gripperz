/* 
 * File:   EvaluationProcessManager.hpp
 * Author: dagothar
 *
 * Created on October 7, 2015, 3:39 PM
 */

#ifndef EVALUATIONPROCESSMANAGER_HPP
#define	EVALUATIONPROCESSMANAGER_HPP

#include <rw/common/Ptr.hpp>

namespace gripperz {
    namespace process {

        class EvaluationProcessManager {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<EvaluationProcessManager> Ptr;

        public:
            EvaluationProcessManager();
            virtual ~EvaluationProcessManager();
            
        private:
        };

    }
}

#endif	/* EVALUATIONPROCESSMANAGER_HPP */

