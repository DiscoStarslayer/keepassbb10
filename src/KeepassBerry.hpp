// Navigation pane project template
#ifndef KeepassBerry_HPP_
#define KeepassBerry_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class KeepassBerry : public QObject
{
    Q_OBJECT
public:
    KeepassBerry(bb::cascades::Application *app);
    virtual ~KeepassBerry() {}
};

#endif /* KeepassBerry_HPP_ */