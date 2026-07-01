#include "TrustAllSkins.h"

#include <memory>

#include <ll/api/mod/RegisterHelper.h>

namespace trust_all_skins {

static std::unique_ptr<TrustAllSkins> instance;

TrustAllSkins& TrustAllSkins::getInstance() { return *instance; }

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

extern "C" {
LL_SHARED_EXPORT bool ll_mod_load(ll::mod::NativeMod& self) {
    trust_all_skins::instance = std::make_unique<trust_all_skins::TrustAllSkins>(self);
    return trust_all_skins::instance->load();
}

LL_SHARED_EXPORT bool ll_mod_unload(ll::mod::NativeMod& /* self */) {
    trust_all_skins::instance.reset();
    return true;
}

LL_SHARED_EXPORT bool ll_mod_enable(ll::mod::NativeMod& /* self */) {
    return trust_all_skins::instance->enable();
}

LL_SHARED_EXPORT bool ll_mod_disable(ll::mod::NativeMod& /* self */) {
    return trust_all_skins::instance->disable();
}
}
