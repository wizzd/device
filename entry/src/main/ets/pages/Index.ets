// @ts-nocheck
// entry\src\main\ets\pages\index.ets

import hilog from '@ohos.hilog';
// 通过import的方式，引入native能力。
import entry from 'libhello.so'
import Ability from '@ohos.app.ability.Ability';
import abilityManager from '@ohos.app.ability.abilityManager';
import drive_steering_engine_api from '@ohos.drive_steering_engine_api'
import prompt from '@system.prompt'
@Entry
@Component
struct Index {

  build() {
    Row() {
      Column() {
        // 第一个按钮，调用add方法，对应到native侧的Add方法，进行两数相加。
        Button('ArkTS call C++')
          .fontSize(50)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            hilog.isLoggable(0x0000, 'testTag', hilog.LogLevel.INFO);
            hilog.info(0x0000, 'testTag', 'Test NAPI 2 + 3 = %{public}d', entry.add(2, 3));
            abilityManager.getAbilityRunningInfos();
            let strFromNAPI =drive_steering_engine_api.getDrive_steering_engine_apiString();
            prompt.showToast({ message: strFromNAPI })
          })
          .margin(10)
        // 第二个按钮，调用nativeCallArkTS方法，对应到native的NativeCallArkTS，在native中执行ArkTS function。
        Button('C++ call ArkTS')
          .fontSize(50)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            hilog.isLoggable(0x0000, 'testTag', hilog.LogLevel.INFO);
            let ret = entry.nativeCallArkTS((value)=>{return value * 2;});
            hilog.info(0x0000, 'testTag', 'Test NAPI nativeCallArkTS ret = %{public}d', ret);
          })
          .margin(10)
      }
      .width('100%')
    }
    .height('100%')
  }
}

