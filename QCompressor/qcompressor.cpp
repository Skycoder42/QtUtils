#include "qcompressor.h"
#include <QBuffer>
#include <QtEndian>

QCompressor::QCompressor(QIODevice *outDevice, QObject *parent) :
	QCompressor(outDevice, 100*MB, 9, parent)
{}

QCompressor::QCompressor(QByteArray *outArray, QObject *parent) :
	QCompressor(outArray, 100*MB, 9, parent)
{}

QCompressor::QCompressor(QIODevice *outDevice, quint32 blockSize, int compressionRate, QObject *parent) :
	QObject(parent),
	blockSize(blockSize),
	compressionRate(compressionRate),
	outDevice(outDevice),
	blockBuffer(new uchar[blockSize]),
	bufferPos(0),
	allOk(true)
{
	if(outDevice->isOpen()) {
		QIODevice::OpenMode mode = outDevice->openMode();
		if(!mode.testFlag(QIODevice::WriteOnly)) {
			outDevice->close();
			outDevice->open(mode | QIODevice::WriteOnly);
		}
	} else
		outDevice->open(QIODevice::WriteOnly);
}

QCompressor::QCompressor(QByteArray *outArray, quint32 blockSize, int compressionRate, QObject *parent) :
	QCompressor(new QBuffer(outArray, this), blockSize, compressionRate, parent)
{}

QCompressor::~QCompressor()
{
	delete this->blockBuffer;
}

void QCompressor::addData(QByteArray data)
{
	QBuffer buff(&data);
	buff.open(QIODevice::ReadOnly);
	this->addData(&buff);
	buff.close();
}

void QCompressor::addData(QIODevice *device)
{
	while(device->bytesAvailable() > 0) {
		qint64 read = device->read((char*)(this->blockBuffer + this->bufferPos),
								   this->blockSize - this->bufferPos);
		if(read == -1) {
			this->allOk = false;
			return;
		}
		this->bufferPos += (quint32)read;

		if(this->bufferPos == this->blockSize)
			this->writeBlock();
	}
}

bool QCompressor::finish()
{
	this->writeBlock();
	return this->allOk;
}

void QCompressor::writeBlock()
{
	QByteArray compressed = qCompress(this->blockBuffer, (int)this->bufferPos, this->compressionRate);
	quint32 cSize = compressed.size();
	cSize = qToBigEndian(cSize);
	if(this->outDevice->write((char*)&cSize, sizeof(quint32)) == sizeof(quint32)) {
		if(this->outDevice->write(compressed) != compressed.size())
			this->allOk = false;
	} else
		this->allOk = false;
	this->bufferPos = 0;
}


QDecompressor::QDecompressor(QIODevice *outDevice, QObject *parent) :
	QObject(parent),
	outDevice(outDevice),
	blockBuffer(NULL),
	blockSize(0),
	blockSizeBuffer(),
	bufferPos(0),
	allOk(true)
{
	if(outDevice->isOpen()) {
		QIODevice::OpenMode mode = outDevice->openMode();
		if(!mode.testFlag(QIODevice::WriteOnly)) {
			outDevice->close();
			outDevice->open(mode | QIODevice::WriteOnly);
		}
	} else
		outDevice->open(QIODevice::WriteOnly);
}

QDecompressor::QDecompressor(QByteArray *outArray, QObject *parent) :
	QDecompressor(new QBuffer(outArray, this), parent)
{}

QDecompressor::~QDecompressor()
{
	delete this->blockBuffer;
}

void QDecompressor::addData(QByteArray data)
{
	QBuffer buff(&data);
	buff.open(QIODevice::ReadOnly);
	this->addData(&buff);
	buff.close();
}

void QDecompressor::addData(QIODevice *device)
{
	while(device->bytesAvailable() > 0) {
		if(this->blockSize == 0) {
			this->blockSizeBuffer += device->read(sizeof(quint32) -
												  this->blockSizeBuffer.size());
			if(this->blockSizeBuffer.size() == sizeof(quint32)) {
				this->blockSize = qFromBigEndian<quint32>((uchar*)this->blockSizeBuffer.data());
				this->blockSizeBuffer.clear();
				this->blockBuffer = new uchar[this->blockSize];
			} else
				return;
		}

		qint64 read = device->read((char*)(this->blockBuffer + this->bufferPos),
								   this->blockSize - this->bufferPos);
		if(read == -1) {
			this->allOk = false;
			return;
		}
		this->bufferPos += (quint32)read;

		if(this->bufferPos == this->blockSize)
			this->writeBlock();
	}
}

bool QDecompressor::finish()
{
	if(this->blockSize > 0)
		this->allOk = false;
	return this->allOk;
}

void QDecompressor::writeBlock()
{
	if(this->blockSize > 0) {
		QByteArray raw = qUncompress(this->blockBuffer, (int)this->bufferPos);
		if(raw.isEmpty())
			this->allOk = false;
		else {
			if(this->outDevice->write(raw) != raw.size())
				this->allOk = false;
		}
		delete this->blockBuffer;
		this->blockBuffer = NULL;
		this->blockSize = 0;
		this->bufferPos = 0;
	}
}
