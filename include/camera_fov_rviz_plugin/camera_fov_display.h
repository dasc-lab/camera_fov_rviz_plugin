#ifndef CAMERA_FOV_DISPLAY_HPP_
#define CAMERA_FOV_DISPLAY_HPP_

#ifndef Q_MOC_RUN
#include <deque>
#include <memory>

#include "rviz_common/message_filter_display.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#endif

namespace Ogre {
class SceneNode;
}

namespace rviz_common::properties {
class ColorProperty;
class FloatProperty;
class IntProperty;
} // namespace rviz_common::properties

// All the source in this plugin is in its own namespace.  This is not
// required but is good practice.
namespace camera_fov_rviz_plugin {

class CameraFovVisual;

// Here we declare our new subclass of rviz_common::Display.  Every display
// which can be listed in the "Displays" panel is a subclass of
// rviz_common::Display.
//
//
// The CameraFovDisplay class itself just implements the deque,
// editable parameters, and Display subclass machinery.  The visuals
// themselves are represented by a separate class, CameraFovVisual.  The
// idiom for the visuals is that when the objects exist, they appear
// in the scene, and when they are deleted, they disappear.
class CameraFovDisplay
    : public rviz_common::MessageFilterDisplay<sensor_msgs::msg::CameraInfo> {
  Q_OBJECT

public:
  // Constructor.  pluginlib::ClassLoader creates instances by calling
  // the default constructor, so make sure you have one.
  CameraFovDisplay();
  virtual ~CameraFovDisplay();

  // Overrides of protected virtual functions from Display.  As much
  // as possible, when Displays are not enabled, they should not be
  // subscribed to incoming data and should not show anything in the
  // 3D view.  These functions are where these connections are made
  // and broken.

protected:
  virtual void onInitialize();

  // A helper to clear this display back to the initial state.
  virtual void reset();

  // These Qt slots get connected to signals indicating changes in the
  // user-editable properties.

private Q_SLOTS:
  void updateColorAndAlpha();
  void updateHistoryLength();

private:
  // Function to handle an incoming ROS message.
  void processMessage(sensor_msgs::msg::CameraInfo::ConstSharedPtr msg);

  // Storage for the list of visuals. It is a deque where
  // data gets popped from the back (oldest) and pushed to the front (newest).
  std::deque<std::shared_ptr<CameraFovVisual>> visuals_;
  std::size_t history_length_{1};

  // User-editable property variables.
  rviz_common::properties::ColorProperty *color_property_;
  rviz_common::properties::FloatProperty *alpha_property_;
  rviz_common::properties::FloatProperty *horizontal_fov_property_;
  rviz_common::properties::IntProperty *history_length_property_;
};

} // end namespace camera_fov_rviz_plugin

#endif // CAMERA_FOV_DISPLAY_HPP_
