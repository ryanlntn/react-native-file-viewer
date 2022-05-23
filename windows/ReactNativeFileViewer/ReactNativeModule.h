#pragma once

#include "JSValue.h"
#include "NativeModules.h"

namespace RN = winrt::Microsoft::ReactNative;

using namespace winrt::Microsoft::ReactNative;

namespace winrt::ReactNativeFileViewer
{

REACT_MODULE(ReactNativeModule, L"ReactNativeFileViewer")
struct ReactNativeModule
{
    // See https://microsoft.github.io/react-native-windows/docs/native-modules for details on writing native modules

    REACT_INIT(Initialize)
    void Initialize(ReactContext const &reactContext) noexcept
    {
        m_reactContext = reactContext;
    }
    
    REACT_METHOD(Open, L"open")
    void Open(std::string filepath, RN::ReactPromise<bool>&& promise) noexcept
    {
        promise.Resolve(true);
    }

    private:
        ReactContext m_reactContext{nullptr};
};

} // namespace winrt::ReactNativeFileViewer
