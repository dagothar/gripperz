/* 
 * File:   GraspFilterChain.cpp
 * Author: dagothar
 * 
 * Created on October 8, 2015, 12:36 PM
 */

#include "GraspFilterChain.hpp"

using namespace std;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;

GraspFilterChain::GraspFilterChain() {
}

GraspFilterChain::~GraspFilterChain() {
}

Grasps GraspFilterChain::filter(Grasps grasps) {
    
    Grasps filtered = grasps;
    
    for (vector<GraspFilter::Ptr>::iterator i = _filters.begin(); i != _filters.end(); ++i) {
        filtered = (*i)->filter(filtered);
    }
    
    return filtered;
}

void GraspFilterChain::addFilter(GraspFilter::Ptr filter) {
    _filters.push_back(filter);
}