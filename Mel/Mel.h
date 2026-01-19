#ifndef MEL_H
#define MEL_H

#include "Mel_export.h"
#include <QString>

namespace Mel {

class MEL_EXPORT MelLib {
public:
    MelLib();

    ~MelLib();

    // 版本信息
    static QString getVersion();

    static QString getName();

    // 静态版本信息方法
    static int getVersionMajor();

    static int getVersionMinor();

    static int getVersionPatch();

    static QString getVersionString();
};

} // namespace Mel

#endif // MEL_H
