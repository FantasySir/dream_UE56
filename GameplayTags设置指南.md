# GameplayTags 设置完整指南

## 概述

您的项目已经在代码中定义了 GameplayTags，现在需要在编辑器中正确设置以确保系统能够正常工作。

## 当前已有的 GameplayTags

根据您的代码，以下标签需要在编辑器中创建：

### 能力标签 (Ability)
- `Ability.TimePause` - 时间暂停能力
- `Ability.Summon` - 召唤单位能力  
- `Ability.Attack` - 攻击能力
- `Ability.Defense` - 防御能力
- `Ability.Heal` - 治疗能力

### 状态标签 (State)
- `State.Alive` - 存活状态
- `State.Dead` - 死亡状态
- `State.Stunned` - 眩晕状态
- `State.Invincible` - 无敌状态
- `State.TimePaused` - 时间暂停状态

### 单位类型标签 (Unit.Type)
- `Unit.Type.Player` - 玩家单位
- `Unit.Type.Enemy` - 敌方单位
- `Unit.Type.Neutral` - 中立单位

### 事件标签 (Event)
- `Event.Death` - 死亡事件
- `Event.Summon` - 召唤事件
- `Event.TimePause` - 时间暂停事件
- `Event.TimeResume` - 时间恢复事件

### 效果标签 (Effect)
- `Effect.Damage` - 伤害效果
- `Effect.Heal` - 治疗效果
- `Effect.ActionPointCost` - 行动点消耗效果

## 设置方法

### 方法 1: 直接在编辑器中创建 (推荐)

1. **打开 Project Settings**
   - 菜单栏 → Edit → Project Settings

2. **找到 GameplayTags 设置**
   - 在左侧搜索 "Gameplay Tags"
   - 或在 Game 分类下找到 "Gameplay Tags"

3. **添加 GameplayTags**
   - 点击 "Add New Gameplay Tag"
   - 输入标签名称 (例如: `Ability.TimePause`)
   - 添加描述/注释
   - 点击 "Add Tag"

### 方法 2: 使用 DataTable (可选，用于复杂数据)

虽然基础的 GameplayTags 不需要 DataTable，但如果您想要为标签添加额外的元数据，可以创建 DataTable。

## 完整的编辑器设置步骤

### 步骤 1: 创建基础标签结构

1. 打开 Project Settings → Gameplay Tags
2. 按以下顺序创建父标签：
   ```
   Ability
   State  
   Unit
   Event
   Effect
   ```

3. 创建子分类：
   ```
   Unit.Type
   ```

### 步骤 2: 创建具体标签

按照以下列表逐一创建标签：

#### Ability 分类
```
Ability.TimePause (描述: 时间暂停能力)
Ability.Summon (描述: 召唤单位能力)
Ability.Attack (描述: 攻击能力)
Ability.Defense (描述: 防御能力)
Ability.Heal (描述: 治疗能力)
```

#### State 分类
```
State.Alive (描述: 存活状态)
State.Dead (描述: 死亡状态)
State.Stunned (描述: 眩晕状态)
State.Invincible (描述: 无敌状态)
State.TimePaused (描述: 时间暂停状态)
```

#### Unit.Type 分类
```
Unit.Type.Player (描述: 玩家单位)
Unit.Type.Enemy (描述: 敌方单位)
Unit.Type.Neutral (描述: 中立单位)
```

#### Event 分类
```
Event.Death (描述: 死亡事件)
Event.Summon (描述: 召唤事件)
Event.TimePause (描述: 时间暂停事件)
Event.TimeResume (描述: 时间恢复事件)
```

#### Effect 分类
```
Effect.Damage (描述: 伤害效果)
Effect.Heal (描述: 治疗效果)
Effect.ActionPointCost (描述: 行动点消耗效果)
```

## 高级设置 - 使用 DataTable (可选)

如果您需要为 GameplayTags 添加额外的数据，可以创建以下 DataTable：

### 创建 DataTable 的情况

1. **能力配置数据**
   - 能力冷却时间
   - 能力消耗
   - 能力描述文本
   - 能力图标

2. **状态效果数据**
   - 状态持续时间
   - 状态叠加规则
   - 状态视觉效果

### 建议的 DataTable 结构

#### 1. AbilityDataTable (能力数据表)
```cpp
USTRUCT(BlueprintType)
struct FAbilityData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag AbilityTag;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cost;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> Icon;
};
```

#### 2. StateDataTable (状态数据表)
```cpp
USTRUCT(BlueprintType)
struct FStateData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag StateTag;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Duration;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bCanStack;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FLinearColor UIColor;
};
```

### 创建 DataTable 的步骤

1. **创建结构体**
   - 在 C++ 中定义数据结构
   - 继承自 `FTableRowBase`
   - 编译项目

2. **在编辑器中创建 DataTable**
   - Content Browser → 右键 → Miscellaneous → Data Table
   - 选择对应的行结构体
   - 命名：
     - `DT_AbilityData` (能力数据表)
     - `DT_StateData` (状态数据表)
     - `DT_UnitData` (单位数据表)

3. **填充数据**
   - 双击 DataTable 打开
   - 添加行并填入数据

## 验证设置

### 检查标签是否正确创建

1. **在蓝图中验证**
   - 创建测试蓝图
   - 添加 GameplayTag 变量
   - 查看下拉列表是否包含所有标签

2. **在代码中验证**
   - 确保 `FEWGameplayTags::InitializeNativeTags()` 在游戏启动时被调用
   - 在 `GameInstance` 或 `GameMode` 的 `BeginPlay` 中调用

### 常见问题排查

1. **标签未显示**
   - 检查项目设置中是否正确添加
   - 重启编辑器
   - 重新编译项目

2. **代码中找不到标签**
   - 确保调用了 `InitializeNativeTags()`
   - 检查标签名称拼写

## 最佳实践

1. **命名规范**
   - 使用层级结构 (Parent.Child.SubChild)
   - 使用英文名称，中文注释
   - 保持一致的命名风格

2. **组织结构**
   - 按功能分类组织标签
   - 避免过深的嵌套层级
   - 为每个标签添加有意义的描述

3. **维护管理**
   - 定期审查和清理不用的标签
   - 保持代码和编辑器设置同步
   - 使用版本控制管理标签更改

## 下一步

1. 按照上述步骤在编辑器中创建所有 GameplayTags
2. 验证标签在蓝图和代码中都能正常访问
3. 考虑是否需要创建 DataTable 来存储额外的配置数据
4. 测试 GAS 功能是否正常工作
