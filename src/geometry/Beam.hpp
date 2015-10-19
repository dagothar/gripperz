/* 
 * File:   Beam.hpp
 * Author: dagothar
 *
 * Created on October 19, 2015, 1:53 PM
 */

#pragma once

#include <vector>
#include <rw/common/Ptr.hpp>
#include <rw/geometry/TriMesh.hpp>

namespace gripperz {
    namespace geometry {

        /**
         * @class Beam
         * @brief
         */
        class Beam {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Beam> Ptr;
            
            //! Defines a force applied to the beam (magnitude, position)
            typedef struct { double magnitude, position; } Force;
            
            //! Defines a moment applied to the beam (magnitude, position)
            typedef struct { double magnitude, position; } Moment;

        public:
            Beam(rw::geometry::TriMesh::Ptr mesh);

            virtual ~Beam();
            
            /**
             * Calculates a bending moment acting at the position x.
             * @param x position
             * @return bending moment
             */
            virtual double moment(double x) = 0;
            
            /**
             * Calculates stress in the beam at the position x
             * @param x position
             * @return stress
             */
            virtual double stress(double x) = 0;

            void setMoments(const std::vector<Moment>& _moments) {
                this->_moments = _moments;
            }

            std::vector<Moment> getMoments() const {
                return _moments;
            }
            
            void addMoment(const Moment& moment) {
                _moments.push_back(moment);
            }
            
            void addMoment(double magnitude, double position) {
                _moments.push_back({magnitude, position});
            }

            void setForces(const std::vector<Force>& _forces) {
                this->_forces = _forces;
            }

            std::vector<Force> getForces() const {
                return _forces;
            }
            
            void addForce(const Force& force) {
                _forces.push_back(force);
            }

            void addForce(double magnitude, double position) {
                _forces.push_back({magnitude, position});
            }
            
            void setBeamMesh(rw::geometry::TriMesh::Ptr _beamMesh) {
                this->_beamMesh = _beamMesh;
            }

            rw::geometry::TriMesh::Ptr getBeamMesh() const {
                return _beamMesh;
            }
            
        private:
            rw::geometry::TriMesh::Ptr _beamMesh;
            std::vector<Force> _forces;
            std::vector<Moment> _moments;
        };

    }
}


