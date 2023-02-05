physfs = dependency('physfs', version: '>=2.1', static: build_static)
openal = dependency('openal', static: build_static, method: 'pkg-config')
theora = dependency('theora', static: build_static)
vorbisfile = dependency('vorbisfile', static: build_static)
vorbis = dependency('vorbis', static: build_static)
ogg = dependency('ogg', static: build_static)
sdl2 = dependency('SDL2', static: build_static)
sdl_sound = compilers['cpp'].find_library('SDL2_sound')
sdl2_ttf = dependency('SDL2_ttf', static: build_static)
freetype = dependency('freetype2', static: build_static)
sdl2_image = dependency('SDL2_image', static: build_static)
pixman = dependency('pixman-1', static: build_static)
png = dependency('libpng', static: build_static)
jpeg = dependency('libjpeg', static: build_static)
zlib = dependency('zlib', static: build_static)
uchardet = dependency('uchardet', static: build_static)
# Add this later when we can replace theora with dav1d
# dav1d = dependancy('dav1d', static: build_static)

global_args += '-DIMGUI_IMPL_OPENGL3'

if host_system == 'windows'
    bz2 = dependency('bzip2', static: build_static)
    iconv = compilers['cpp'].find_library('iconv', static: build_static)
else
    bz2 = compilers['cpp'].find_library('bz2')
    # FIXME: Specifically asking for static doesn't work if iconv isn't
    # installed in the system prefix somewhere
    iconv = compilers['cpp'].find_library('iconv')
    global_dependencies += compilers['cpp'].find_library('charset')
endif

# If OpenSSL is present, you get HTTPS support
if get_option('enable-https') == true
    openssl = dependency('openssl', required: false, static: build_static)
    if openssl.found() == true
        global_dependencies += openssl
        global_args += '-DMKE_SSL'
        if host_system == 'windows'
            global_link_args += '-lcrypt32'
        endif
    else
        warning('Could not locate OpenSSL. HTTPS will be disabled.')
    endif
endif

# Windows needs to be treated like a special needs child here
explicit_libs = ''
if host_system == 'windows'
    # Newer versions of Ruby will refuse to link without these
    explicit_libs += 'libmsvcrt;libgcc;libmingwex;libgmp;'
endif
if build_static == true
    if host_system == 'windows'
        global_link_args += ['-Wl,-Bstatic', '-lgcc', '-lstdc++', '-lpthread', '-Wl,-Bdynamic']
    else
        global_link_args += ['-static-libgcc', '-static-libstdc++']
    endif
    global_args += '-DAL_LIBTYPE_STATIC'
endif

# Disable this when compilation succeed
# global_link_args += ['-w']

foreach l : explicit_libs.split(';')
        if l != ''
            global_link_args += '-l:' + l + '.a'
        endif
endforeach

alcdev_struct = 'ALCdevice_struct'
if openal.type_name() == 'pkgconfig'
    if openal.version().version_compare('>=1.20.1')
        alcdev_struct = 'ALCdevice'
    endif
endif

global_args += '-DMKE_ALCDEVICE=' + alcdev_struct


global_include_dirs += include_directories('.',
    'filesystem', 'filesystem/ghc',
    'glm',
    'sdl_opengl',
    'libnsgif', 'libnsgif/utils',
    'reactphysics3d',
    'sigslot', 'sigslot/adapter',
    'theoraplay'
)

global_dependencies += [openal, zlib, bz2, sdl2, sdl_sound, pixman, physfs, theora, vorbisfile, vorbis, ogg, sdl2_ttf, freetype, sdl2_image, png, jpeg, iconv, uchardet]
if host_system == 'windows'
    global_dependencies += compilers['cpp'].find_library('wsock32')
endif

if get_option('shared_fluid') == true
    fluidsynth = dependency('fluidsynth', static: build_static)
    add_project_arguments('-DSHARED_FLUID', language: 'cpp')
    global_dependencies += fluidsynth
    if host_system == 'windows'
        global_dependencies += compilers['cpp'].find_library('dsound')
    endif
endif

if get_option('cjk_fallback_font') == true
    add_project_arguments('-DMKE_CJK_FONT', language: 'cpp')
endif

main_source = files(
    'main.cpp',
    'config.cpp',
    'eventthread.cpp',
    'settingsmenu.cpp',
    'sharedstate.cpp',
    
    'alstream.cpp',
    'audio.cpp',
    'audiostream.cpp',
    'fluid-fun.cpp',
    'midisource.cpp',
    'sdlsoundsource.cpp',
    'soundemitter.cpp',
    'vorbissource.cpp',
    'theoraplay/theoraplay.c',
    
    'rgssad.cpp',
    
    'autotiles.cpp',
    'autotilesvx.cpp',
    'bitmap.cpp',
    'font.cpp',
    'graphics.cpp',
    'plane.cpp',
    'sprite.cpp',
    'tilemap.cpp',
    'tilemapvx.cpp',
    'viewport.cpp',
    'window.cpp',
    'windowvx.cpp',
    
    'libnsgif/libnsgif.c',
    'libnsgif/lzw.c',
    
    'gl-debug.cpp',
    'gl-fun.cpp',
    'gl-meta.cpp',
    'glstate.cpp',
    'scene.cpp',
    'shader.cpp',
    'texpool.cpp',
    'tileatlas.cpp',
    'tileatlasvx.cpp',
    'tilequad.cpp',
    'vertex.cpp',
    
    'easylogging++.cpp',
    'iniconfig.cpp',
    'win-consoleutils.cpp',
    
    'etc.cpp',
    'table.cpp',
    
    'filesystem/filesystem.cpp',
    'filesystem/filesystemImpl.cpp',
    
    'input.cpp',
    'keybindings.cpp',
    
    'LUrlParser.cpp',
    'net.cpp',
    
    'systemImpl.cpp',
    
    'binding-mri.cpp',
    'binding-util.cpp',
    'binding-table.cpp',
    'binding-etc.cpp',
    'binding-bitmap.cpp',
    'binding-font.cpp',
    'binding-graphics.cpp',
    'binding-input.cpp',
    'binding-sprite.cpp',
    'binding-viewport.cpp',
    'binding-plane.cpp',
    'binding-window.cpp',
    'binding-tilemap.cpp',
    'binding-audio.cpp',
    'binding-filesystem.cpp',
    'binding-windowvx.cpp',
    'binding-tilemapvx.cpp',
    'binding-http.cpp',
    'binding-tinygltf.cpp',
    
    'module_rpg.cpp'
)

if steamworks == true
    main_source += files('binding-cusl.cpp')
endif

if miniffi == true
    main_source += files('binding-miniffi.cpp')
    main_source += files('miniffi.cpp')
endif

global_sources += main_source

if get_option('mri_includes') == ''
    ver = get_option('mri_version')
    if ver.version_compare('<=1.8')
        global_args += '-DMKE_LEGACY_RUBY'
    elif ver.version_compare('>=3.0') and compilers['cpp'].get_id() == 'clang'
        global_args += '-fdeclspec'
    endif
    global_dependencies += dependency('ruby-' + ver)
else
    global_args += ('-I' + get_option('mri_includes'))
    global_dependencies += compilers['cpp'].find_library(get_option('mri_library'), dirs: get_option('mri_libpath'))
endif
