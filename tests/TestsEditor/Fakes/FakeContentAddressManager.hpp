#pragma once

#include <EASTL/hash_map.h>
#include <SWCommon/CAS/IContentAddressManager.hpp>
#include <SWCommon/Stream/MemoryStream.hpp>
#include <SWRHI/Shader/IShaderFeatureSet.hpp>

using namespace PyroshockStudios;
using namespace PyroshockStudios::Types;
using namespace PyroshockStudios::RHI;

struct FakeContentAddressManager : public IContentAddressManager {
    FakeContentAddressManager() = default;
    ~FakeContentAddressManager() {
    }
    eastl::string GetContentAbsolutePath(GUID address) override {
        return {};
    }
    IStreamReader* CreateContentReadStream(GUID address) override {
        return mLoadedContent[address];
    }
    IStreamWriter* CreateContentWriteStream(GUID address) override {
        auto*& writer = mLoadedContent[address];
        if (!writer) {
            writer = new MemoryStream();
        }
        return writer;
    }
    void ReleaseContentStream(IStreamReader*& stream) override {
        static_cast<MemoryStream*>(stream)->Seek(0);
        stream = nullptr;
    }
    void ReleaseContentStream(IStreamWriter*& stream) override {
        static_cast<MemoryStream*>(stream)->Seek(0);
        stream = nullptr;
    }
    void Reset() {
        for (const auto& [k, v] : mLoadedContent) {
            if (v)
                delete v;
        }
        mLoadedContent.clear();
    }

private:
    eastl::hash_map<GUID, MemoryStream*> mLoadedContent;
};