/*
 * CryptoHash.cpp
 *
 *  Created on: Feb 8, 2013
 *      Author: Darren
 */

#include "CryptoHash.h"

CryptoHash::CryptoHash(enum Algorithm algo){
	switch(algo){
	case CryptoHash::Sha256:
		md = EVP_sha256();
		break;

	default:
		Q_ASSERT(false);
		break;
	}

	mdctx = EVP_MD_CTX_create();
	EVP_DigestInit_ex(mdctx, md, NULL);
}

CryptoHash::~CryptoHash(){
	EVP_MD_CTX_destroy(mdctx);
}

QByteArray CryptoHash::result() const{
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int length;
	EVP_DigestFinal_ex(mdctx, md_value, &length);

	const char * result = (const char*)md_value;

	return QByteArray(result, length);
}

void CryptoHash::reset(){
	EVP_MD_CTX_destroy(mdctx);
	mdctx = EVP_MD_CTX_create();
	EVP_DigestInit_ex(mdctx, md, NULL);
}

void CryptoHash::addData(const QByteArray & data){
	EVP_DigestUpdate(mdctx, data.constData(), data.size());
}

QByteArray CryptoHash::hash(const QByteArray & data, enum Algorithm algo){
	CryptoHash cryptoHash(algo);
	cryptoHash.addData(data);
	return cryptoHash.result();
}

