/* 
 * File:   GraspTargetCompare.hpp
 * Author: dagothar
 *
 * Created on October 26, 2015, 2:44 PM
 */

#pragma once

#include <grasps/filters/SortingFilter.hpp>

/**
 * @class GraspTargetCompare
 * @brief
 */
class GraspTargetCompare : public gripperz::grasps::filters::SortingFilter::Compare {
public:
    //! Smart pointer.
    typedef rw::common::Ptr<GraspTargetCompare> Ptr;

public:
    GraspTargetCompare(const rw::math::Transform3D<>& origin);

    virtual ~GraspTargetCompare();

    virtual bool operator()(rwlibs::task::GraspTarget t1, rwlibs::task::GraspTarget t2);

private:
    rw::math::Transform3D<> _origin;
};


