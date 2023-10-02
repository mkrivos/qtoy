///
/// \package qtoy
///
/// \author Marian Krivos <marian.krivos@rsys.sk>
/// \date 10. 7. 2023 
/// \brief definicia typu
///
/// (C) Copyright 2023 R-SYS s.r.o
/// All rights reserved.
///

#include "msdfgen.h"
#include "msdfgen-ext.h"

using namespace msdfgen;

msdfgen::Bitmap<float, 3> generate(int ch, int w, int h)
{
    //           image width, height
    Bitmap<float, 3> msdf(w,h);

    FreetypeHandle* ft = initializeFreetype();
    if (ft)
    {
        FontHandle* font = loadFont(ft, "CascadiaMono.ttf");
        if (font)
        {
            Shape shape;
            if (loadGlyph(shape, font, ch))
            {
                shape.inverseYAxis = true;
                shape.normalize();
                //                      max. angle
                edgeColoringSimple(shape, 3.0);
                //                     range, scale, translation
                generateMSDF(msdf, shape, 4.0, 1.0, Vector2(4.0, 4.0));
                savePng(msdf, "output.png");
            }
            destroyFont(font);
        }
        deinitializeFreetype(ft);
    }
    return msdf;
}

