
#include <Ogre.h>

#include "camera_fov_rviz_plugin/camera_fov_visual.h"
#include "rviz_rendering/objects/arrow.hpp"

namespace camera_fov_rviz_plugin {

CameraFovVisual::CameraFovVisual(Ogre::SceneManager *scene_manager,
                                 Ogre::SceneNode *parent_node) {
  scene_manager_ = scene_manager;

  // Ogre::SceneNodes form a tree, with each node storing the
  // transform (position and orientation) of itself relative to its
  // parent.  Ogre does the math of combining those transforms when it
  // is time to render.
  //
  // Here we create a node to store the pose of the CameraInfo's header frame
  // relative to the RViz fixed frame.
  frame_node_ = parent_node->createChildSceneNode();

  // We create the arrow object within the frame node so that we can
  // set its position and direction relative to its header frame.
  arrow_.reset(new rviz_rendering::Arrow(scene_manager_, frame_node_));
}

CameraFovVisual::~CameraFovVisual() {
  // Destroy the frame node since we don't need it anymore.
  scene_manager_->destroySceneNode(frame_node_);
}

void CameraFovVisual::setMessage(
    sensor_msgs::msg::CameraInfo::ConstSharedPtr msg) {

  // Create Ogre::Vector3
  Ogre::Vector3 acc(1.0, 0.0, 0.0);

  // Find the magnitude of the acceleration vector.
  float length = 1.0;

  // Scale the arrow's thickness in each dimension along with its length.
  Ogre::Vector3 scale(length, length, length);
  arrow_->setScale(scale);

  // Set the orientation of the arrow to match the direction of the
  // acceleration vector.
  arrow_->setDirection(acc);
}

// Position and orientation are passed through to the SceneNode.
void CameraFovVisual::setFramePosition(const Ogre::Vector3 &position) {
  frame_node_->setPosition(position);
}

void CameraFovVisual::setFrameOrientation(const Ogre::Quaternion &orientation) {
  frame_node_->setOrientation(orientation);
}

// Color is passed through to the Arrow object.
void CameraFovVisual::setColor(float r, float g, float b, float a) {
  arrow_->setColor(r, g, b, a);
}

} // namespace camera_fov_rviz_plugin
