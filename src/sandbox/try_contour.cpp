#include <iostream>
#include <rw/geometry/Contour2D.hpp>
#include <rw/loaders/model3d/STLFile.hpp>

using namespace std;
using namespace rw::geometry;
using namespace rw::loaders;

int main(int argc, char* argv[]) {
    
    // make contour
    Contour2D::Point p1{{0, 0}, {0, 0}};
    Contour2D::Point p2{{0, 1}, {0, 0}};
    Contour2D::Point p3{{1, 2}, {0, 0}};
    Contour2D::Point p4{{2, 1}, {0, 0}};
    Contour2D::Point p5{{2, 0}, {0, 0}};
    
    vector<Contour2D::Point> points{p1, p2, p3, p4, p5};
  
    Contour2D::Ptr contour = new Contour2D({1, 1}, points);
    Contour2D::write(*contour, "contour.txt");
    
    // what is the outer contour?
    Contour2D::Ptr outer = Contour2D::getOuterContour(*contour, 0.1);
    Contour2D::write(*outer, "outer.txt");
    
    // make mesh
    TriMesh::Ptr mesh = contour->toTriMesh(1.0);
    STLFile::save(*mesh, "mesh.stl");
    
    return 0;
}
