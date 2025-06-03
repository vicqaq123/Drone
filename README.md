# Drone A* Path Finding with Obstacle Avoidance

## 版本说明
这是 A* 避障失败版本的代码，主要问题：
1. 路径规划时可能穿过障碍物
2. 障碍物检测和膨胀处理存在精度问题

## 主要组件
- `AStarPathFinderComponent`: A* 路径规划组件
- `GridMapComponent`: 网格地图组件
- `ObstacleScannerComponent`: 障碍物扫描组件
- `PathModifierComponent`: 路径修改组件

## 文件结构
```
Source/Drone/
├── AStarPathFinderComponent.cpp/h
├── GridMapComponent.cpp/h
├── ObstacleScannerComponent.cpp/h
├── PathModifierComponent.cpp/h
└── Drone.cpp/h
```

## 已知问题
1. 路径点可能穿过障碍物
2. 障碍物膨胀处理不够精确
3. 浮点数精度问题导致路径点不准确 