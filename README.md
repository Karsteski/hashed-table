# Hash Table Implementation in C

Simple hash table implementation in C for learning purposes, following [Ben Hoyt's blog post](https://benhoyt.com/writings/hash-table-in-c/).

Utilizes the [FNV-1a Hash Function](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash).

To build, the user must have the [Meson meta build system](https://mesonbuild.com/) installed as well as the [Ninja build system](https://ninja-build.org/).

Once those are on your system, it's as simple as running the following commands and executing the binary according to your OS :)

```bash

# From the project root folder

 > meson configure build
 > cd build
 > meson compile
 
# In the case of Linux
 
 > ./hashed-table-demo 

```
