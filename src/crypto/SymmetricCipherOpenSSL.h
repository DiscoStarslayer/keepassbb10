/*
 * SymmetricCipherOpenSSL.h
 *
 *  Created on: Feb 8, 2013
 *      Author: Darren
 */

#ifndef SYMMETRICCIPHEROPENSSL_H_
#define SYMMETRICCIPHEROPENSSL_H_

#include <QtCore/QPointer>
#include <QtCore/QByteArray>
#include <openssl/evp.h>

#include "SymmetricCipher.h"
#include "SymmetricCipherBackend.h"

class SymmetricCipherOpenSSL : public SymmetricCipherBackend
{
public:
	SymmetricCipherOpenSSL(SymmetricCipher::Algorithm algo,SymmetricCipher::Mode mode,SymmetricCipher::Direction direction);
	virtual ~SymmetricCipherOpenSSL();

	void init();
	void setKey(const QByteArray& key);
	void setIv(const QByteArray& iv);

	QByteArray process(const QByteArray& data, quint64 rounds);
	QByteArray process(const QByteArray& data);
	void processInPlace(QByteArray& data);
	void processInPlace(QByteArray& data, quint64 rounds);

	void reset();
	int blockSize() const;
private:
	inline void determineCip();

	SymmetricCipher::Algorithm oAlgo;
	SymmetricCipher::Mode oMode;
	SymmetricCipher::Direction oDir;
	QByteArray oKey;
	QByteArray oIv;
	EVP_CIPHER_CTX * ctx;
	const EVP_CIPHER * cip;
};

#endif /* SYMMETRICCIPHEROPENSSL_H_ */
