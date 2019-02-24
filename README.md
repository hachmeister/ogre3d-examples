# Ogre3D examples

### Building

```bash
git clone https://github.com/hachmeister/ogre3d-examples.git
cd ogre3d-examples
mkdir build
cd build
cmake -DOGRE_PREFIX_PATH=<path-to-ogre-dir> ..
make
ln -s ../assets .
./cube/cube
```
