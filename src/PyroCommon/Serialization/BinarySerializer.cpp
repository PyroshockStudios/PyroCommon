#include "BinarySerializer.hpp"
#include <libassert/assert.hpp>

namespace PyroshockStudios {
    BinarySerializer::BinarySerializer(IStreamReader* reader, IStreamWriter* writer) : mReader(reader), mWriter(writer) {}
    void BinarySerializer::Write(const void* bytes, usize size) {
        ASSERT(mWriter != nullptr, "Stream Writer has not been set!");
        mWriter->Write(bytes, size);
    }
    void BinarySerializer::Read(void* out, usize size) {
        ASSERT(mReader != nullptr, "Stream Reader has not been set!");
        mReader->Read(out, size);
    }
}
