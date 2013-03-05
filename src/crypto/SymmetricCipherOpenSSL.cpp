/*
 * SymmetricCipherOpenSSL.cpp
 *
 *  Created on: Feb 8, 2013
 *      Author: Darren
 */

#include <QtCore/qpointer.h>

#include "SymmetricCipherOpenSSL.h"
#include "stdio.h"

SymmetricCipherOpenSSL::SymmetricCipherOpenSSL(SymmetricCipher::Algorithm algo,SymmetricCipher::Mode mode,SymmetricCipher::Direction direction){
	oAlgo = algo;
	oMode = mode;
	oDir = direction;
	ctx = new EVP_CIPHER_CTX;
	determineCip();
}

void SymmetricCipherOpenSSL::init(){
	EVP_CIPHER_CTX_init(ctx);
}

void SymmetricCipherOpenSSL::setKey(const QByteArray& data){
	oKey = data;
}

void SymmetricCipherOpenSSL::setIv(const QByteArray& iv){
	oIv = iv;
}

void SymmetricCipherOpenSSL::determineCip(){
	if (oAlgo == SymmetricCipher::Aes256){
			if (oMode == SymmetricCipher::Cbc){
				cip = EVP_aes_256_cbc();
			}
			else if (oMode == SymmetricCipher::Ecb){
				cip = EVP_aes_256_ecb();
			}
			else {
				Q_ASSERT(false);
			}
		}
		else if (oAlgo == SymmetricCipher::Twofish){
			printf("NOT IMPLEMENTED: TWOFISH");

			Q_ASSERT(false);
		}
}

QByteArray SymmetricCipherOpenSSL::process(const QByteArray& data){
	QByteArray newData = data;
	int delta = blockSize() - data.size();

	if (data.size() < blockSize()){
		for (int i = 0; i < delta; i++){
			char c = delta;
			newData.append(c);
		}
	}

	if (oDir == SymmetricCipher::Encrypt){
		int c_len = 0;
		int finalLen = 0;
		unsigned char ciphertext[data.size()];

		EVP_EncryptInit_ex(ctx, cip, NULL, (const unsigned char*)oKey.constData(), (const unsigned char*)oIv.constData());
		EVP_CIPHER_CTX_set_padding(ctx, 0);
		int toProcess = data.size();
		int i = 0;
		//while(i != toProcess){
			EVP_EncryptUpdate(ctx, ciphertext+i, &c_len, (const unsigned char*)newData.constData(), newData.size());
			finalLen += c_len;
			i += 16;
		//}

		//EVP_DecryptFinal_ex(ctx, ciphertext+c_len, &c_len);

		QByteArray result((const char*)ciphertext, finalLen);
		return result;
	}
	else if (oDir == SymmetricCipher::Decrypt){
		unsigned char plaintext[data.size()];
		int toProcess = data.size();
		int p_len = 0;
		int finalLen = 0;

		EVP_DecryptInit_ex(ctx, cip, NULL, (const unsigned char*)oKey.constData(), (const unsigned char*)oIv.constData());
		EVP_CIPHER_CTX_set_padding(ctx, 0);

		int i = 0;
		//while(i != toProcess){
			EVP_DecryptUpdate(ctx, plaintext+i, &p_len, (const unsigned char*)newData.constData(), newData.size());
			finalLen += p_len;
			i += 16;
		//}
		//decrypt left, then right
		//EVP_DecryptUpdate(ctx, plaintext+p_len, &f_len, (const unsigned char*)newData.constData(), data.size());

		//padding
		//EVP_DecryptFinal_ex(ctx, plaintext+p_len, &p_len);

		QByteArray result((const char*)plaintext, finalLen);
		return result;
	}
	Q_ASSERT(false);
	return QByteArray();
}

void SymmetricCipherOpenSSL::processInPlace(QByteArray & data){
	processInPlace(data, 1);
}

void SymmetricCipherOpenSSL::processInPlace(QByteArray & data, quint64 rounds){
	if (data.size() != blockSize()){
		int delta = blockSize() - data.size();
		for (int i = 0; i < delta; i++){
			char c = delta;
			data.append(c);
		}
	}
	int size = 0;
	if (oDir == SymmetricCipher::Encrypt){
		EVP_EncryptInit_ex(ctx,cip,NULL,(const unsigned char*)oKey.constData(), (const unsigned char*)oIv.constData());
		for (quint64 i = 0; i != rounds; i++){
			EVP_EncryptUpdate(ctx,(unsigned char*)data.data(), &size, (const unsigned char*)data.constData(), data.size());
		}
	}
	else {
		EVP_DecryptInit_ex(ctx,cip,NULL,(const unsigned char*)oKey.constData(), (const unsigned char*)oIv.constData());
		for (quint64 i = 0; i != rounds; i++){
			EVP_DecryptUpdate(ctx,(unsigned char*)data.data(), &size, (const unsigned char*)data.constData(), data.size());
		}
	}
}

void SymmetricCipherOpenSSL::reset(){
	EVP_CIPHER_CTX_cleanup(ctx);
	EVP_CIPHER_CTX_init(ctx);
	if (oDir == SymmetricCipher::Encrypt){
		EVP_EncryptInit_ex(ctx, cip, NULL, (const unsigned char*)oKey.constData(), (const unsigned char*)oIv.constData());
	}
	else {
		EVP_DecryptInit_ex(ctx, cip, NULL, (const unsigned char*)oKey.constData(), (const unsigned char*)oIv.constData());
	}
}

int SymmetricCipherOpenSSL::blockSize() const{
	return EVP_CIPHER_block_size(cip);
}

SymmetricCipherOpenSSL::~SymmetricCipherOpenSSL() {
	EVP_CIPHER_CTX_cleanup(ctx);
}

