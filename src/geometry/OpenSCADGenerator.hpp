/* 
 * File:   OpenSCADGenerator.hpp
 * Author: dagothar
 *
 * Created on October 21, 2015, 3:48 PM
 */

#pragma once

#include "ParametrizedMeshGenerator.hpp"

namespace gripperz {
    namespace geometry {

        /**
         * @class OpenSCADGenerator
         * @brief Uses OpenSCAD to implement mesh generation.
         */
        class OpenSCADGenerator : public ParametrizedMeshGenerator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<OpenSCADGenerator> Ptr;

        public:
            OpenSCADGenerator(
                    const std::string& script,
                    const std::string& temporaryFilename = ".mesh",
                    const std::string& command = "openscad"
                    );

            virtual ~OpenSCADGenerator();

            virtual rw::geometry::TriMesh::Ptr createMesh();

            void setTemporaryFilename(const std::string& _temporaryFilename) {
                this->_temporaryFilename = _temporaryFilename;
            }

            std::string getTemporaryFilename() const {
                return _temporaryFilename;
            }

            void setScriptFilename(const std::string& _scriptFilename) {
                this->_scriptFilename = _scriptFilename;
            }

            std::string getScriptFilename() const {
                return _scriptFilename;
            }

            void setCommand(const std::string& _command) {
                this->_command = _command;
            }

            std::string getCommand() const {
                return _command;
            }

        private: 
            std::string _scriptFilename;
            std::string _temporaryFilename;
            std::string _command;
        };

    }
}


