/*
 * Software License Agreement (Apache License)
 *
 * Copyright (c) 2014, Dan Solomon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * joint_trajectory_pt.h
 *
 *  Created on: Oct 3, 2014
 *      Author: Dan Solomon
 */

#ifndef JOINT_TRAJECTORY_PT_H_
#define JOINT_TRAJECTORY_PT_H_

#include <vector>
#include "descartes_trajectory_planning/trajectory_pt.h"

namespace descartes
{

//TODO add warning if non-zero tolerances are specified because initial implementation will only allow fixed joints
struct JointTolerance
{
  JointTolerance(): lower(0.), upper(0.) {};
  JointTolerance(double low, double high):
    lower(std::abs(low)), upper(std::abs(high)) {};

  double lower, upper;
};

struct TolerancedJointValue
{
  TolerancedJointValue() {};
  TolerancedJointValue(double _nominal, double _tol_above, double _tol_below):
    nominal(_nominal), tolerance(_tol_above, _tol_below) {};
  TolerancedJointValue(double _nominal)
  {
    *this = TolerancedJointValue(_nominal, 0., 0.);
  }

  double upperBound() const
  {
    return nominal+tolerance.upper;
  }

  double lowerBound() const
  {
    return nominal-tolerance.lower;
  }

  double range() const
  {
    return upperBound() - lowerBound();
  }

  double nominal;
  JointTolerance tolerance;
};

/**@brief Joint Trajectory Point used to describe a joint goal for a robot trajectory.
 * The TOOL is something held by the robot. It is located relative to robot wrist/tool plate.
 * The WOBJ is something that exists in the world/global environment that is not held by robot.
 *
 * For a JointTrajectoryPt, the transform from wrist to tool, and base to workobject, are defined by fixed frames.
 * These transforms are important when calculating interpolation.
 * The joint position is specified as a nominal with upper/lower tolerances.
 */
class JointTrajectoryPt: public TrajectoryPt
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;      //TODO is this needed when Frame already has it?
public:
  JointTrajectoryPt();
  virtual ~JointTrajectoryPt() {};

  /**@name Getters for Cartesian pose(s)
   * @{
   */

  //TODO complete
  virtual bool getClosestCartPose(Eigen::Affine3d &pose, const moveit::core::RobotState &seed_state) const;

  //TODO complete
  virtual bool getNominalCartPose(Eigen::Affine3d &pose, const moveit::core::RobotState &seed_state) const;

  //TODO complete
  virtual void getCartesianPoses(EigenSTL::vector_Affine3d &poses, const moveit::core::RobotState &state) const;
  /** @} (end section) */

  /**@name Getters for joint pose(s)
   * @{
   */

  //TODO complete
  virtual bool getClosestJointPose(std::vector<double> &joint_pose, const moveit::core::RobotState &seed_state) const;

  //TODO complete
  virtual bool getNominalJointPose(std::vector<double> &joint_pose, const moveit::core::RobotState &seed_state) const;

  //TODO complete
  virtual void getJointPoses(std::vector<std::vector<double> > &joint_poses, const moveit::core::RobotState &state) const;
  /** @} (end section) */

  //TODO complete
  virtual bool isValid(const moveit::core::RobotState &state) const;

  //TODO complete
  /**@brief Set discretization. Each joint can have a different discretization.
   * @param discretization Vector of discretization values. If length=1, set all elements of discretization_ are set to value.
   * @return True if vector is length 1 or length(joint_position_) and value[ii] are within 0-range(joint_position[ii]).
   */
  virtual bool setDiscretization(const std::vector<double> &discretization);


protected:
  std::vector<TolerancedJointValue> joint_position_;  /**<@brief Fixed joint position with tolerance */
  std::vector<double>               discretization_;  /**<@brief How finely to discretize each joint */

  /** @name JointTrajectoryPt transforms. Used in get*CartPose() methods and for interpolation.
   *  @{
   */
  Frame                         tool_;                  /**<@brief Transform from robot wrist to active tool pt. */
  Frame                         wobj_;                  /**<@brief Transform from world to active workobject pt. */
  /** @} (end section) */

};

} /* namespace descartes */

#endif /* JOINT_TRAJECTORY_PT_H_ */
