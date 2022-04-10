FROM epitechcontent/epitest-docker:devel

WORKDIR /arcade

COPY assets/ assets/
COPY cmake/ cmake/
COPY doc/ doc/
COPY src/ src/
COPY CMakeLists.txt .
