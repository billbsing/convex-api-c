convex-api-c


## Building this library

First run autoreconf

```bash
autoreconf --install
```

Then run the configure script to make sure all the support libraries are ready

```bash
./configure
```

Then make the library

```bash
make
```

Test the library, you need to do the following:

```bash
make check
```

