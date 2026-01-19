#include "Mel.h"
#include "Mel_version.h"  // 生成的版本头文件

namespace Mel {

MelLib::MelLib() = default;

MelLib::~MelLib() = default;

QString MelLib::getVersion() {
    return MEL_VERSION_STRING;
}

QString MelLib::getName() {
    return "Mel Library";
}

int MelLib::getVersionMajor() {
    return MEL_VERSION_MAJOR;
}

int MelLib::getVersionMinor() {
    return MEL_VERSION_MINOR;
}

int MelLib::getVersionPatch() {
    return MEL_VERSION_PATCH;
}

QString MelLib::getVersionString() {
    return MEL_VERSION_STRING;
}

} // namespace Mel
