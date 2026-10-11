#include "TrustAllSkins.h"

#include "ll/api/memory/Hook.h"

#include "mc/deps/core/utility/BinaryStream.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/packet/PlayerListPacket.h"
#include "mc/network/packet/PlayerListPacketPayload.h"
#include "mc/world/actor/player/SerializedSkinRef.h"
#include "mc/world/actor/player/TrustedSkinFlag.h"

#include <variant>

namespace trust_all_skins {

namespace {

void markSkinTrusted(::SerializedSkinRef& skinRef) {
    if (!skinRef.isValid()) {
        return;
    }
    skinRef.setIsTrustedSkinFlag(::TrustedSkinFlag::True);
}

void markPlayerListSkins(::PlayerListPacket const& packet) {
    for (auto const& entry : packet.mEntries.get()) {
        if (auto const* addEntry = std::get_if<::PlayerListPacketPayload::AddEntry>(&entry)) {
            markSkinTrusted(const_cast<::SerializedSkinRef&>(addEntry->mSkin.get()));
        }
    }
}

} // namespace

LL_TYPE_STATIC_HOOK(
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
    PlayerListPacketWriteHook,
    ll::memory::HookPriority::Normal,
    ::PlayerListPacket,
    &::PlayerListPacket::$write,
    void,
    ::BinaryStream& stream
) {
    markPlayerListSkins(*static_cast<::PlayerListPacket*>(this));
    TrustAllSkins::getInstance().getSelf().getLogger().debug("Forced skin to be trusted (player list write)");
    origin(stream);
}

LL_TYPE_INSTANCE_HOOK(
    PlayerListPacketCerealWriteHook,
    ll::memory::HookPriority::Normal,
    ::PlayerListPacket,
    &::PlayerListPacket::$write,
    void,
    ::BinaryStream& stream,
    ::cereal::ReflectionCtx const& reflectionCtx
) {
    markPlayerListSkins(*static_cast<::PlayerListPacket*>(this));
    TrustAllSkins::getInstance().getSelf().getLogger().debug("Forced skin to be trusted (player list cereal write)");
    origin(stream, reflectionCtx);
}

void TrustAllSkins::enableHook() {
    TrustAllSkinsHook::hook();
    PlayerListPacketWriteHook::hook();
    PlayerListPacketCerealWriteHook::hook();
    getInstance().getSelf().getLogger().info("Skin trust hooks enabled - all skins will be marked as trusted");
}

void TrustAllSkins::disableHook() {
    TrustAllSkinsHook::unhook();
    PlayerListPacketWriteHook::unhook();
    PlayerListPacketCerealWriteHook::unhook();
    getInstance().getSelf().getLogger().info("Skin trust hooks disabled");
}

} // namespace trust_all_skins
