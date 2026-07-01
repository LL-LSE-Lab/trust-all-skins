#pragma once

#include "ll/api/mod/NativeMod.h"

namespace trust_all_skins {

class TrustAllSkins {

public:
    static TrustAllSkins& getInstance();

    TrustAllSkins() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();

    bool enable();

    bool disable();

    static void enableHook();
    static void disableHook();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace trust_all_skins