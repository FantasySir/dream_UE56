# UI框架使用教程

## 架构概述

本UI框架采用了三层架构：
1. **Actor层** - 游戏逻辑和数据（如EWCharacterBase、EWUnitManager）
2. **WidgetController层** - UI和游戏逻辑的中间层
3. **Widget层** - 实际的UI控件

## 1. Widget Controller 配置

### 1.1 创建WidgetController蓝图
1. 在内容浏览器中，右键 -> 蓝图类 -> 选择对应的WidgetController基类：
   - `EWOverlayWidgetController` - 用于HUD overlay
   - `EWAttributeMenuWidgetController` - 用于属性菜单
   - `EWUnitManagementWidgetController` - 用于单位管理

### 1.2 HUD设置
1. 创建HUD蓝图，继承自 `EWHUD`
2. 在默认值中设置：
   - `OverlayWidgetClass` - 设置为你的Overlay Widget蓝图
   - `OverlayWidgetControllerClass` - 设置为对应的Controller蓝图
   - 其他Widget和Controller类也同样设置

## 2. UI控件配置

### 2.1 创建基础控件

#### 属性进度条 (EWAttributeProgressBar)
```cpp
// 在蓝图中绑定事件
void OnWidgetControllerSet()
{
    // 获取OverlayWidgetController
    auto Controller = Cast<UEWOverlayWidgetController>(WidgetController);
    if (Controller)
    {
        // 绑定血量变化事件
        Controller->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
        Controller->OnMaxHealthChanged.AddDynamic(this, &ThisClass::OnMaxHealthChanged);
    }
}

void OnHealthChanged(float NewHealth)
{
    // 更新血量显示
    SetProgress(NewHealth, MaxHealth);
}
```

#### 单位槽位控件 (EWUnitSlotWidget)
```cpp
// 处理拖拽放置
bool NativeOnDrop(...)
{
    // 自动处理单位槽位的拖拽逻辑
    // 可以通过UnitManagementWidgetController进行单位管理
}
```

### 2.2 属性菜单配置
1. 创建Widget蓝图，继承自 `EWUserWidget`
2. 添加属性显示控件
3. 在 `OnWidgetControllerSet` 中绑定属性变化事件：

```cpp
void OnWidgetControllerSet()
{
    auto Controller = Cast<UEWAttributeMenuWidgetController>(WidgetController);
    if (Controller)
    {
        Controller->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthUpdated);
        Controller->OnManaChanged.AddDynamic(this, &ThisClass::OnManaUpdated);
        // ... 绑定其他属性
    }
}
```

### 2.3 单位管理界面配置
1. 创建单位管理Widget蓝图
2. 添加单位槽位控件和战斗槽位控件
3. 绑定单位管理事件：

```cpp
void OnWidgetControllerSet()
{
    auto Controller = Cast<UEWUnitManagementWidgetController>(WidgetController);
    if (Controller)
    {
        Controller->OnUnitSlotChanged.AddDynamic(this, &ThisClass::OnUnitSlotUpdated);
        Controller->OnBattleSlotChanged.AddDynamic(this, &ThisClass::OnBattleSlotUpdated);
        Controller->OnSummonCostChanged.AddDynamic(this, &ThisClass::OnSummonCostUpdated);
    }
}
```

## 3. 游戏模式集成

### 3.1 设置HUD类
1. 在GameMode蓝图中，设置HUD类为你创建的HUD蓝图
2. 确保PlayerController使用 `EWPlayerController`

### 3.2 初始化UI
在PlayerController的BeginPlay中：
```cpp
void AEWPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    if (AEWHUD* EWHud = Cast<AEWHUD>(GetHUD()))
    {
        if (AEWCharacterBase* Character = Cast<AEWCharacterBase>(GetPawn()))
        {
            UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
            UAttributeSet* AS = Character->GetAttributeSet();
            
            EWHud->InitOverlay(this, GetPlayerState(), ASC, AS);
        }
    }
}
```

## 4. 输入绑定

### 4.1 添加UI输入动作
在PlayerController中添加UI相关的InputAction：
```cpp
// 在头文件中
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|UI")
TObjectPtr<UInputAction> OpenAttributeMenuAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|UI")
TObjectPtr<UInputAction> OpenUnitManagementAction;

// 在SetupInputComponent中绑定
void SetupInputComponent()
{
    // ... 其他绑定
    
    if (OpenAttributeMenuAction)
    {
        EnhancedInputComponent->BindAction(OpenAttributeMenuAction, ETriggerEvent::Triggered, this, &AEWPlayerController::OpenAttributeMenu);
    }
    
    if (OpenUnitManagementAction)
    {
        EnhancedInputComponent->BindAction(OpenUnitManagementAction, ETriggerEvent::Triggered, this, &AEWPlayerController::OpenUnitManagement);
    }
}

// 实现函数
void OpenAttributeMenu()
{
    if (AEWHUD* EWHud = Cast<AEWHUD>(GetHUD()))
    {
        EWHud->ToggleAttributeMenu();
    }
}
```

## 5. 数据绑定示例

### 5.1 血量条绑定
```cpp
// 在血量条Widget的蓝图中
UFUNCTION(BlueprintCallable)
void UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
    float Percent = MaxHealth > 0 ? CurrentHealth / MaxHealth : 0;
    SetProgressPercent(Percent);
    
    FText HealthText = FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth));
    SetDisplayText(HealthText);
}
```

### 5.2 单位槽位更新
```cpp
UFUNCTION(BlueprintCallable)
void OnUnitSlotUpdated(int32 SlotIndex, const FUnitSlotData& SlotData)
{
    if (SlotIndex == GetSlotIndex())
    {
        SetUnitSlotData(SlotData);
        
        // 更新UI显示
        if (SlotData.UnitClass)
        {
            // 显示单位信息
            SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            // 隐藏或显示空槽位
            SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
```

## 6. 扩展建议

1. **动画效果** - 可以在Widget蓝图中添加动画，在属性变化时播放
2. **音效集成** - 在UI事件中播放对应音效
3. **本地化** - 使用UE5的本地化系统处理文本
4. **样式系统** - 创建可复用的样式资源
5. **数据驱动** - 使用DataTable配置UI元素

这个框架提供了完整的UI-逻辑分离，支持数据绑定、事件驱动，便于维护和扩展。
