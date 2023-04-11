#include "pickupobjecttask.hpp"

PickupObjectTask::PickupObjectTask(ObjectType objectType)
    : Task(TaskType::PICKUPOBJECT, PICKUPOBJECTTASK_PRIORITY)
{
    
}

void PickupObjectTask::notStarted(std::shared_ptr<Map> map, 
                        std::shared_ptr<Navigator> navigator, 
                        RobotState& nextRobotState)
{
    
}

void PickupObjectTask::inProgress(std::shared_ptr<Map> map, 
                        std::shared_ptr<Navigator> navigator, 
                        RobotState& nextRobotState)
{
    
}

void PickupObjectTask::suspended(std::shared_ptr<Map> map, 
                        std::shared_ptr<Navigator> navigator, 
                        RobotState& nextRobotState, TaskType& nextTaskType)
{
    
}

void PickupObjectTask::complete(std::shared_ptr<Map> map, 
                        std::shared_ptr<Navigator> navigator, 
                        RobotState& nextRobotState, TaskType& nextTaskType)
{
    
}