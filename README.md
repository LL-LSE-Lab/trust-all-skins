# Trust All Skins

一个 LeviLamina 模组，让服务器信任所有玩家的皮肤。

## 功能

安装此模组后，服务器会将所有玩家的皮肤标记为"受信任"，这意味着：

- 其他玩家**无需**关闭游戏设置中的"仅允许受信任的皮肤"选项
- 所有玩家都可以看到其他玩家的自定义皮肤
- 包括自定义几何体（4D皮肤）和自定义披风

## 工作原理

此模组通过Hook `ServerNetworkHandler::_decideIfSkinIsTrusted` 函数，将所有皮肤的 `mIsTrustedSkin` 标志强制设置为 `True`。当客户端收到标记为受信任的皮肤时，即使开启了"仅允许受信任的皮肤"选项，也会正常显示该皮肤。

## 安装

### 使用 lip 安装（推荐）

```bash
lip install github.com/yourusername/trust-all-skins
```

### 手动安装

1. 从 [Releases](https://github.com/yourusername/trust-all-skins/releases) 下载最新版本
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
- [Visual Studio 2022](https://visualstudio.microsoft.com/) 或更高版本（需要 MSVC 编译器）
- [Git](https://git-scm.com/)

### 构建步骤

1. 克隆仓库：
   ```bash
   git clone https://github.com/yourusername/trust-all-skins.git
   cd trust-all-skins
   ```

2. 更新 xmake 仓库：
   ```bash
   xmake repo -u
   ```

3. 配置构建（可选择 debug/release/releasedbg 模式）：
   ```bash
   xmake f -m release
   ```

4. 构建：
   ```bash
   xmake
   ```

5. 构建完成后，模组文件将位于 `bin/trust-all-skins/` 目录

## 兼容性

- LeviLamina 1.x
- Minecraft Bedrock Edition 专用服务器

## 注意事项

- 此模组仅在服务器端运行
- 客户端不需要安装任何东西
- 此模组不会绕过任何安全检查，只是让服务器信任所有皮肤

## 许可证

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！
