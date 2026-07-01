#include "TrustAllSkins.h"

#include <ll/api/memory/Hook.h>
#include <ll/api/memory/Symbol.h>

namespace trust_all_skins {

// Forward declarations - using the same enum definition as in the SDK
enum class TrustedSkinFlag : signed char {
    Unset = 0,
    False = 1,
    True  = 2,
};

// The decorated symbol for ServerNetworkHandler::_decideIfSkinIsTrusted(SerializedSkinRef&)
// Updated for BDS 1.21.132+ / LeviLamina 1.9.2+
// Old symbol was: ?_decideIfSkinIsTrusted@ServerNetworkHandler@@QEAAXAEAVSerializedSkin@@@Z
// New symbol is:  ?_decideIfSkinIsTrusted@ServerNetworkHandler@@QEAAXAEAVSerializedSkinRef@@@Z
static constexpr ll::memory::FixedSymbol decideIfSkinIsTrustedSymbol{
    "?_decideIfSkinIsTrusted@ServerNetworkHandler@@QEAAXAEAVSerializedSkinRef@@@Z"
};

// The decorated symbol for PlayerListEntry::write(BinaryStream&) const
static constexpr ll::memory::FixedSymbol playerListEntryWriteSymbol{
    "?write@PlayerListEntry@@QEBAXAEAVBinaryStream@@@Z"
};

// Helper function to set skin as trusted in SerializedSkinRef
// SerializedSkinRef layout (BDS 1.21.132+):
// - mSkinImpl: shared_ptr<ThreadOwner<SerializedSkinImpl, 0>> (16 bytes)
// 
// ThreadOwner layout:
// - mObject: SerializedSkinImpl (the actual data at offset 0)
//
// SerializedSkinImpl member layout (calculated from header):
// - mId: std::string (32 bytes) - offset 0
// - mPlayFabId: std::string (32 bytes) - offset 32
// - mFullId: std::string (32 bytes) - offset 64
// - mResourcePatch: std::string (32 bytes) - offset 96
// - mDefaultGeometryName: std::string (32 bytes) - offset 128
// - mSkinImage: mce::Image (48 bytes) - offset 160
// - mCapeImage: mce::Image (48 bytes) - offset 208
// - mSkinAnimatedImages: std::vector<AnimatedImageData> (24 bytes) - offset 256
// - mGeometryData: Json::Value (16 bytes) - offset 280
// - mGeometryDataMinEngineVersion: MinEngineVersion (32 bytes) - offset 296
// - mGeometryDataMutable: Json::Value (16 bytes) - offset 328
// - mAnimationData: std::string (32 bytes) - offset 344
// - mCapeId: std::string (32 bytes) - offset 376
// - mPersonaPieces: std::vector<SerializedPersonaPieceHandle> (24 bytes) - offset 408
// - mArmSizeType: persona::ArmSize::Type (1 byte) - offset 432
// - padding (7 bytes) - offset 433
// - mPieceTintColors: std::unordered_map<persona::PieceType, TintMapColor> (64 bytes) - offset 440
// - mSkinColor: mce::Color (16 bytes) - offset 504
// - mIsTrustedSkin: TrustedSkinFlag (1 byte) - offset 520
inline void setSkinTrustedInSerializedSkinRef(void* serializedSkinRef) {
    if (!serializedSkinRef) {
        return;
    }
    
    // Get the raw pointer from shared_ptr (first 8 bytes of SerializedSkinRef)
    void* threadOwnerPtr = *reinterpret_cast<void**>(serializedSkinRef);
    if (!threadOwnerPtr) {
        return;
    }
    
    // mIsTrustedSkin is at offset 520 in SerializedSkinImpl
    // This offset is the same as before since the structure layout hasn't changed
    constexpr size_t TRUSTED_SKIN_OFFSET = 520;
    
    TrustedSkinFlag* trustedFlag = reinterpret_cast<TrustedSkinFlag*>(
        reinterpret_cast<char*>(threadOwnerPtr) + TRUSTED_SKIN_OFFSET
    );
    
    *trustedFlag = TrustedSkinFlag::True;
}

// Hook for _decideIfSkinIsTrusted - handles skin changes
// Updated parameter type from SerializedSkin to SerializedSkinRef
LL_INSTANCE_HOOK(
    TrustAllSkinsHook,
    ll::memory::HookPriority::Normal,
    decideIfSkinIsTrustedSymbol.view(),
    void,
    void* skin  // SerializedSkinRef*
) {
    origin(skin);
    setSkinTrustedInSerializedSkinRef(skin);
    TrustAllSkins::getInstance().getSelf().getLogger().debug("Forced skin to be trusted (skin change)");
}

// Hook for PlayerListEntry::write - handles player list packets (initial join)
// PlayerListEntry layout (BDS 1.21.132+):
// - mId: ActorUniqueID (8 bytes) - offset 0
// - mUUID: mce::UUID (16 bytes) - offset 8
// - mName: std::string (32 bytes) - offset 24
// - mXUID: std::string (32 bytes) - offset 56
// - mPlatformOnlineId: std::string (32 bytes) - offset 88
// - mBuildPlatform: BuildPlatform (4 bytes) - offset 120
// - padding (4 bytes for alignment) - offset 124
// - mSkin: SerializedSkinRef (16 bytes) - offset 128
// - mColor: mce::Color (16 bytes) - offset 144
// - mIsTeacher: bool (1 byte) - offset 160
// - mIsHost: bool (1 byte) - offset 161
// - mIsSubClient: bool (1 byte) - offset 162
LL_INSTANCE_HOOK(
    PlayerListEntryWriteHook,
    ll::memory::HookPriority::Normal,
    playerListEntryWriteSymbol.view(),
    void,
    void* binaryStream
) {
    // 'this' points to PlayerListEntry
    // mSkin is at offset 128 in PlayerListEntry (same as before)
    constexpr size_t SKIN_OFFSET_IN_PLAYER_LIST_ENTRY = 128;
    
    void* skinPtr = reinterpret_cast<void*>(
        reinterpret_cast<char*>(this) + SKIN_OFFSET_IN_PLAYER_LIST_ENTRY
    );
    
    setSkinTrustedInSerializedSkinRef(skinPtr);
    
    TrustAllSkins::getInstance().getSelf().getLogger().debug("Forced skin to be trusted (player list write)");
    
    origin(binaryStream);
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
