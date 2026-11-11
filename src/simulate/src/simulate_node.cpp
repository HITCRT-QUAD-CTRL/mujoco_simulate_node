#include "simulate/simulate_node.h"

#include <Eigen/Dense>
#include <chrono>
/*
in global frame:
framepos
framequat
framelinvel
in local frame:
imu
 q
 ang_vel
 acc
*/

void SimulateNode::update_data() {
  RCLCPP_INFO_ONCE(this->get_logger(), "updating sim data");
  double *sensordata = nullptr;
  {
    std::lock_guard lg(*mutex_);
    sensordata = d_->sensordata;
  }
  // --- get rpy ---
  int quat_sensor_adr = 3;
  auto quat_msg = geometry_msgs::msg::Quaternion();
  quat_msg.set__w(*(sensordata + quat_sensor_adr + 0));
  quat_msg.set__x(*(sensordata + quat_sensor_adr + 1));
  quat_msg.set__y(*(sensordata + quat_sensor_adr + 2));
  quat_msg.set__z(*(sensordata + quat_sensor_adr + 3));

  int angvel_sensor_adr = 10;
  auto angvel_msg = geometry_msgs::msg::Vector3();
  angvel_msg.set__x(*(sensordata + angvel_sensor_adr + 0));
  angvel_msg.set__y(*(sensordata + angvel_sensor_adr + 1));
  angvel_msg.set__z(*(sensordata + angvel_sensor_adr + 2));

  int linacc_sensor_adr = 13;
  auto linacc_msg = geometry_msgs::msg::Vector3();
  linacc_msg.set__x(*(sensordata + linacc_sensor_adr + 0));
  linacc_msg.set__y(*(sensordata + linacc_sensor_adr + 1));
  linacc_msg.set__z(*(sensordata + linacc_sensor_adr + 2));

  auto imu_msg = sensor_msgs::msg::Imu();
  imu_msg.header.stamp = this->get_clock()->now();
  imu_msg.set__orientation(quat_msg);
  imu_msg.set__angular_velocity(angvel_msg);
  imu_msg.set__linear_acceleration(linacc_msg);

  imu_msg_ = imu_msg;

  int qpos_sensor_adr = 16;
  int qvel_sensor_adr = 28;
  auto encoder_msg = quad::msg::Encoder();
  for (int i = 0; i < 12; ++i) {
    encoder_msg.qpos[i] = *(sensordata + qpos_sensor_adr + i);

    encoder_msg.qvel[i] = *(sensordata + qvel_sensor_adr + i);
  }
  if(is_use_cheat_mode)
  {
  for (int i = 0; i < 12; ++i) {
    state_msg_.encoder.qpos[i] = *(sensordata + qpos_sensor_adr + i);

    state_msg_.encoder.qvel[i] = *(sensordata + qvel_sensor_adr + i);
  }
    auto timePoint = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    long long millis = timePoint.time_since_epoch().count();
    state_msg_.time_stamp = (double)millis;
   // std::cout<<"this->get_clock()->now()"<<this->get_clock()->now().seconds()<<std::endl;
      int world_footpos_sensor_adr = 40;
      int world_footvel_sensor_adr = 40+12;
    for (int i = 0; i < 12; ++i) {
      state_msg_.foot_pos_world[i] = *(sensordata + world_footpos_sensor_adr + i);
      state_msg_.foot_vel_world[i] = *(sensordata + world_footvel_sensor_adr + i);

    int pos_sensor_adr = 0;
    int linvel_sensor_adr = 7;
    for(int i=0;i<3;i++)
    {
    state_msg_.pos_world[i] = *(sensordata + pos_sensor_adr + i);
    state_msg_.lin_vel_world[i] = *(sensordata + linvel_sensor_adr + i);
    }
      

      state_msg_.imu_world.set__w(*(sensordata + quat_sensor_adr + 0));
      state_msg_.imu_world.set__x(*(sensordata + quat_sensor_adr + 1));
      state_msg_.imu_world.set__y(*(sensordata + quat_sensor_adr + 2));
      state_msg_.imu_world.set__z(*(sensordata + quat_sensor_adr + 3));

      state_msg_.imu_world.ang_vel[0] =   (*(sensordata + angvel_sensor_adr + 0));
      state_msg_.imu_world.ang_vel[1] = (*(sensordata + angvel_sensor_adr + 1));
      state_msg_.imu_world.ang_vel[2] = (*(sensordata + angvel_sensor_adr + 2));

// state_msg_.
    }

  }
  encoder_msg_ = encoder_msg;

  flag_data_init_ = true;

  return;
}
