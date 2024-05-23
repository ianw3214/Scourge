#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace Shade {

    // ======================================
    enum class ValueType : int8_t {
        UNKNOWN = -1,
        INT,
        FLOAT,
        STRING,
        LIST
    };

    // ======================================
    struct ValueOption {
        ValueType mType = ValueType::UNKNOWN;

        // TODO: Should this use a union?
        int mInt = 0;
        float mFloat = 0.f;
        std::string mString = "";

        static ValueOption IntOption(int i) {
            ValueOption option = { ValueType::INT };
            option.mInt = i;
            return option;
        };
        static ValueOption FloatOption(float f) {
            ValueOption option = { ValueType::FLOAT };
            option.mFloat = f;
            return option;
        };
        static ValueOption StringOption(const std::string& str) {
            ValueOption option = { ValueType::STRING };
            option.mString = str;
            return option;
        };
        static ValueOption ListOption() {
            ValueOption option = { ValueType::LIST };
            return option;
        };
    };

    // ======================================
    struct KeyValuePair {
        std::string mKey;
        ValueOption mValue;
        
        uint8_t mDepth = 0;
    };

    // ======================================
    // - This stores the key value pair buffer as a reference directly
    //   - Handle is not meant for long term storage, copy and store the data instead
    class KeyValueHandle {
    public:
        KeyValueHandle(const std::vector<KeyValuePair>& bufferRef);
        KeyValueHandle(const std::vector<KeyValuePair>& bufferRef, size_t index);
        KeyValueHandle Invalid(const std::vector<KeyValuePair>& bufferRef) const;
        void Invalidate();

        bool IsValid() const;
        bool ToNext();

        bool IsInt() const;
        bool IsFloat() const;
        bool IsString() const;
        bool IsList() const;

        const std::string& GetKey() const;
        int GetInt() const;
        float GetFloat() const;
        const std::string& GetString() const;
        KeyValueHandle GetListHead() const;

    private:
        size_t mIndex = 0;
        const std::vector<KeyValuePair>& mBufferRef;
    };

    // ======================================
    class KeyValueFile {
    public:
        KeyValueFile(std::vector<KeyValuePair>&& buffer);

        static std::unique_ptr<KeyValueFile> LoadFile(std::ifstream& fileStream);
        bool SaveFile(std::ofstream& fileStream) const;

        // Returns the handle to the first key/value pair in the buffer
        KeyValueHandle GetContents() const;

        // Writing to file/direct buffer modifications
        // - This should be called before writing a new file to ensure state is cleared
        static KeyValueFile CreateFileForWrite();
        void EndWriteNewFile();
        // - The helper functions should be used for file writing to ensure depth is properly accounted for 
        void AddIntEntry(const std::string& key, int value);
        void AddFloatEntry(const std::string& key, float value);
        void AddStringEntry(const std::string& key, const std::string& value);
        void PushList(const std::string& key);
        void PopList();

    protected:
        KeyValueFile() {}

    private:
        std::vector<KeyValuePair> mBuffer;

        // State for writing to file/direct buffer modifications
        uint8_t mCurrentDepth = 0;
    };

}