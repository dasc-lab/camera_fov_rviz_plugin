
#include <Ogre.h>

#include "camera_fov_rviz_plugin/camera_fov_visual.h"
// #include "rviz_rendering/objects/arrow.hpp"
#include "rviz_rendering/objects/line.hpp"

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
  // for (int i=0; i<4; i++){
  //   arrows_[i].reset(new rviz_rendering::Arrow(scene_manager_, frame_node_));
  // }

  // create all the lines
  for (auto &l : lines_) {
    l.reset(new rviz_rendering::Line(scene_manager_, frame_node_));
  }
}

CameraFovVisual::~CameraFovVisual() {
  // Destroy the frame node since we don't need it anymore.
  scene_manager_->destroySceneNode(frame_node_);
}

void CameraFovVisual::setMessage(
    sensor_msgs::msg::CameraInfo::ConstSharedPtr msg) {

  // assumes no distortion when plotting the FOV
  // assumes binning_x = binning_y = 1!

  // K = [fx  0  cx]
  //     [0  fy  cy]
  //     [0   0   1]

  double fx = msg->k[0];
  double fy = msg->k[4];
  // double cx = msg->k[2];
  // double cy = msg->k[5];

  // Calculuate the field of view
  // https://stackoverflow.com/questions/39992968/how-to-calculate-field-of-view-of-the-camera-from-camera-intrinsic-matrix
  double fov_h = 2 * std::atan2(msg->width, 2 * fx);
  double fov_v = 2 * std::atan2(msg->height, 2 * fy);

  // convert it to a set of vectors

  // Create Ogre::Vector3
  Ogre::Vector3 acc(1.0, 0.0, 0.0);

  double L = 4.0;

  // create origin
  Ogre::Vector3 o(0, 0, 0);

  // Mark out the four corners of the FOV
  Ogre::Vector3 v0(L * std::sin(fov_h / 2), L * std::sin(fov_v / 2), L);
  Ogre::Vector3 v1(-L * std::sin(fov_h / 2), L * std::sin(fov_v / 2), L);
  Ogre::Vector3 v2(-L * std::sin(fov_h / 2), -L * std::sin(fov_v / 2), L);
  Ogre::Vector3 v3(L * std::sin(fov_h / 2), -L * std::sin(fov_v / 2), L);

  // Scale the arrow's thickness in each dimension along with its length.
  Ogre::Vector3 scale(1, 1, 1);
  // for (int i=0; i<4; i++) {
  //         arrows_[i]->setScale(scale);
  // }

  // Set the orientation of the arrow
  // arrows_[0]->setDirection(v0);
  // arrows_[1]->setDirection(v1);
  // arrows_[2]->setDirection(v2);
  // arrows_[3]->setDirection(v3);

  // // now create the mesh
  // mesh_->estimateVertexCount(12); // only do one face for now
  // mesh_-> beginTriangles();

  // // sides
  // mesh_->addVertex(o, v0, v1); // top
  // mesh_->addVertex(o, v1, v3); // left
  // mesh_->addVertex(o, v3, v2); // bottom
  // mesh_->addVertex(o, v2, v0); // right

  // // end
  // mesh_-> endTriangles();

  // add the lines
  lines_[0]->setPoints(o, v0);
  lines_[1]->setPoints(o, v1);
  lines_[2]->setPoints(o, v2);
  lines_[3]->setPoints(o, v3);

  lines_[4]->setPoints(v0, v1);
  lines_[5]->setPoints(v1, v2);
  lines_[6]->setPoints(v2, v3);
  lines_[7]->setPoints(v3, v0);

  for (auto &l : lines_) {
    l->setScale(scale);
  }
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
  // for (int i=0; i<4; i++){
  // arrows_[i]->setColor(r, g, b, a);
  // }

  for (auto &l : lines_) {
    l->setColor(r, g, b, a);
  }
}

} // namespace camera_fov_rviz_plugin
