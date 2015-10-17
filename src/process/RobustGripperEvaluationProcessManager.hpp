/**
 * @file RobustEvaluationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#pragma once

#include "GripperEvaluationProcessManager.hpp"
#include <string>

namespace gripperz {
    namespace process {

        /**
         * @class RobustEvaluationManager
         * @brief Performs robust evaluation by delegating to a script running separately.
         * In case of crash, evaluation is re-tried, until successful, or a limit on
         * number of evaluations reached.
         */
        class RobustGripperEvaluationProcessManager : public GripperEvaluationProcessManager {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<RobustGripperEvaluationProcessManager> Ptr;

        public:
            /**
             * @brief Constructor.
             * 
             * @param dwcPath [in] path to dwc file
             * @param tdPath [in] path to td file
             * @param samplesPath [in] path to samples xml file (set to "" to disable using samples)
             * @param config [in] configuration
             * @param nCores [in] number of cores to use in evaluation
             * @param nTries [in] number of tries of evaluation in case of faults
             * @param tmpName [in] name of the temporary gripper file
             * @param executablePath [in] path to gripper evaluator

             */
            RobustGripperEvaluationProcessManager(
                    const std::string& dwcPath,
                    const std::string& tdPath,
                    const std::string& samplesPath,
                    unsigned nCores = 1,
                    unsigned nTries = 5,
                    const std::string& tmpName = ".gripper",
                    const std::string& executablePath = "${GRIPPERZ_ROOT}/bin/gripper_evaluate"
                    );

            virtual ~RobustGripperEvaluationProcessManager();

            virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper);

            const std::string& getDwcPath() const {
                return _dwcPath;
            }

            void setDwcPath(const std::string& path) {
                _dwcPath = path;
            }

            const std::string& getTdPath() const {
                return _tdPath;
            }

            void setTdPath(const std::string& path) {
                _tdPath = path;
            }

            const std::string& getSamplesPath() const {
                return _samplesPath;
            }

            void setSamplesPath(const std::string& path) {
                _samplesPath = path;
            }

            const std::string& getTmpName() const {
                return _tmpName;
            }

            void setTmpName(const std::string& name) {
                _tmpName = name;
            }

            std::string getExecutablePath() const {
                return _executable;
            }

            void setExecutablePath(const std::string& executable) {
                _executable = executable;
            }

            unsigned getNCores() const {
                return _cores;
            }

            void setNCores(unsigned n) {
                _cores = n;
            }

            unsigned getNTries() const {
                return _ntries;
            }

            void setNTries(unsigned n) {
                _ntries = n;
            }

        private:
            std::string _executable;
            unsigned _cores;
            std::string _dwcPath;
            std::string _tdPath;
            std::string _samplesPath;
            std::string _tmpName;
            unsigned _ntries;
        };

    } /* evaluation */
} /* gripperz */
