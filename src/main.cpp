// Navigation pane project template
#include "KeepassBerry.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>
#include <qobject.h>
#include <Qt/qdeclarativedebug.h>
#include <QtTest/qtest.h>

#include <openssl/evp.h>
//#include <sbengine/sbengine.h>

#include "core/Metadata.h"
#include "core/Database.h"
#include "format/KeePass1Reader.h"
#include "keys/CompositeKey.h"
#include "keys/PasswordKey.h"
#include "crypto/SymmetricCipher.h"
#include "crypto/CryptoHash.h"
#include "streams/SymmetricCipherStream.h"
#include "stdio.h"

#define KEEPASSX_TEST_DATA_DIR "/accounts/1000/shared/misc"

using namespace bb::cascades;

#define QCOMPARE(a, b) test = a == b

Q_DECL_EXPORT int main(int argc, char **argv)
{
	//ENGINE_load_sb(ENGINE_METHOD_ALL);
	//ENGINE_register_all_complete();
    // this is where the server is started etc
    //Application app(argc, argv);

    // localization support
//    QTranslator translator;
//    QString locale_string = QLocale().name();
//    QString filename = QString( "KeepassBerry_%1" ).arg( locale_string );
//    if (translator.load(filename, "app/native/qm")) {
//        app.installTranslator( &translator );
//    }

    QByteArray get, should;
    bool test = false;

    QByteArray key = QByteArray::fromHex("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4");
    QByteArray iv = QByteArray::fromHex("000102030405060708090a0b0c0d0e0f");
    QByteArray cipherText = QByteArray::fromHex("f58c4c04d6e5f1ba779eabfb5f7bfbd6");
    cipherText.append(QByteArray::fromHex("9cfc4e967edb808d679f777bc6702c7d"));
    QByteArray plainText = QByteArray::fromHex("6bc1bee22e409f96e93d7e117393172a");
    plainText.append(QByteArray::fromHex("ae2d8a571e03ac9c9eb76fac45af8e51"));

    SymmetricCipher cipher(SymmetricCipher::Aes256, SymmetricCipher::Cbc, SymmetricCipher::Decrypt, key, iv);
    QCOMPARE(cipher.blockSize(), 16);

    get = cipher.process(cipherText);
    QCOMPARE(get,
             plainText);

    // padded with 16 0x16 bytes
    QByteArray cipherTextPadded = cipherText + QByteArray::fromHex("3a3aa5e0213db1a9901f9036cf5102d2");
    QBuffer buffer(&cipherTextPadded);
    SymmetricCipherStream stream(&buffer, SymmetricCipher::Aes256, SymmetricCipher::Cbc,
                                 SymmetricCipher::Decrypt, key, iv);
    buffer.open(QIODevice::ReadOnly);
    stream.open(QIODevice::ReadOnly);

    get = stream.read(10);
    should = plainText.left(10);
    QCOMPARE(get,
             should);
    buffer.reset();
    stream.reset();
    get = stream.read(20);
    should = plainText.left(20);
    QCOMPARE(get,
             should);
    buffer.reset();
    stream.reset();
    QCOMPARE(stream.read(16),
             plainText.left(16));
    buffer.reset();
    stream.reset();
    get = stream.read(100);
    should = plainText;
    QCOMPARE(get,
             should);

//    CompositeKey key;
//    key.addKey(PasswordKey(QString("how did you get up the mountain togo?")));
//
//    KeePass1Reader reader;
//
//    Database *db = reader.readDatabase("/accounts/1000/shared/misc/test.kdb", "test", 0);
//
//    bool test2 = !reader.hasError();
//    QString test = db->metadata()->name();
//
//    bool test1, test2, test3;
//
//    CryptoHash cryptoHash1(CryptoHash::Sha256);
//    test1 = cryptoHash1.result() == QByteArray::fromHex("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
//
//    QByteArray source2 = QString("KeePassX").toAscii();
//    QByteArray result2 = CryptoHash::hash(source2, CryptoHash::Sha256);
//    test2 = result2 == QByteArray::fromHex("0b56e5f65263e747af4a833bd7dd7ad26a64d7a4de7c68e52364893dca0766b4");
//
//    CryptoHash cryptoHash3(CryptoHash::Sha256);
//    cryptoHash3.addData(QString("KeePa").toAscii());
//    cryptoHash3.addData(QString("ssX").toAscii());
//    test3 = cryptoHash3.result() == QByteArray::fromHex("0b56e5f65263e747af4a833bd7dd7ad26a64d7a4de7c68e52364893dca0766b4");
//
//    // we complete the transaction started in the app constructor and start the client event loop here
//    return Application::exec();
//    // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
