#pragma once

#include <ll/api/mod/NativeMod.h>

namespace trust_all_skins {

class TrustAllSkins {

public:
    static TrustAllSkins& getInstance();

    TrustAllSkins(ll::mod::NativeMod& self) : mSelf(self) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    // Hook management - defined in TrustAllSkinsHook.cpp
    static void enableHook();
    static void disableHook();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace trust_all_skins
