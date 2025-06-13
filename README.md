# UE5 开发环境配置指南

## 1. 安装 Epic Games Launcher
1. 访问 [Epic Games 官网](https://www.epicgames.com/store/zh-CN/)
2. 下载并安装 Epic Games Launcher
3. 创建/登录 Epic 账号

## 2. 安装虚幻引擎 5.4
1. 打开 Epic Games Launcher
2. 切换到"虚幻引擎"标签页
3. 点击"安装引擎"
4. 选择版本 5.4
5. 在安装选项中确保选中：
   - 游戏开发
   - 桌面开发
   - 必要的组件

## 3. 安装 Visual Studio
1. 访问 [Visual Studio 官网](https://visualstudio.microsoft.com/zh-hans/)
2. 下载 Visual Studio 2022 Community
3. 在安装程序中选择以下组件：
   - 使用 C++ 的桌面开发
   - 游戏开发
   - Windows 10/11 SDK
   - C++ CMake 工具
   - MSVC v143 构建工具
   - Windows 通用 C 运行时

## 4. 安装 JetBrains Rider
1. 访问 [JetBrains Rider 官网](https://www.jetbrains.com/rider/)
2. 下载 Rider 2024.1.1 版本
3. 安装步骤：
   - 解压下载的 zip 文件
   - 进入 scripts 文件夹
   - 运行 install-all-users.bat
   - 等待安装完成
4. 激活步骤：
   - 打开 Rider
   - 使用科学上网访问 jetbra.in
   - 复制激活码
   - 在 Rider 中完成激活

## 5. UE5 编译器设置
1. 打开 Epic Games Launcher
2. 进入虚幻引擎设置
3. 在"源代码"选项卡中：
   - 设置 Visual Studio 为默认 IDE
   - 确保已安装 Windows SDK
   - 验证 C++ 开发工具已正确安装

## 环境验证
完成以上步骤后，建议：
1. 创建一个新的 C++ 项目
2. 验证项目能否正常编译
3. 测试 Rider 的代码补全和调试功能

## 注意事项
- 确保所有组件版本兼容
- 保持足够的磁盘空间（建议至少 100GB）
- 定期更新各个组件
- 建议使用 SSD 存储项目文件

## 常见问题解决
1. 如果遇到编译错误：
   - 检查 Visual Studio 组件是否完整安装
   - 验证 Windows SDK 版本
   - 确保项目路径不包含中文字符

2. 如果 Rider 无法识别 UE5 项目：
   - 重新生成项目文件
   - 检查 .uproject 文件配置
   - 验证 Rider 插件是否正确安装

## 推荐配置
- 操作系统：Windows 10/11 64位
- 处理器：Intel i7/i9 或 AMD Ryzen 7/9
- 内存：32GB 或更高
- 显卡：NVIDIA RTX 3060 或更高
- 存储：1TB SSD 