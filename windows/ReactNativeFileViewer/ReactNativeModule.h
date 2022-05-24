#pragma once

#include <filesystem>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>
#include "JSValue.h"
#include "NativeModules.h"

namespace RN = winrt::Microsoft::ReactNative;

using namespace winrt;
using namespace Windows::Foundation;
using namespace winrt::Microsoft::ReactNative;
using namespace winrt::Windows::Storage;

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
    winrt::fire_and_forget Open(std::string filepath, RN::ReactPromise<bool> promise) noexcept
    try 
    {
        winrt::hstring directoryPath, fileName;
        splitPath(filepath, directoryPath, fileName);

        StorageFolder folder{ co_await StorageFolder::GetFolderFromPathAsync(directoryPath) };
        StorageFile file{ co_await folder.GetFileAsync(fileName) };
        if (file != nullptr) 
        {
            bool success{ co_await Windows::System::Launcher::LaunchFileAsync(file) };
            if (success) 
            {
                promise.Resolve(true);
            }
            else 
            {
                promise.Reject(RN::ReactError{ "Error opening the file" + filepath });
            }
        }
        else
        {
            promise.Reject(RN::ReactError{ "Error opening the file" + filepath });
        }
    }
    catch (const hresult_error& ex) {
        promise.Reject(winrt::to_string(ex.message()).c_str());
    }

    

    private:
        ReactContext m_reactContext{nullptr};

        void splitPath(const std::string& fullPath, winrt::hstring& directoryPath, winrt::hstring& fileName) noexcept
        {
            std::filesystem::path path(fullPath);
            path.make_preferred();

            directoryPath = path.has_parent_path() ? winrt::to_hstring(path.parent_path().c_str()) : L"";
            fileName = path.has_filename() ? winrt::to_hstring(path.filename().c_str()) : L"";
        }
};

} // namespace winrt::ReactNativeFileViewer
