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

#include "simplexmlexporter.h"
#include "../fontconfig.h"
#include <QtXml>
#include <cassert>

SimpleXMLExporter::SimpleXMLExporter(QObject *parent) :
    AbstractExporter(parent)
{
    setExtension("xml");
}

static QDomElement text(QDomDocument& doc, const QString& elem_name, const QString& data) {
  QDomText t = doc.createTextNode(data);
  QDomElement elem = doc.createElement(elem_name);
  elem.appendChild(t);
  return elem;
}

bool SimpleXMLExporter::Export(QByteArray& out) {
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"" ));
    QDomElement root = doc.createElement("Font");
    doc.appendChild(root);
    root.appendChild(text(doc, "size", QString::number(fontConfig()->size())));
    root.appendChild(text(doc, "height", QString::number(metrics().height)));

    int offset = metrics().ascender;

    foreach (const Symbol& c , symbols()) {
        QDomElement ce = doc.createElement("Char");
        ce.appendChild(text(doc, "code", QString::number(c.id)));
        {
            QDomElement place = doc.createElement("place");
            place.appendChild(text(doc, "x", QString::number(c.placeX)));
            place.appendChild(text(doc, "y", QString::number(c.placeY)));
            ce.appendChild(place);
        }

        {
            QDomElement dim = doc.createElement("dimensions");
            dim.appendChild(text(doc, "x", QString::number(c.placeW)));
            dim.appendChild(text(doc, "y", QString::number(c.placeH)));
            ce.appendChild(dim);
        }

        {
            QDomElement rect = doc.createElement("rect");
            rect.appendChild(text(doc, "x", QString::number(c.offsetX)));
            rect.appendChild(text(doc, "y", QString::number(offset - c.offsetY)));
            ce.appendChild(rect);
        }

        ce.appendChild(text(doc, "width", QString::number(c.advance)));

        assert(c.kerning.begin() == c.kerning.end());

        root.appendChild(ce);
    }
    out = doc.toByteArray(1);


    return true;
}


AbstractExporter* SimpleXMLExporterFactoryFunc (QObject* parent) {
    return new SimpleXMLExporter(parent);
}
