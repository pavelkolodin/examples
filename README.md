Examples of using fir
* httpserver - tiny HTTP server displaying your User-Agent using Fir's HTML templater

####Checkout
```
git clone git@github.com:pavelkolodin/examples.git
cd examples
git submodule update --init --recursive
```

####Build

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```

####Run
##### httpserver
```
./httpserver 127.0.0.1 44333 ../httpserver
```
(the last argument points to a directory where **index.html** template is placed)<br>
Open [http://127.0.0.1:44333/](http://127.0.0.1:44333/) in browser.

