// entry\src\main\cpp\hello.cpp
// 引入N-API相关头文件。
#include "napi/native_api.h"

// 开发者提供的native方法，入参有且仅有如下两个，开发者不需进行变更。
// napi_env 为当前运行的上下文。
// napi_callback_info 记录了一些信息，包括从ArkTS侧传递过来参数等。
static napi_value Add(napi_env env, napi_callback_info info)
{
    // 期望从ArkTS侧获取的参数的数量，napi_value可理解为ArkTS value在native方法中的表现形式。
    size_t argc = 2;
    napi_value args[2] = {nullptr};
    
    // 从info中，拿到从ArkTS侧传递过来的参数，此处获取了两个ArkTS参数，即arg[0]和arg[1]。
    napi_get_cb_info(env, info, &argc, args         //Native API get callback info
                    , nullptr       //接收JavaScript中的this参数。
                    , nullptr);     //接收与回调关联的数据。这通常是在创建回调时传递的数据。

    // 将获取的ArkTS参数转换为native信息，此处ArkTS侧传入了两个number，这里将其转换为native侧可以操作的double类型。
    double value0;
    napi_get_value_double(env, args[0], &value0);
    

    double value1;
    napi_get_value_double(env, args[1], &value1);
    
    // native侧的业务逻辑，这里简单以两数相加为例。
    double nativeSum = value0 + value1;
    
    // 此处将native侧业务逻辑处理结果转换为ArkTS值，并返回给ArkTS。
    napi_value sum;
    napi_create_double(env, nativeSum , &sum);
    return sum;
}

static napi_value NativeCallArkTS(napi_env env, napi_callback_info info)
{
    // 期望从ArkTS侧获取的参数的数量，napi_value可理解为ArkTS value在native方法中的表现形式。
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    
    // 从info中，拿到从ArkTS侧传递过来的参数，此处获取了一个ArkTS参数，即arg[0]。
    napi_get_cb_info(env, info, &argc, args , nullptr, nullptr);
    
    // 创建一个ArkTS number作为ArkTS function的入参。
    napi_value argv = nullptr;
    napi_create_int32(env, 10, &argv);
    
    napi_value result = nullptr;
    // native方法中调用ArkTS function，其返回值保存到result中并返到ArkTS侧。
    napi_call_function(env, nullptr, args[0], 1, &argv, &result);
    
    return result;
}

EXTERN_C_START
// Init将在exports上挂上Add/NativeCallArkTS这些native方法，此处的exports就是开发者import之后获取到的ArkTS对象。
static napi_value Init(napi_env env, napi_value exports)
{
    // 函数描述结构体，以Add为例，第三个参数"Add"为上述的native方法，
    // 第一个参数"add"为ArkTS侧对应方法的名称。
    napi_property_descriptor desc[] = {
        { "add", nullptr, Add, nullptr, nullptr, nullptr, napi_default, nullptr },
        { "nativeCallArkTS", nullptr, NativeCallArkTS, nullptr, nullptr, nullptr, napi_default, nullptr },
    };
    // 在exports这个ArkTS对象上，挂载native方法。
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END

// 准备模块加载相关信息，将上述Init函数与本模块名等信息记录下来。
static napi_module demoModule = {
    .nm_version =1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "entry",
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

// 打开so时，该函数将自动被调用，使用上述demoModule模块信息，进行模块注册相关动作。
extern "C" __attribute__((constructor)) void RegisterHelloModule(void)
{
    napi_module_register(&demoModule);
}
