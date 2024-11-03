/// Terminal emulator overview (on Win32):
///
/// - Start ConPTY session, setup pipes, start a shell process
///
/// - Create virtual screen buffer
///   (ScreenCell buffer[width_in_char][height_in_char])
///
/// - Read ConPTY output (async?)
///
/// - Write ConPTY input (async?)
///
/// - Render screen buffer (OpenGL)
///
/// Considerations:
///
/// - Configuration (lua/toml/etc)
/// - Resizing
/// - Scrolling
/// - Mouse click support
/// - Line wrapping via renderer

#include <stdio.h>

int
main(int argc, char **argv)
{
    printf("Hello world!");

    return 0;
}
