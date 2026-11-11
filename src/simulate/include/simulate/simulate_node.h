#pragma once
#include <chrono>

#include <functional>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <quad/msg/detail/state__struct.hpp>
#include <quad/msg/encoder.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>

#include <quad/msg/state.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>

#include "quad/msg/encoder.hpp"
#include "mujoco/mujoco.h"
#include "quad/msg/torque.hpp"
#include "quad/msg/state.h"
using namespace std::chrono_literals;
class SimulateNode : public rclcpp::Node {
 public:
  SimulateNode() : Node("sim", "quad") {
    imu_publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("imu", 1);
    encoder_publisher_ =
        this->create_publisher<quad::msg::Encoder>("encoder", 1);
    torque_msg_.joint_torques.fill(0);
    state_publisher_ = 
    this->create_publisher<quad::msg::State>("test_estm_state", 1);
    auto imu_timer_callback = [this] {
      if (flag_data_init_) {
        RCLCPP_INFO_ONCE(this->get_logger(), "imu publishing");
        imu_publisher_->publish(imu_msg_);
      }
    };
    auto state_timer_callback = [this] {
      if (flag_data_init_) {
        RCLCPP_INFO_ONCE(this->get_logger(), "state publishing");
        state_publisher_->publish(state_msg_);
      }
    };

    auto encoder_timer_callback = [this] {
      if (flag_data_init_) {
        RCLCPP_INFO_ONCE(this->get_logger(), "encoder publishing");
        encoder_publisher_->publish(encoder_msg_);
      }
    };

    auto torque_sub_callback = [this](const quad::msg::Torque torque_msg) {
      std::lock_guard<std::mutex> lk(set_torque_mutex);
      torque_msg_ = torque_msg;
    };

    auto wbc_torque_sub_callback = [this](const quad::msg::Torque torque_msg) {
      std::lock_guard<std::mutex> lk(set_torque_mutex);
      wbc_torque_msg_ = torque_msg;
    };

    update_timer_ = this->create_wall_timer(
        2ms, std::bind(&SimulateNode::update_data, this));
 
    state_timer_ = this->create_wall_timer(2ms, state_timer_callback);
    imu_timer_ = this->create_wall_timer(2ms, imu_timer_callback);
    encoder_timer_ = this->create_wall_timer(2ms, encoder_timer_callback);
    

    torque_subscribtion_ = this->create_subscription<quad::msg::Torque>(
        "joint_torques", 10, torque_sub_callback);
    wbc_torque_subscribtion_ = this->create_subscription<quad::msg::Torque>(
        "wbc_torques", 10, wbc_torque_sub_callback);
  }

  void set_mutex(std::recursive_mutex* mutex) {
    mutex_ = mutex;
    return;
  }

  void set_model(mjData* d) {
    d_ = d;
    return;
  }

  void set_ctrl_ptr(double* global_ctrl_ptr) {
    global_ctrl_ptr_ = global_ctrl_ptr;
    return;
  }

  bool is_ready() { return d_ && mutex_; }
  std::mutex set_torque_mutex;
  quad::msg::Torque torque_msg_;
  quad::msg::Torque wbc_torque_msg_;
  bool is_use_cheat_mode;
    rclcpp::TimerBase::SharedPtr update_timer_;
  rclcpp::TimerBase::SharedPtr imu_timer_;
  rclcpp::TimerBase::SharedPtr encoder_timer_;
    rclcpp::TimerBase::SharedPtr state_timer_;

 private:
  mjData* d_ = nullptr;
  double* global_ctrl_ptr_ = nullptr;
  std::recursive_mutex* mutex_ = nullptr;
  std::atomic_bool flag_toque_update;


  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher_;
  rclcpp::Publisher<quad::msg::Encoder>::SharedPtr encoder_publisher_;
  rclcpp::Publisher<quad::msg::State>::SharedPtr state_publisher_;

  rclcpp::Subscription<quad::msg::Torque>::SharedPtr torque_subscribtion_;
  rclcpp::Subscription<quad::msg::Torque>::SharedPtr wbc_torque_subscribtion_;
  sensor_msgs::msg::Imu imu_msg_;
  quad::msg::Encoder encoder_msg_;
  quad::msg::State state_msg_;
  bool flag_data_init_ = false;
  void update_data();
};
