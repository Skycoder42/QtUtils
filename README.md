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
