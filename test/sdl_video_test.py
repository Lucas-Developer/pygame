try:
    import pygame2.test.pgunittest as unittest
except:
    import pgunittest as unittest

import pygame2
import pygame2.sdl.video as video
import pygame2.sdl.constants as constants

class SDLVideoTest (unittest.TestCase):

    def test_pygame2_sdl_video_get_drivername(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.get_drivername:

        # get_drivername () -> str
        # 
        # Gets the name of the video driver.
        # 
        # Gets the name of the video driver or None, if the video system has
        # not been initialised or it could not be determined.
        video.init ()
        self.assert_ (video.get_drivername () != None)
        video.quit ()
        self.assert_ (video.get_drivername () == None)

    def todo_test_pygame2_sdl_video_get_gammaramp(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.get_gammaramp:

        # get_gammaramp () -> (int, int, ...), (int, int, ...), (int, int, ...)
        # 
        # Gets the color gamma lookup tables for the display.
        # 
        # Gets the color gamma lookup table for the display. This will
        # return three tuples for the red, green and blue gamma values. Each
        # tuple contains 256 values.

        self.fail() 

    def test_pygame2_sdl_video_get_info(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.get_info:

        # get_info () -> dict
        # 
        # Gets information about the video hardware.
        # 
        # Gets information about the video hardware. The returned dictionary
        # contains the following entries.
        # 
        # +------------------+---------------------------------------------+
        # | Entry            | Meaning                                     |
        # +==================+=============================================+
        # | hw_available     | Is it possible to create hardware surfaces? |
        # +------------------+---------------------------------------------+
        # | wm_available     | Is a window manager available?              |
        # +------------------+---------------------------------------------+
        # | blit_hw          | Are hardware to hardware blits accelerated? |
        # +------------------+---------------------------------------------+
        # | blit_hw_CC       | Are hardware to hardware colorkey blits     |
        # |                  | accelerated?                                |
        # +------------------+---------------------------------------------+
        # | blit_hw_A        | Are hardware to hardware alpha blits        |
        # |                  | accelerated?                                |
        # +------------------+---------------------------------------------+
        # | blit_sw          | Are software to hardware blits accelerated? |
        # +------------------+---------------------------------------------+
        # | blit_sw_CC       | Are software to hardware colorkey blits     |
        # |                  | accelerated?                                |
        # +------------------+---------------------------------------------+
        # | blit_sw_A        | Are software to hardware alpha blits        |
        # |                  | accelerated?                                |
        # +------------------+---------------------------------------------+
        # | blit_fill        | Are color fills accelerated?                |
        # +------------------+---------------------------------------------+
        # | video_mem        | Total amount of video memory in Kilobytes   |
        # +------------------+---------------------------------------------+
        # | vfmt             | Pixel format of the video device            |
        # +------------------+---------------------------------------------+
        self.assertRaises (pygame2.Error, video.get_info)
        video.init ()
        info = video.get_info ()
        self.assert_ (type (info) == dict)
        self.assert_ (type (info["hw_available"]) == bool)
        self.assert_ (type (info["wm_available"]) == bool)
        self.assert_ (type (info["blit_hw"]) == bool)
        self.assert_ (type (info["blit_hw_CC"]) == bool)
        self.assert_ (type (info["blit_hw_A"]) == bool)
        self.assert_ (type (info["blit_sw"]) == bool)
        self.assert_ (type (info["blit_sw_CC"]) == bool)
        self.assert_ (type (info["blit_sw_A"]) == bool)
        self.assert_ (type (info["blit_fill"]) == bool)
        self.assert_ (type (info["video_mem"]) == int)
        self.assert_ (type (info["vfmt"]) == video.PixelFormat)
        video.quit ()

    def test_pygame2_sdl_video_get_videosurface(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.get_videosurface:

        # get_videosurface () -> Surface
        # 
        # Gets the current display surface or None, if there is no such Surface.
        self.assertRaises (pygame2.Error, video.get_videosurface)
        video.init ()
        self.assert_ (video.get_videosurface () == None)
        video.quit ()
        self.assertRaises (pygame2.Error, video.get_videosurface)
        video.init ()
        video.set_mode (1, 1)
        self.assert_ (type (video.get_videosurface ()) == video.Surface)
        video.quit ()
        self.assertRaises (pygame2.Error, video.get_videosurface)

    def test_pygame2_sdl_video_init(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.init:

        # init () -> None
        # 
        # Initializes the video subsystem of the SDL library.
        self.assertEquals (video.init (), None)
        video.quit ()

    def todo_test_pygame2_sdl_video_is_mode_ok(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.is_mode_ok:

        # is_mode_ok (width, height[, bpp, flags]) -> bool
        # is_mode_ok (size[, bpp, flags]) -> bool
        # 
        # Checks, whether the requested video mode is supported.
        # 
        # Checks, whether the video mode is supported for the passed size,
        # bit depth and flags. If the bit depth (bpp) argument is omitted,
        # the current screen bit depth will be used.
        # 
        # The optional flags argument is the same as for set_mode.

        self.fail() 

    def test_pygame2_sdl_video_list_modes(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.list_modes:

        # list_modes ([, format, flags]) -> [rect, rect, ...]
        # 
        # Returns the supported modes for a specific format and flags.
        # 
        # Returns the supported modes for a specific format and flags.
        # The optional format argument must be a PixelFormat
        # instance with the desired mode information. The optional flags
        # argument is the same as for set_mode.
        # 
        # If both, the format and flags are omitted, all supported screen
        # resolutions for all supported formats and flags are returned.
        self.assertRaises (pygame2.Error, video.list_modes)
        video.init ()
        modes = video.list_modes()
        self.assert_ (type (modes) == list)
        for r in modes:
            self.assert_ (type (r) == pygame2.Rect)
        video.quit ()
        self.assertRaises (pygame2.Error, video.list_modes)

    def test_pygame2_sdl_video_quit(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.quit:

        # quit () -> None
        # 
        # Shuts down the video subsystem of the SDL library.
        # 
        # After calling this function, you should not invoke any class,
        # method or function related to the video subsystem as they are
        # likely to fail or might give unpredictable results.
        self.assertEquals (video.quit (), None)

    def todo_test_pygame2_sdl_video_set_gamma(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.set_gamma:

        # set_gamma (red, green, blue) -> None
        # 
        # Sets the gamma values for all three color channels.
        # 
        # Sets the gamma values for all three color channels. In case
        # adjusting the gamma is not supported, an exception will be raised.

        self.fail() 

    def todo_test_pygame2_sdl_video_set_gammaramp(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.set_gammaramp:

        # set_gammaramp (redtable, greentable, bluetable) -> None
        # 
        # Sets the color gamma lookup tables for the display.
        # 
        # Sets the color gamma lookup table for the display. The three
        # arguments must be sequences with 256 integer value enties for the
        # gamma ramps.

        self.fail() 

    def todo_test_pygame2_sdl_video_set_mode(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.set_mode:

        # set_mode (width, height[, bpp, flags]) -> Surface
        # set_mode (size[, bpp, flags]) -> Surface
        # 
        # Creates the main display Surface.
        # 
        # Creates the main display Surface using the specified size, bit
        # depth and flags. If the bit depth (bpp) argument is omitted, the
        # current screen bit depth will be used.
        # 
        # The optional flags argument can be a bitwise OR'd combination of
        # the constants below.
        # 
        # +-------------------+-----------------------------------------------+
        # | Flag              | Description                                   |
        # +===================+===============================================+
        # | SWSURFACE         | Create the video surface in system memory     |
        # +-------------------+-----------------------------------------------+
        # | HWSURFACE         | Create the video surface in video memory      |
        # +-------------------+-----------------------------------------------+
        # | ASYNCBLIT         | Enables the use of asynchronous updates of    |
        # |                   | the display surface. This will usually slow   |
        # |                   | down blitting on single CPU machines, but may |
        # |                   | provide a speed increase on SMP systems.      |
        # +-------------------+-----------------------------------------------+
        # | ANYFORMAT         | Normally, if a video surface of the requested |
        # |                   | bits-per-pixel (bpp) is not available, SDL    |
        # |                   | will emulate one with a shadow surface.       |
        # |                   | Passing ANYFORMAT prevents this and causes    |
        # |                   | SDL to use the video surface, regardless of   |
        # |                   | its pixel depth.                              |
        # +-------------------+-----------------------------------------------+
        # | HWPALETTE         | Give SDL exclusive palette access. Without    |
        # |                   | this flag you may not always get the the      |
        # |                   | colors you request with                       |
        # |                   |        Surface.set_colors  or                 |
        # |                   |        Surface.set_palette .                  |
        # +-------------------+-----------------------------------------------+
        # | DOUBLEBUF         | Enable hardware double buffering; only valid  |
        # |                   | with HWSURFACE. Calling        Surface.flip   |
        # |                   | will flip the buffers and update the screen.  |
        # |                   | All drawing will take place on the surface    |
        # |                   | that is not displayed at the moment. If       |
        # |                   | double buffering could not be enabled then    |
        # |                   |        Surface.flip  will just perform a      |
        # |                   |        Surface.update  on the entire screen.  |
        # +-------------------+-----------------------------------------------+
        # | FULLSCREEN        | SDL will attempt to use a fullscreen mode. If |
        # |                   | a hardware resolution change is not possible  |
        # |                   | (for whatever reason), the next higher        |
        # |                   | resolution will be used and the display       |
        # |                   | window centered on a black background.        |
        # +-------------------+-----------------------------------------------+
        # | OPENGL            | Create an OpenGL rendering context. You       |
        # |                   | should have previously set OpenGL video       |
        # |                   | attributes with                               |
        # |                   |        pygame2.sdl.gl.set_attribute .         |
        # +-------------------+-----------------------------------------------+
        # | OPENGLBLIT        | Create an OpenGL rendering context, like      |
        # |                   | above, but allow normal blitting operations.  |
        # |                   | The screen (2D) surface may have an alpha     |
        # |                   | channel, and        Surface.update  must be   |
        # |                   | used for updating changes to the screen       |
        # |                   | surface. NOTE: This option is kept for        |
        # |                   | compatibility only, and is not recommended    |
        # |                   | for new code.                                 |
        # +-------------------+-----------------------------------------------+
        # | RESIZABLE         | Create a resizable window. When the window is |
        # |                   | resized by the user a VIDEORESIZE event is    |
        # |                   | generated and        set_mode  can be called  |
        # |                   | again with the new size.                      |
        # +-------------------+-----------------------------------------------+
        # | NOFRAME           | If possible, NOFRAME causes SDL to create a   |
        # |                   | window with no title bar or frame decoration. |
        # |                   | Fullscreen modes automatically have this flag |
        # |                   | set.                                          |
        # +-------------------+-----------------------------------------------+

        self.fail() 

    def test_pygame2_sdl_video_was_init(self):

        # __doc__ (as of 2009-05-31) for pygame2.sdl.video.was_init:

        # was_init () -> bool
        # 
        # Returns, whether the video subsystem of the SDL library is
        # initialized.
        video.quit ()
        self.assertEquals (video.was_init (), False)
        video.init ()
        self.assertEquals (video.was_init (), True)
        video.quit ()
        self.assertEquals (video.was_init (), False)

if __name__ == "__main__":
    unittest.main ()