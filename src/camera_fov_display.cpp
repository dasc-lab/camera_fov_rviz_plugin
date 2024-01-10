#include "camera_fov_rviz_plugin/camera_fov_display.h"
#include "camera_fov_rviz_plugin/camera_fov_visual.h"

#include <memory>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include "rclcpp/rclcpp.hpp"
#include "rviz_common/properties/color_property.hpp"
#include "rviz_common/properties/float_property.hpp"
#include "rviz_common/properties/int_property.hpp"
#include "rviz_common/visualization_manager.hpp"
#include "tf2_ros/transform_listener.h"

namespace camera_fov_rviz_plugin {

CameraFovDisplay::CameraFovDisplay() {

  color_property_ = new rviz_common::properties::ColorProperty(
      "Color", QColor(204, 51, 204), "Color to draw the fov.", this,
      SLOT(updateColorAndAlpha()));

  alpha_property_ = new rviz_common::properties::FloatProperty(
      "Alpha", 1.0, "0 is fully transparent, 1.0 is fully opaque.", this,
      SLOT(updateColorAndAlpha()));

  horizontal_fov_property_ = new rviz_common::properties::FloatProperty(
      "Horizontal FOV (degrees)", 90.0, "Horizontal field of view in degrees.",
      this, SLOT(updateHorizontalFov()));
  horizontal_fov_property_->setMin(0.0);
  horizontal_fov_property_->setMax(360.0);

  history_length_property_ = new rviz_common::properties::IntProperty(
      "History Length", 1, "Number of prior measurements to display.", this,
      SLOT(updateHistoryLength()));
  history_length_property_->setMin(1);
  history_length_property_->setMax(100000);
}

// After the top-level rviz_common::Display::initialize() does its own setup,
// it calls the subclass's onInitialize() function.  This is where we
// instantiate all the workings of the class.  We make sure to also
// call our immediate super-class's onInitialize() function, since it
// does important stuff setting up the message filter.
//
// Note that "MFDClass" is a typedef of
// ``MessageFilterDisplay<message type>``, to save typing that long
// templated class name every time you need to refer to the
// superclass.
void CameraFovDisplay::onInitialize() {
  MFDClass::onInitialize();
  updateHistoryLength();
}

CameraFovDisplay::~CameraFovDisplay() {}

// Clear the visuals by deleting their objects.
void CameraFovDisplay::reset() {
  MFDClass::reset();
  visuals_.clear();
}

// Set the current color and alpha values for each visual.
void CameraFovDisplay::updateColorAndAlpha() {
  float alpha = alpha_property_->getFloat();
  Ogre::ColourValue color = color_property_->getOgreColor();

  for (size_t i = 0; i < visuals_.size(); i++) {
    visuals_[i]->setColor(color.r, color.g, color.b, alpha);
  }
}

// Set the number of past visuals to show.
void CameraFovDisplay::updateHistoryLength() {
  history_length_ =
      static_cast<std::size_t>(history_length_property_->getInt());
  if (visuals_.size() > history_length_) {
    visuals_.resize(history_length_);
  }
}

// This is our callback to handle an incoming message.
void CameraFovDisplay::processMessage(
    sensor_msgs::msg::CameraInfo::ConstSharedPtr msg) {
  // Here we call the rviz_common::FrameManager to get the transform from the
  // fixed frame to the frame in the header of this CameraInfo message.  If
  // it fails, we can't do anything else so we return.
  Ogre::Quaternion orientation;
  Ogre::Vector3 position;
  if (!context_->getFrameManager()->getTransform(
          msg->header.frame_id, msg->header.stamp, position, orientation)) {
    RCLCPP_INFO(rclcpp::get_logger("imu_display"),
                "Error transforming from frame '%s' to frame '%s'",
                msg->header.frame_id.c_str(), qPrintable(fixed_frame_));
    return;
  }

  // Set the contents of the visual.
  std::shared_ptr<CameraFovVisual> visual;
  visual.reset(new CameraFovVisual(context_->getSceneManager(), scene_node_));
  visual->setMessage(msg);
  visual->setFramePosition(position);
  visual->setFrameOrientation(orientation);

  float alpha = alpha_property_->getFloat();
  Ogre::ColourValue color = color_property_->getOgreColor();
  visual->setColor(color.r, color.g, color.b, alpha);

  // We are keeping a deque of visual pointers.
  // This removes the oldest visual from the back if the capacity has been
  // reached, and adds our new visual to the front.
  if (visuals_.size() == history_length_) {
    visuals_.pop_back();
  }
  visuals_.push_front(visual);
}

} // namespace camera_fov_rviz_plugin

// Tell pluginlib about this class.  It is important to do this in
// global scope, outside our package's namespace.
#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(camera_fov_rviz_plugin::CameraFovDisplay,
                       rviz_common::Display)
