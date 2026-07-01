# Trust All Skins

一个 LeviLamina 模组，让服务器信任所有玩家的皮肤。

## 功能

安装此模组后，服务器会将所有玩家的皮肤标记为"受信任"，这意味着：

- 其他玩家**无需**关闭游戏设置中的"仅允许受信任的皮肤"选项
- 所有玩家都可以看到其他玩家的自定义皮肤
- 包括自定义几何体（4D皮肤）和自定义披风

## 工作原理

此模组 Hook `ServerNetworkHandler::_decideIfSkinIsTrusted` 与 `PlayerListEntry::write`，在 26.10 上将皮肤数据中的 `mIsTrustedSkin` 设为 `TrustedSkinFlag::True`。客户端开启「仅允许受信任的皮肤」时仍可显示自定义皮肤。

## 安装

### 使用 lip 安装（推荐）

```bash
lip install github.com/YUMU1658/trust-all-skins
```

### 手动安装

1. 从 [Releases](https://github.com/YUMU1658/trust-all-skins/releases) 下载最新版本
2. 解压到 `plugins/` 目录下
3. 启动服务器

安装后的目录结构：
```
plugins/
└── trust-all-skins/
    ├── trust-all-skins.dll
    └── manifest.json
```

## 从源码构建

### 前置要求

- [xmake](https://xmake.io/) 
- [Visual Studio 2022](https://visualstudio.microsoft.com/)（C++ 桌面 workload）
- [LLVM](https://releases.llvm.org/)（提供 `clang-cl`，与 LeviLamina 26.10 一致）
- [Git](https://git-scm.com/)

### 构建步骤

1. 克隆仓库：
   ```bash
   git clone https://github.com/YUMU1658/trust-all-skins.git
   cd trust-all-skins
   ```

2. 更新 xmake 仓库：
   ```bash
   xmake repo -u
   ```

3. 配置构建（需 [LLVM/clang-cl](https://releases.llvm.org/) 与 Visual Studio 生成工具）：
   ```bash
   xmake f -m release --target_type=server
   ```

4. 构建：
   ```bash
   xmake
   ```

5. 构建前请在 **“x64 Native Tools”** 或已执行 `vcvars64.bat` 的环境中运行（否则链接可能找不到 `msvcrt.lib`）

6. 构建完成后，模组目录位于 `bin/trust-all-skins/`（含 `trust-all-skins.dll` 与 `manifest.json`）

## 兼容性

- LeviLamina **26.10.x**（与 BDS **26.10.4** 对应）
- Minecraft Bedrock Edition 专用服务器

## 注意事项

- 此模组仅在服务器端运行
- 客户端不需要安装任何东西
- 此模组不会绕过任何安全检查，只是让服务器信任所有皮肤

## 许可证

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！
