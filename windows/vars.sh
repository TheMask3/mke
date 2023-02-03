DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

MKE_HOST=$(gcc -dumpmachine)
MKE_PREFIX=$(ruby -e "printf ('${MKE_HOST}'[/i686/].nil?) ? 'mingw64' : 'mingw'")
export LDFLAGS="-L$DIR/build-${MKE_PREFIX}/lib -L$DIR/build-${MKE_PREFIX}/bin"
export CFLAGS="-I$DIR/build-${MKE_PREFIX}/include"
export PATH="$DIR/build-${MKE_PREFIX}/bin:$PATH"
MKE_OLD_PC=$(pkg-config --variable pc_path pkg-config)

# Try to load the stuff we built first
export PKG_CONFIG_PATH="$DIR/build-${MKE_PREFIX}/lib/pkgconfig"
export MKE_PREFIX="$DIR/build-${MKE_PREFIX}"