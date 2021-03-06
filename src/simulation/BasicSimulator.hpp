/**
 * @file BasicSimulator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "GripperSimulator.hpp"
#include "GraspTaskSimulator.hpp"

namespace gripperz {
namespace simulation {

/**
 * @class BasicSimulator
 * @brief Uses basic RWSIM simulation to simulate tasks.
 * 
 * Assigns status to individual targets. Slippages are counted as successes.
 */
class BasicSimulator : virtual public GripperSimulator, protected GraspTaskSimulator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<BasicSimulator> Ptr;

public:
	BasicSimulator(rwsim::dynamics::DynamicWorkCell::Ptr dwc, unsigned threads=1);
	virtual ~BasicSimulator();
	
	/**
	 * @brief Load tasks to simulate.
	 */
	virtual void loadTasks(rwlibs::task::GraspTask::Ptr tasks);
	
	/**
	 * @brief Get tasks / results.
	 */
	virtual rwlibs::task::GraspTask::Ptr getTasks();
	
	/**
	 * @brief Starts the simulation.
	 */
	virtual void start(const rw::kinematics::State& initState);
	
	virtual void stop();
	
	virtual bool isRunning();
	
	unsigned getNrTasksDone() const;
	
	unsigned getThreads() const { return _threads; }
	
	/**
	 * @brief Returns the vector of thread simulations.
	 * Inherited from GraspTaskSimulator.
	 */
	std::vector<rwsim::simulator::ThreadSimulator::Ptr> getSimulators();

protected:
	const rw::kinematics::State& getInitState() const { return _initState; }
	
	//! Inherited from GraspTaskSimulator
	virtual void graspFinished(SimState& sstate);
	
	/**
	 * @brief Evaluates a grasp and changes the status if necessary.
	 * Changes ObjectSlipped to Success.
	 */
	virtual void evaluateGrasp(SimState& sstate);
	
	virtual void printGraspResult(SimState& sstate);
	
private:
	unsigned _threads;
	
	rw::kinematics::State _initState;
};

} /* simulation */
} /* gripperz */
