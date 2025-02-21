/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   play_png;
    const int            play_pngSize = 1303;

    extern const char*   Bg_png;
    const int            Bg_pngSize = 179371;

    extern const char*   icon_png;
    const int            icon_pngSize = 437428;

    extern const char*   slider2_png;
    const int            slider2_pngSize = 227843;

    extern const char*   slider1_png;
    const int            slider1_pngSize = 228548;

    extern const char*   knob1_png;
    const int            knob1_pngSize = 38009;

    extern const char*   knob2_png;
    const int            knob2_pngSize = 162692;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

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
