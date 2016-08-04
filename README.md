# QtUtils
A collection of various Qt-Classes, branch-sorted.

## Projekt structure
The utils are simply a collection of various helpful Qt-Classes. Whats special about them is the way they are organized:
Every part of thus library has it's own branch. All those branches are based on the "Base" branch. The master branch combines them all and provides a simple library projekt to include.

This structure has a great advantage: If you want to use the whole library, or don't really care, because you can use a library, you can use the master branch. However, if you only want to use one specific module of the utils, simply use this repository as submodule and checkout the correpsonding branch. It will only contain this readme, the license and the single subfolder with the module that you require.

## Example
The DialogMaster is one of the various modules of the library. If you need this one, you have the following choices:
 1. Use the QtUtils.lib provided in the realeases. It includes all modules, including the DialogMaster
 2. Add a submodule to your project and checkout the "DialogMaster" branch. This way you will only have the dialog master in your projekt
 3. Same as 2, but with the master branch, if you need all of the modules

## Modules
### DialogMaster
The dialog master provides advanced dialogs in Qt, automatically handling modality, window icons resizing and more!
