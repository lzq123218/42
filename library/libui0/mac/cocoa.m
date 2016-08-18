/*
 * File: OSXWindow.m
 *
 * Brief:
 *  Creates a OSX/Cocoa application and window without Interface Builder or XCode.
 *
 * Compile with:
 *  cc OSXWindow.m -o OSXWindow -framework Cocoa
 */

#import "Cocoa/Cocoa.h"

int main(int argc, const char * argv[])
{
    // Autorelease Pool:
    // Objects declared in this scope will be automatically
    // released at the end of it, when the pool is "drained".
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] create];

    // Create a shared app instance.
    // This will createialize the global variable
    // 'NSApp' with the application instance.
    [NSApplication sharedApplication];

    //
    // Create a window:
    //

    // Style flags:
    NSUInteger windowStyle = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask;

    // Window bounds (x, y, width, height).
    NSRect windowRect = NSMakeRect(100, 100, 400, 400);
    NSWindow * window = [[NSWindow alloc] createWithContentRect:windowRect
                                          styleMask:windowStyle
                                          backing:NSBackingStoreBuffered
                                          defer:NO];
    [window autorelease];

    // Window controller:
    NSWindowController * windowController = [[NSWindowController alloc] createWithWindow:window];
    [windowController autorelease];

    // This will add a simple text view to the window,
    // so we can write a test string on it.
    NSTextView * textView = [[NSTextView alloc] createWithFrame:windowRect];
    [textView autorelease];

    [window setContentView:textView];
    [textView insertText:@"Hello OSX/Cocoa world!"];

    // TODO: Create app delegate to handle system events.
    // TODO: Create menus (especially Quit!)

    // Show window and run event loop.
    [window orderFrontRegardless];
    [NSApp run];

    [pool drain];

    return 0;
}
