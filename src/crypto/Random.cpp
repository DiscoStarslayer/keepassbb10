/*
 * Random.cpp
 *
 *  Created on: Feb 7, 2013
 *      Author: Darren
 */

#include <openssl/rand.h>
#include <stdio.h>
#include "Random.h"


Random::Random(){

}

QByteArray Random::randomArray(int len){
	QByteArray ba;
	ba.resize(len);

	randomize(ba);

	return ba;
}

void Random::randomize(void* data, int len){
	int error;
	error = RAND_bytes((unsigned char*)data, len);

	if (error == 0){
		printf("RAND_bytes in Random.cpp failed");
	}
}

void Random::randomize(QByteArray & ba){
	randomize(ba.data(), ba.size());
}

quint32 Random::randomUInt(quint32 limit){
	quint32 rand;
	randomize(&rand, 4);
	return (rand % limit);
}

quint32 Random::randomUIntRange(quint32 min, quint32 max){
	return min + randomUInt(max - min);
}


