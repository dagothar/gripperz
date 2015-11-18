#include <iostream>
#include <math.hpp>
#include <optimization.hpp>
#include <util/Rosenbrock.hpp>
#include <rw/common/macros.hpp>

using namespace std;
using namespace gripperz::math;
using namespace gripperz::optimization;
using namespace gripperz::util;

class Test : public ObjectiveFunction {
public:
    typedef rw::common::Ptr<Test> Ptr;
    

    virtual Scalar evaluate(const Vector& v) {
        RW_ASSERT(v.size() == 2);
        
        double x = v[0] - 1.0;
        double y = v[1] - 1.5;
        
        return x*x + y*y;
    }

};

int main(int argc, char* argv[]) {
    ObjectiveFunction::Ptr objective = new Rosenbrock();
    RbfOptimizer::Configuration config;
    config.maxNOfIterations = 1000;
    config.maxNOfEvaluations = 100;
    Optimizer::Ptr optimizer = new RbfOptimizer(config);
    OptimizationManager::Ptr manager = new OptimizationManager(optimizer, {{-10, 10}, {-10, 10}}, true);
    
    Vector result = manager->optimize(objective, {0, 0}, "minimize");
    
    cout << "Result:" << endl;
    cout << "* value= " << objective->evaluate(result) << endl;
    cout << "* at= " << result << endl;
    
    return 0;
}