#ifndef QCOMPRESSOR_H
#define QCOMPRESSOR_H

#include <QObject>
#include <QByteArray>
#include <QIODevice>

#define KB 1024ul
#define MB 1048576ul
#define GB 1073741824ul

class QCompressor : public QObject
{
public:
	explicit QCompressor(QIODevice *outDevice, QObject *parent);
	explicit QCompressor(QByteArray *outArray, QObject *parent);
	explicit QCompressor(QIODevice *outDevice, quint32 blockSize = 100*MB, int compressionRate = 9, QObject *parent = NULL);
	explicit QCompressor(QByteArray *outArray, quint32 blockSize = 100*MB, int compressionRate = 9, QObject *parent = NULL);
	~QCompressor();

	void addData(QByteArray data);
	void addData(QIODevice *device);

	bool finish();

private:
	const quint32 blockSize;
	const int compressionRate;
	QIODevice *outDevice;
	uchar *blockBuffer;
	quint32 bufferPos;
	bool allOk;

	void writeBlock();
};

class QDecompressor : public QObject
{
public:
	explicit QDecompressor(QIODevice *outDevice, QObject *parent = NULL);
	explicit QDecompressor(QByteArray *outArray, QObject *parent = NULL);
	~QDecompressor();

	void addData(QByteArray data);
	void addData(QIODevice *device);

	bool finish();

private:
	QIODevice *outDevice;
	uchar *blockBuffer;
	quint32 blockSize;
	QByteArray blockSizeBuffer;
	quint32 bufferPos;
	bool allOk;

	void writeBlock();
};

#endif // QCOMPRESSOR_H
