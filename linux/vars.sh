DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

MKE_PREFIX=$(uname -m)
export LDFLAGS="-L$DIR/build-${MKE_PREFIX}/lib"
export CFLAGS="-I$DIR/build-${MKE_PREFIX}/include"
MKE_OLD_PC=$(pkg-config --variable pc_path pkg-config)

# Try to load the stuff we built first
export PKG_CONFIG_LIBDIR="$DIR/build-${MKE_PREFIX}/lib/pkgconfig:$DIR/build-${MKE_PREFIX}/lib64/pkgconfig:${MKE_OLD_PC}"
export PATH="$DIR/build-${MKE_PREFIX}/bin:$PATH"
export LD_LIBRARY_PATH="$DIR/build-${MKE_PREFIX}/lib:${LD_LIBRARY_PATH}"
export MKE_PREFIX="$DIR/build-${MKE_PREFIX}"