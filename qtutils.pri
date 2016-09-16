include(./QPropertySettings/qpropertysettings.pri)
include(./QElementModel/qelementmodel.pri)
include(./QCompressor/qcompressor.pri)

contains(QT, widgets) {
	include(./DialogMaster/dialogmaster.pri)
	include(./QFlowLayout/qflowlayout.pri)
	include(./QProgressGroup/qprogressgroup.pri)
}

contains(QT, network) {
	include(./QSslServer/qsslserver.pri)
}
