#include <iostream>
#include <string>
#include <sstream>
#include <rw/loaders/model3d/STLFile.hpp>
#include <rwlibs/csg/CSGModel.hpp>
#include <rwlibs/csg/CSGModelFactory.hpp>
#include <rw/geometry/GeometryUtil.hpp>
#include <rw/math/InertiaMatrix.hpp>

using namespace std;
using namespace rw::geometry;
using namespace rw::loaders;
using namespace rw::math;
using namespace rwlibs::csg;

int main(int argc, char* argv[]) {

    const double sliceDepth = 0.001;
    const double sliceSize = 1000.0;

    if (argc < 2) {
        cout << "Usage: try_slice [STL file]" << endl;
        return -1;
    }

    string filename = argv[1];

    //cout << "Loading " << filename << endl;
    TriMesh::Ptr mesh = STLFile::load(filename);

    unsigned n = 0;
    for (double z = 0.0; z < 0.1; z += 0.001) {

        CSGModel::Ptr probe = CSGModelFactory::makeBox(sliceSize, sliceSize, sliceDepth);
        probe->translate(0, 0, z);

        CSGModel::Ptr slice = new CSGModel(*mesh);
        slice->intersect(probe);
        
        double area = GeometryUtil::estimateVolume(*slice->getTriMesh()) / sliceDepth;
        Geometry::Ptr geometry = new Geometry(slice->getTriMesh());
        Vector3D<> cog = GeometryUtil::estimateCOG(*slice->getTriMesh());
        InertiaMatrix<> I = GeometryUtil::estimateInertia(area, {geometry}, Transform3D<>(-cog));
      
        
        //cout << "SLICE " << n << endl;
        //cout << " - area = " << area << endl;
        //cout << " - inertia matrix: " << I << endl;
        //cout << " - Iyy = " << I(1, 1) << endl;
        
        cout << I(1, 1) << endl;
        
        stringstream sstr;
        sstr << "slice_" << n << ".stl";
        string slicename = sstr.str();
        STLFile::save(*slice->getTriMesh(), slicename);
        
        ++n;
    }

    return 0;
}

