/**
 * @file gripperz.hpp
 * @author Adam Wolniakowski
 * @date 19-07-2015
 */

#include "optimization.hpp"
#include "evaluation.hpp"
#include "process.hpp"
#include "context.hpp"
#include "loaders.hpp"
#include "util.hpp"
#include "grasps.hpp"
#include "simulation.hpp"
#include "models.hpp"
#include "math.hpp"
#include "geometry.hpp"
#include "parametrization.hpp"

#define USE_GRIPPERZ_NAMESPACE \
	namespace gripperz { \
		using namespace gripperz::optimization; \
		using namespace gripperz::evaluation; \
		using namespace gripperz::evaluation::calculators; \
		using namespace gripperz::process; \
		using namespace gripperz::context; \
		using namespace gripperz::loaders; \
		using namespace gripperz::util; \
		using namespace gripperz::grasps; \
                using namespace gripperz::grasps::filters; \
                using namespace gripperz::grasps::planners; \
                using namespace gripperz::grasps::databases; \
                using namespace gripperz::grasps::decorators; \
                using namespace gripperz::simulation; \
		using namespace gripperz::models; \
		using namespace gripperz::math; \
		using namespace gripperz::geometry; \
		using namespace gripperz::parametrization; \
	}
