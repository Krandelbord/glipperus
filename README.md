# glipperus
glipperus is lightwieght clipboard manager.
# Features
 - It keeps track of your copy/paste and selections history.
 - You can access history by clicking on tray-icon or by pressing ctrl+alt+V in any application.

# Screenshots
![Screenshot animation](https://user-images.githubusercontent.com/8105637/29374102-93cfaae4-82b0-11e7-8ae0-b86b658e1d80.gif)
# Installation
On ubuntu/debian you can build package by running:

$ dpkg-buildpackage -b -uc 

and then install package:

$ sudo dpkg -i dpkg -i glipperus_<version>.deb

On other distros see below:

# Compilation
To compile you will need C compiler, and gtk-dev package.
Simply type make
