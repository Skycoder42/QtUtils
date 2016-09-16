# QtUtils
A collection of various Qt-Classes, branch-sorted.

## Projekt structure
The utils are simply a collection of various helpful Qt-Classes. Whats special about this project is the way they are organized:
Every part of thus library has it's own branch. All those branches are based on the "Base" branch. The master branch combines them all and provides a simple library projekt to include. The library branch contains the compiled library for all platforms (may not always be up to date!)

This structure has a great advantage: If you want to use the whole library, or don't really care, because you can use a library, you can use the master branch (or the library branch). However, if you only want to use one specific module of the utils, simply use this repository as submodule and checkout the correpsonding branch. It will only contain this readme, the license and the single subfolder with the module that you require.

## Example
The DialogMaster is one of the various modules of the library. If you need this one, you have the following choices:
 1. Use the QtUtils.lib provided in the releases. It includes all modules, including the DialogMaster
 2. Add a submodule to your project and checkout the "DialogMaster" branch. This way you will only have the dialog master in your projekt
 3. Same as 2, but with the master branch, if you need all of the modules.
 4. Same as 3, but use the library branch. This way you don't have to compile the utils. Note: 3 and 4 are compatible. You can easily switch between both of them. If you added the library via the `.pri` file, nothing will change from the outside.
 5. Build it yourself. This is, of course, always an option ;)

## Modules
### DialogMaster
The dialog master provides advanced dialogs in Qt, automatically handling modality, window icons resizing and more!

### QPropertySettings
The property settings are an extension for QSettings to autmatically store properties of an object in the settings.

### QElementModel
The element model provides a object-based list model with dynamic roles to be used with QML Views.
### QCompressor
A class to compress data of any size, by splitting it into chucks that Qt's build in compression algorithms can handel. This is NOT a Zip-library. It simply compresses binary data, without any metadata or compability with other tools (but cross-plattform, i.e. data can be compressed and extracted on different devices)

### QFlowLayout
The Flow layout from Qts flow layout example, with soem adjustments and improvements. for the original, see here: https://doc.qt.io/qt-5/qtwidgets-layouts-flowlayout-example.html.
As the same suggests, it's a layout implementation to organize elements in a dynamic flow.

### QProgressGroup
A class to combine multiple kinds of progress indicators, and manage them with a combined interface. Example: YOu have a normal progressbar and a taskbar progress. Thanks to this class, you can simply add both to a progress group, and everytime you update the progress group, both bars will be updated accordingly.

Currently supported kinds of progress elements are:
- [QProgressBar](https://doc.qt.io/qt-5/qprogressbar.html)
- [QSystemTrayIcon](https://doc.qt.io/qt-5/qsystemtrayicon.html) (With some additional code to make the tray show a busy indicator)
- [QWinTaskbarProgress](https://doc.qt.io/qt-5/qwintaskbarprogress.html)

### QSslServer
The QSslServer is an extension of the [QTcpServer](https://doc.qt.io/qt-5/qtcpserver.html) to provide a server that can handle ssl connections. It works just like the tcp version, but with ssl, and uses the [QSslSocket](https://doc.qt.io/qt-5/qsslsocket.html) for connections.
