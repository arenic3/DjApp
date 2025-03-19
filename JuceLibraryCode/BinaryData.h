/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   fader_vertical_png;
    const int            fader_vertical_pngSize = 1273656;

    extern const char*   fader_horizontal_png;
    const int            fader_horizontal_pngSize = 1540264;

    extern const char*   big_knob_png;
    const int            big_knob_pngSize = 7658752;

    extern const char*   medium_knob_png;
    const int            medium_knob_pngSize = 7683439;

    extern const char*   small_knob_png;
    const int            small_knob_pngSize = 6114029;

    extern const char*   bg2_png;
    const int            bg2_pngSize = 552379;

    extern const char*   slider3_png;
    const int            slider3_pngSize = 224189;

    extern const char*   play_png;
    const int            play_pngSize = 1303;

    extern const char*   icon_png;
    const int            icon_pngSize = 437428;

    extern const char*   knob2_png;
    const int            knob2_pngSize = 162692;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 10;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
