#pragma once

#include <fstream>
#include <string>
#include <memory>
#include <unordered_map>

namespace Shade {

    // ======================================
    enum class ValueType {
        INT,
        FLOAT,
        STRING,
        LIST
    };

    // ======================================
    // TODO: Many ways to optimize this memory wise
    struct ValueOption {
        ValueType mType;

        int mInt = 0;
        float mFloat = 0.f;
        std::string mString = "";
        // std::unordered_map<std::string, std::shared_ptr<ValueOption>> mList;

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
    class KeyValueFile {
    public:
        typedef std::unordered_map<std::string, ValueOption> KeyValueList;
    public:
        KeyValueFile(KeyValueList&& mContents);

        static std::unique_ptr<KeyValueFile> LoadFile(std::ifstream& fileStream);

        const KeyValueList& GetContents() const;
    private:
        std::unordered_map<std::string, ValueOption> mContents;
    };

}