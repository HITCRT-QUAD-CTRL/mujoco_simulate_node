# HITCRT RC26 四足机器人仿真

HITCRT RoboCon2026 赛季四足机器人仿真代码。基于 ROS 2 Humble + MuJoCo 3.1.1 构建,提供 500 Hz 物理仿真接口,用于机器人强化学习策略训练、调试与仿真部署验证。

## 功能

- **MuJoCo 物理仿真**: 500 Hz 实时步进,支持可视化调试(GLFW 渲染窗口)。
- **多机器人模型**: 内置 `a1`(Unitree A1)、`black`、`white` 三种模型,自带测试场地场景。
- **ROS 2 接口**: 标准 ROS 2 话题通信,方便与上层控制/状态估计算法对接。

## 目录结构

```text
src/
├── quad/          # 接口包:ROS 2 msg/srv 定义
├── simulate/      # 仿真节点(MuJoCo + ROS 2 桥接)
├── interface/     # 自定义消息定义
├── launch/        # launch 文件
├── config/        # 参数配置(simulate_cfg.yaml)
├── models/        # 机器人模型与测试场景
│   ├── a1/        # Unitree A1
│   ├── black/     # 自研机型 + 任务场景
│   ├── white/     # 自研机型 + 墙体场景
│   └── black_task/
└── utils/         # 共享头文件(状态机、参数、Eigen 别名等)
```

## 关节编号约定

12 个关节(`NUM_DOF=12`)按 `[腿, 关节]` 排列,顺序为髋(hip)→ 大腿(thigh)→ 小腿(calf):

```text
[FR_hip, FR_thigh, FR_calf,
 FL_hip, FL_thigh, FL_calf,
 RR_hip, RR_thigh, RR_calf,
 RL_hip, RL_thigh, RL_calf]
```

四条腿依次为 FR / FL / RR / RL。

## ROS 2 接口

| 方向 | 话题名 | 消息 | 频率 | 说明 |
| --- | --- | --- | --- | --- |
| 发布 | `quad/imu` | `sensor_msgs/Imu` | 500 Hz | IMU 数据 |
| 发布 | `quad/encoder` | `quad/Encoder` | 500 Hz | 关节编码器反馈 |
| 发布 | `quad/test_estm_state` | `quad/State` | 500 Hz | 真值状态,仅 cheat mode |
| 订阅 | `quad/joint_torques` | `quad/Torque` | — | 关节力矩指令 |
| 订阅 | `quad/wbc_torques` | `quad/Torque` | — | WBC 附加力矩 |

实际下发到 MuJoCo 的 `ctrl[i] = joint_torques[i] + wbc_torques[i]`。

## 状态机

外部包 `/commands/state_machine` 定义状态机与按键映射,枚举在 `/controllers/common/FSM.h` 的 `State_e`。本仓库内置枚举见 [`src/utils/fsm_defs.h`](utils/fsm_defs.h):

- `WAIT_DATA` — 等待数据
- `STAND` — 站立
- `WALK` — 行走
- `FALL` — 倒地保护

## 快速开始

### 依赖

- ROS 2 Humble
- MuJoCo 3.1.1(预编译 `.so` 已包含在 `src/simulate/lib/`)
- GLFW3(系统库)
- C++20 编译器

### 编译

```bash
colcon build
source install/setup.bash
```

单独编译:

```bash
colcon build --packages-select quad
colcon build --packages-select simulate
```

### 启动仿真

```bash
ros2 launch quad simulate.launch.py
```

### 选择机器人模型

修改 [`src/config/simulate_cfg.yaml`](config/simulate_cfg.yaml) 中的 `model_name`,可选值: `"a1"` / `"black"` / `"white"`。

### 关键参数

`src/config/simulate_cfg.yaml`:

| 参数 | 说明 |
| --- | --- |
| `model_name` | 机器人模型 |
| `scene_name` | 场景名(默认 `scene`) |
| `is_use_cheat_mode` | 是否发布真值状态 |
| `is_gpu_accelerated` | 是否启用 GPU 加速渲染 |

