#ifndef KEEPASSX_SYMMETRICCIPHER_H
#define KEEPASSX_SYMMETRICCIPHER_H

#include <QtCore/QByteArray>
#include <QtCore/QScopedPointer>

#include "core/Global.h"
#include "crypto/SymmetricCipherBackend.h"

class SymmetricCipher
{
public:
    enum Algorithm
    {
        Aes256,
        Twofish,
        Salsa20
    };

    enum Mode
    {
        Cbc,
        Ecb,
        Stream
    };

    enum Direction
	{
		Decrypt,
		Encrypt
	};

    SymmetricCipher(SymmetricCipher::Algorithm algo, SymmetricCipher::Mode mode,
                    SymmetricCipher::Direction direction, const QByteArray& key, const QByteArray& iv);
    ~SymmetricCipher();

    inline QByteArray process(const QByteArray& data) {
        return m_backend->process(data);
    }

    inline void processInPlace(QByteArray& data) {
        m_backend->processInPlace(data);
    }

    inline void processInPlace(QByteArray& data, quint64 rounds) {
        Q_ASSERT(rounds > 0);
        m_backend->processInPlace(data, rounds);
    }

    void reset();
    int blockSize() const;

private:
    static SymmetricCipherBackend* createBackend(SymmetricCipher::Algorithm algo, SymmetricCipher::Mode mode,
                                                 SymmetricCipher::Direction direction);

    const QScopedPointer<SymmetricCipherBackend> m_backend;

    Q_DISABLE_COPY(SymmetricCipher)

};

#endif
