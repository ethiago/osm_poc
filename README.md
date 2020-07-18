# C++ OSMesa proove of concept

This repository is intended to serve as a starting point for working with off-screen rendering when using OSMesa. It is a simple C ++ application that renders a triangle in an application memory block using OpenGL and saves the result as a png.
It can be built with docker, so just do:
`docker build -t osm_poc .`
It will build and run the application. A `output.png` will be generated inside the image and can be retrieved with:
```
$ CHASH=`docker create osm_poc`
$ docker cp ${CHASH}:/app/output.png .
$ docker rm $CHASH
```
If you want to build without Docker, read the Dockerfile to learn the dependencies, how to build an how to run.
