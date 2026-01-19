#ifndef MEL_H
#define MEL_H

#include <QString>

namespace Mel {

class MelLib
{
public:
    MelLib();
    ~MelLib();
    
    QString getVersion() const;
    QString getName() const;
};

} // namespace Mel

#endif // MEL_H
