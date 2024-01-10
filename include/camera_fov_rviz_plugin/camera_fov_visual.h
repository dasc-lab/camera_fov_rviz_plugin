#ifndef CAMERA_FOV_VISUAL_HPP_
#define CAMERA_FOV_VISUAL_HPP_

#include <memory>

#include <Ogre.h>

#include "rviz_rendering/objects/arrow.hpp"
#include "sensor_msgs/msg/camera_info.hpp"

namespace Ogre {
class Quaternion;
}

namespace rviz_rendering {
class Arrow;
}

namespace camera_fov_rviz_plugin {

// Declare the visual class for this display.
//
// Each instance of CameraFovVisual represents the visualization of a single
// sensor_msgs::msg::CameraInfo message.
class CameraFovVisual {
public:
  // Constructor.  Creates the visual stuff and puts it into the
  // scene, but in an unconfigured state.
  CameraFovVisual(Ogre::SceneManager *scene_manager,
                  Ogre::SceneNode *parent_node);

  // Destructor.  Removes the visual stuff from the scene.
  virtual ~CameraFovVisual();

  // Configure the visual to show the data in the message.
  void setMessage(sensor_msgs::msg::CameraInfo::ConstSharedPtr msg);

  // Set the pose of the coordinate frame the message refers to.
  // These could be done inside setMessage(), but that would require
  // calls to FrameManager and error handling inside setMessage(),
  // which doesn't seem as clean.  This way CameraFovVisual is only
  // responsible for visualization.
  void setFramePosition(const Ogre::Vector3 &position);
  void setFrameOrientation(const Ogre::Quaternion &orientation);

  // Set the color and alpha of the visual, which are user-editable
  // parameters and therefore don't come from the Imu message.
  void setColor(float r, float g, float b, float a);

private:
  // The object implementing the actual arrow shape.
  std::shared_ptr<rviz_rendering::Arrow> arrow_;

  // A SceneNode whose pose is set to match the coordinate frame of
  // the CameraInfo message header.
  Ogre::SceneNode *frame_node_;

  // The SceneManager, kept here only so the destructor can ask it to
  // destroy the ``frame_node_``.
  Ogre::SceneManager *scene_manager_;
};

} // end namespace camera_fov_rviz_plugin

#endif // CAMERA_Fov_VISUAL_HPP_
