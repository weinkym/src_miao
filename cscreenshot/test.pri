
LIB_DIR=$$PWD/lib
OUTPUT_DIR=$$PWD/TEMP

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
    DESTDIR = $$LIB_DIR/debug
    DLLDESTDIR = $$LIB_DIR/debug

    OBJECTS_DIR = $$OUTPUT_DIR/debug/cscreenshot/obj
    MOC_DIR = $$OUTPUT_DIR/debug/cscreenshot/moc
    UI_DIR = $$OUTPUT_DIR/debug/cscreenshot/ui
    RCC_DIR = $$OUTPUT_DIR/debug/cscreenshot/rcc
}
CONFIG(release, debug|release) {
    DESTDIR = $$LIB_DIR/release
    DLLDESTDIR = $$LIB_DIR/release
    OBJECTS_DIR = $$OUTPUT_DIR/release/cscreenshot/obj
    MOC_DIR = $$OUTPUT_DIR/release/cscreenshot/moc
    UI_DIR = $$OUTPUT_DIR/release/cscreenshot/ui
    RCC_DIR = $$OUTPUT_DIR/release/cscreenshot/rcc
}
