This project contains a small header only C++ library of helpful utilities.

## How to use this library
As it is a header only library, you only need to add the headers from the "src/utils_h" folder to your project.

## How to develop this library
```
docker build --tag utilsh:latest tools/ci/docker
docker run --tty --interactive --publish 43265:22 --mount type=bind,source="<path>/utils_h",target="/project" utilsh:latest
./build.sh
```
