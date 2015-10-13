/**
 * @file evaluation.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "evaluation/Types.hpp"
#include "evaluation/GripperEvaluator.hpp"
#include "evaluation/ConstrainedGripperEvaluator.hpp"
#include "evaluation/GripperObjectiveFunction.hpp"
#include "evaluation/GripperEvaluationManager.hpp"
#include "evaluation/StandardEvaluationManager.hpp"
#include "evaluation/RobustEvaluationManager.hpp"
#include "evaluation/GripperEvaluationManagerFactory.hpp"
#include "evaluation/GripperEvaluationManagerBuilder.hpp"
#include "evaluation/AlignmentCalculator.hpp"
#include "evaluation/StablePoseAlignment.hpp"
#include "evaluation/AllPosesAlignment.hpp"
#include "evaluation/VersorAlignment.hpp"
#include "evaluation/SimpleAlignment.hpp"
#include "evaluation/ClusteringAlignment.hpp"
#include "evaluation/GripperQualityExtractor.hpp"
#include "evaluation/QualityIndexCalculator.hpp"

