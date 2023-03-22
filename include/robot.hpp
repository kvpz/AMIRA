#ifndef ROBOT_HPP
#define ROBOT_HPP
#include <memory>
#include "waypoint.hpp"
#include "taskmanager.hpp"
#include "comms.hpp"
#include "enums/robotPoseToWaypoint.hpp"
#include "enums/robotState.hpp"
#include "enums/robotOrientationAtEndpoint.hpp"
#include "map.hpp"
#include "navigator.hpp"

#define ROBOTDEBUG true

class Robot {
public:
    Robot(double xpos, double ypos, double orientation) 
        : state(STOP), robotPoseToWaypoint(NONE)
      //: map(std::make_unique<Map>()), navigator(std::make_unique<Navigator>()), taskManager(std::make_unique<TaskManager>())
    { 
        comport = std::make_unique<Comms>("/dev/ttyACM0");
        taskManager = std::make_unique<TaskManager>();
        navigator = std::make_unique<Navigator>();
        map = std::make_unique<Map>();
        map->setRobotCurrentCoordinate(xpos, ypos);
        map->setRobotOrientation(orientation);
    }

    //void run();
    void run()
    {
        switch (state) {
          case MOVE_FORWARD:
            move_forward(); 
            break;
          case MOVE_LEFT:
            move_left();
            break;
          case MOVE_RIGHT:
              move_right();
            break;
          case ROTATE_CW:
            rotate_CW(); 
            break;
          case ROTATE_CCW:
            rotate_CCW(); 
            break;
          case MOVE_BACKWARD:
            move_backward();
            break;
          case STOP:
            stop();
            break;
        }
    }

    void move_forward() { comport->send_command("F"); }
    void move_backward() { comport->send_command("B"); }
    void move_left() { comport->send_command("L"); }
    void move_right() { comport->send_command("R"); }
    void rotate_CW() { comport->send_command("C"); }
    void rotate_CCW() { comport->send_command("Z"); }
    void stop() { comport->send_command("S"); }

    void printStatus() 
    {
        std::cout << "\n====== Robot Status ======\n";
        std::cout << "State: " << RobotStateToString(state) << "\n";
        std::cout << "current location: ("
                  << map->getNextDestinationXY().getX() << ", "
                  << map->getNextDestinationXY().getY() << ")\n";
        std::cout << "current orientation (yaw): " << map->getRobotOrientation() << "\n";
        std::cout << "robot pose relative to waypoint: " << printRobotPoseToWaypoint(robotPoseToWaypoint) << "\n";
        std::cout << "==========================\n";
        std::cout << std::endl;
    }

    // getters (inlined)
    inline double getX() const { return map->getNextDestinationXY().getX(); }
    inline double getY() const { return map->getNextDestinationXY().getY(); }
    inline RobotState getState() const { return state; }
    inline double getOrientation() const { return map->getRobotOrientation(); }
    inline double getAngleToDestination() const { return navigator->getAngleToDestination(); }
    inline std::unique_ptr<TaskManager> getTaskManager() { return std::move(taskManager); }
    inline bool hasTasks() { return taskManager->hasTasks(); }
    // map related functions
    inline bool isNearEndpoint() const { return nearEndpoint; }

    //inline std::unique_ptr<Map> getMap() { return std::move(map); }
    inline std::unique_ptr<Navigator> getNavigator() { return std::move(navigator); }

    // setters (inlined)
    void setTravelDirection(RobotState travDir) { state = travDir; } //travelDirection = travDir; }

    void setCurrentXY(double x, double y) 
    {
      map->setRobotCurrentCoordinate(x,y);
    }

    void setOrientation(double o)
    {
      map->setRobotOrientation(o);
    }
    
    void setIsNearEndpoint(bool b) { nearEndpoint = b; }

    /*
      Robot sends data about itself to the task manager. 
      The task manager then executes the current task. 
      A task will update the state of the robot.
    */
    void executeCurrentTask()
    {
      RobotState nextRobotState;
      //nextRobotState = taskManager->executeCurrentTask(std::move(map), std::move(navigator));
      taskManager->executeCurrentTask(std::move(map), std::move(navigator), nextRobotState);

      // change robot state if it is different from current state
      if(state != nextRobotState) {
          state = nextRobotState;
          run();
      }
    }

private:
    RobotState state;

    std::unique_ptr<Comms> comport;
    RobotPoseToWaypoint robotPoseToWaypoint = NONE;
    RobotOrientationAtEndpoint robotOrientationAtEndpoint = NOTORIENTED;
    bool nearEndpoint = false;

    //Waypoint currentLocation;
    //double currentOrientation; // (gyro) orientation
    //RobotState travelDirection;

    std::unique_ptr<TaskManager> taskManager;
    std::unique_ptr<Navigator> navigator;
    std::unique_ptr<Map> map;


};


#endif
