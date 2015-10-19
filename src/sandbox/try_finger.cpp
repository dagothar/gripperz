#include <iostream>
#include <string>
#include <sstream>
#include <geometry/JawPrimitive.hpp>
#include <rwlibs/csg/CSGModel.hpp>
#include <rwlibs/csg/CSGModelFactory.hpp>
#include <rw/geometry/GeometryUtil.hpp>
#include <rw/math/InertiaMatrix.hpp>
#include <rw/loaders/model3d/STLFile.hpp>

using namespace std;
using namespace rw::geometry;
using namespace rw::common;
using namespace rw::loaders;
using namespace rw::math;
using namespace rwlibs::csg;
using namespace gripperz::geometry;

int main(int argc, char* argv[]) {

    const double sliceDepth = 0.001;
    const double sliceSize = 1000.0;

    Q q(11);

    q(0) = 0;
    q(1) = 0.1;
    q(2) = 0.025;
    q(3) = 0.02;
    q(4) = 0.5;
    q(5) = 45 * Deg2Rad;
    q(6) = 0.075;
    q(7) = 0.018;
    q(8) = 110.0 * Deg2Rad;
    q(9) = 0.0;
    q(10) = 0 * Deg2Rad;

    Geometry::Ptr fingerGeo = ownedPtr(new Geometry(new JawPrimitive(q), std::string("FingerGeo")));
    //fingerGeo->setTransform(Transform3D<>(RPY<>(0, -90 * Deg2Rad, 0).toRotation3D()));
    TriMesh::Ptr mesh = fingerGeo->getGeometryData()->getTriMesh();

    unsigned n = 0;
    for (double z = 0.0; z < 0.1; z += 0.001) {

        CSGModel::Ptr probe = CSGModelFactory::makeBox(sliceSize, sliceSize, sliceDepth);
        probe->translate(0, 0, z + sliceDepth / 2);

        CSGModel::Ptr slice = new CSGModel(*mesh);
        slice->rotate(0, -90 * Deg2Rad, 0);
        slice->intersect(probe);

        double area = GeometryUtil::estimateVolume(*slice->getTriMesh()) / sliceDepth;
        if (area > 0.0) {
            Geometry::Ptr geometry = new Geometry(slice->getTriMesh());
            Vector3D<> cog = GeometryUtil::estimateCOG(*slice->getTriMesh());
            InertiaMatrix<> I = GeometryUtil::estimateInertia(area,{geometry}, Transform3D<>(-cog));

            cout << I(1, 1) << endl;
        } else {
            cout << 0.0 << endl;
        }

        stringstream sstr;
        sstr << "slice_" << n << ".stl";
        string slicename = sstr.str();
        STLFile::save(*slice->getTriMesh(), slicename);

        ++n;
    }

    return 0;
}

