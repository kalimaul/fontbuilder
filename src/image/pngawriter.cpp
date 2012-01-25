/**
 * Copyright (c) 2010-2010 Andrey AndryBlack Kunitsyn
 * email:support.andryblack@gmail.com
 *
 * Report bugs and download new versions at http://code.google.com/p/fontbuilder
 *
 * This software is distributed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "pngawriter.h"

#include "builtinimagewriter.h"
#include "layoutdata.h"
#include <QImage>
#include <QPainter>
#include "../layoutconfig.h"
#include <png++/png.hpp>

#include <QDebug>

PNGAImageWriter::PNGAImageWriter(QString ext,QObject *parent) :
    AbstractImageWriter(parent)
{
    setExtension(ext);
    setReloadSupport(false);
}


bool PNGAImageWriter::Export(QFile& file) {
    QImage pixmap = buildImage();
    assert(pixmap.isGrayscale());
    png::image<png::gray_pixel> image(pixmap.width(), pixmap.height());
    for (size_t y = 0; y < image.get_height(); ++y)
    {
        for (size_t x = 0; x < image.get_width(); ++x)
        {
            QRgb pix = pixmap.pixel(x, y);
            assert(qIsGray(pix));
            image[y][x] = png::gray_pixel(qAlpha(pix));
        }
    }
    image.write(file.fileName().toStdString());
    return true;
}
