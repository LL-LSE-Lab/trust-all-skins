#include "TrustAllSkins.h"

#include "ll/api/mod/RegisterHelper.h"

namespace trust_all_skins {

TrustAllSkins& TrustAllSkins::getInstance() {
    static TrustAllSkins instance;
    return instance;
}

bool TrustAllSkins::load() {
    getSelf().getLogger().info("TrustAllSkins loaded!");
    return true;
}

bool TrustAllSkins::enable() {
    getSelf().getLogger().info("TrustAllSkins enabling - hooking skin trust check...");
    enableHook();
    return true;
}

bool TrustAllSkins::disable() {
    getSelf().getLogger().info("TrustAllSkins disabling - unhooking skin trust check...");
    disableHook();
    return true;
}

} // namespace trust_all_skins

LL_REGISTER_MOD(trust_all_skins::TrustAllSkins, trust_all_skins::TrustAllSkins::getInstance());