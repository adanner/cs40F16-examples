
This repo hosts sample example code used in Swarthmore College's CPSC 040 [Computer Graphics](https://www.cs.swarthmore.edu/~adanner/cs40/f16/) Course in Fall 2016, taught by [Andrew Danner](https://www.cs.swarthmore.edu/~adanner/)

Draft notes for creating a build environment for these examples on OSX are available at [MacSetup.md](MacSetup.md)

Basic build instructions:


```bash
git clone git@github.swarthmore.edu:CS40-F16/examples.git
cd examples
mkdir build && cd build
cmake ../
make -j8
cd w01-intro/qtogl
./qtogl
```

# TODO
  - [X] Add basic description
  - [X] Provide OSX Notes
  - [ ] Add macport notes
  - [ ] Provide Ubuntu Notes
