# Configuring a MacBook for OpenGL

These instructions are from Andrew Danner's experience setting up OpenGL on his MacBook Pro running OS X El Capitan (10.11.6).
Comments, issues, and fixes welcome. You can file an issue directly on the [github page](https://github.swarthmore.edu/CS40-F16/examples/issues). 

# Basic, Non OpenGL code setup

 * try to run git, install XCode from popup
   * run `git --help` as sudo to accept XCode EULA

 * (OPTIONAL) download GitHub Desktop
  * install command line tools
  * add github.com, https://github.swarthmore.edu

 * (OPTIONAL) Install [XQuartz](https://www.xquartz.org/)
  * log out to restart X, test with ssh -Y

 * (OPTIONAL) Install [gitX-dev](https://rowanj.github.io/gitx/)
  * install terminal support when asked
  
 * Either copy ssh-keys from the CS network or generate new keys on your mac and add the new key to github.swarthmore.edu.  
   See [git setup](https://www.cs.swarthmore.edu/~adanner/cs40/f16/git-setup.php) for details. 
 
* Install [CMake >= 3.6.0](https://cmake.org/download/)
  * allow command line usage with `sudo /Applications/CMake.app/Contents/bin/cmake-gui --install=/usr/local/bin/`
  * Above fix suggested from [StackOverflow](http://stackoverflow.com/a/32907600)

* Install [Qt Open Source](https://www.qt.io/download-open-source/)
   - You may need to configure cmake manually. 
     See [StackOverflow](http://stackoverflow.com/questions/37737066/qt-creator-wont-open-cmake-c-project-in-osx)
     for details
   
* Install GLEW
  - download glew-2.0.0.tgz from [http://glew.sourceforge.net/](https://www.qt.io/download-open-source/)
  - `tar xzvf glew-2.0.0.tgz`
  - `cd glew-2.0.0 && make && sudo make install`
 
* Install glfw
  - `git clone https://github.com/glfw/glfw.git`
  - `cd glfw`
  - `mkdir build && cd build && cmake -DBUILD_SHARED_LIBS=ON ..`
  - `make -j8 && sudo make install`

* Install glm
  - `git clone https://github.com/g-truc/glm.git`
  - `cd glm`
  - `mkdir build && cd build && cmake ..`
  - `make -j8 && make sudo install`

That should be it. I was at least able to compile and run the w01-samples from the command line or within QtCreator.
If you do your development on OSX, be sure to test that your code works on the labs machines before the submission
deadline. 



