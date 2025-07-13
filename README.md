# 🚀 2025-智能物流搬运物流小车 | Smart Logistics Robot

> 开源硬件与软件系统解决方案，适用于智能仓储物流机器人开发  

## 📂 项目结构
  ├── 1-MaixCam_Embedded/ # 视觉处理子系统
  ├── 2-STM32_MainController/ # 主控核心逻辑
  ├── 3-Mechanical_Design/ # 机械结构模型
  ├── 4-PCB_Design/ # 电路设计文件
  ├── .gitattributes
  └── README.md # 本文档

## 🧩 核心模块详解

### 1. 视觉系统 (MaixCam标准版相机)
- **开发环境**：MaixVision IDE (Python) 
- **核心功能**：
  - 🎨 物料特征识别（颜色/圆心）
  - 💡 自适应环境补光系统
  - 🔄 多模式实时切换：
    - 物料颜色识别
    - 圆心定位检测
    - 垛码识别分析
- **学习资源**：
  [![MaixCam教程](https://img.shields.io/badge/Bilibili-MaixCam教程-00A1D6)](https://www.bilibili.com/video/BV12EeDeaEGp)

### 2. 主控制器 (STM32F103ZET6)
开发环境​​：Keil uVision + STM32标准库
  ​​- **核心功能**：
  - 🤖 全向底盘运动控制（麦克纳姆轮）
  - 🎯 升降抓取机构精确控制
  - 🔗 多设备通信协议：
  - **特别说明**：
   注意：！！！！！逻辑控制程序代码写的有点屎山，但是程序逻辑清晰 (T_T)
  
### 3. 机械结构 (UG NX 12.0)
  - ⚙️ 四轮全向移动平台
  - 📏 可扩展升降结构
  - 🧩 模块化末端执行器设计
- **特别说明**：
    结构命名规范待优化，建议查看装配约束关系

### 4. PCB设计 (嘉立创EDA)
  ​​- **​​核心设计**：
   ​​-🔌全接口扩展板设计：
      部分GPIO引脚引出
      外设专用接口
      
