#include "TrustAllSkins.h"

#include "ll/api/memory/Hook.h"

#include "mc/deps/core/utility/BinaryStream.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/world/actor/player/PlayerListEntry.h"
#include "mc/util/ThreadOwner.h"
#include "mc/world/actor/player/SerializedSkinImpl.h"
#include "mc/world/actor/player/SerializedSkinRef.h"
#include "mc/world/actor/player/TrustedSkinFlag.h"

namespace trust_all_skins {

namespace {

void markSkinTrusted(::SerializedSkinRef& skinRef) {
    auto const& owner = skinRef.mSkinImpl;
    if (!owner) {
        return;
    }
    auto* wrapper = owner.get();
    if (!wrapper) {
        return;
    }
    wrapper->mObject.mIsTrustedSkin = ::TrustedSkinFlag::True;
}

} // namespace

LL_TYPE_INSTANCE_HOOK(
    TrustAllSkinsHook,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::_decideIfSkinIsTrusted,
    void,
    ::SerializedSkinRef& skin
) {
    origin(skin);
    markSkinTrusted(skin);
    TrustAllSkins::getInstance().getSelf().getLogger().debug("Forced skin to be trusted (skin change)");
}

LL_TYPE_INSTANCE_HOOK(
    PlayerListEntryWriteHook,
    ll::memory::HookPriority::Normal,
    PlayerListEntry,
    &PlayerListEntry::write,
    void,
    ::BinaryStream& stream
) {
    markSkinTrusted(static_cast<PlayerListEntry*>(this)->mSkin);
    TrustAllSkins::getInstance().getSelf().getLogger().debug("Forced skin to be trusted (player list write)");
    origin(stream);
}

void TrustAllSkins::enableHook() {
    TrustAllSkinsHook::hook();
    PlayerListEntryWriteHook::hook();
    getInstance().getSelf().getLogger().info("Skin trust hooks enabled - all skins will be marked as trusted");
}

void TrustAllSkins::disableHook() {
    TrustAllSkinsHook::unhook();
    PlayerListEntryWriteHook::unhook();
    getInstance().getSelf().getLogger().info("Skin trust hooks disabled");
}

} // namespace trust_all_skins