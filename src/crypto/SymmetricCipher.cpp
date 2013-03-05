/*
 * SymmetricCipher.cpp
 *
 *  Created on: Feb 8, 2013
 *      Author: Darren
 */

#include "SymmetricCipher.h"
#include "SymmetricCipherOpenSSL.h"
#include "SymmetricCipherSalsa20.cpp"

SymmetricCipher::SymmetricCipher(SymmetricCipher::Algorithm algo,
		SymmetricCipher::Mode mode,
		SymmetricCipher::Direction direction,
		const QByteArray& key, const QByteArray& iv)
		:m_backend(createBackend(algo, mode, direction)){
	m_backend->init();
	m_backend->setKey(key);
	m_backend->setIv(iv);
}

SymmetricCipher::~SymmetricCipher(){

}

void SymmetricCipher::reset(){
	m_backend->reset();
}

int SymmetricCipher::blockSize() const{
	return m_backend->blockSize();
}

SymmetricCipherBackend* SymmetricCipher::createBackend(SymmetricCipher::Algorithm algo, SymmetricCipher::Mode mode,
                                                 SymmetricCipher::Direction direction){
	switch (algo){
	case SymmetricCipher::Aes256:
	case SymmetricCipher::Twofish:
		return new SymmetricCipherOpenSSL(algo, mode, direction);

	case SymmetricCipher::Salsa20:
		return new SymmetricCipherSalsa20(algo, mode, direction);

	default:
		Q_ASSERT(false);
		return Q_NULLPTR;
	}
}

