/******************************************************************************\

    CAMotics is an Open-Source simulation and CAM software.
    Copyright (C) 2011-2015 Joseph Coffland <joseph@cauldrondevelopment.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

\******************************************************************************/

#include "ClipperModule.h"

#include <cbang/json/JSON.h>

#include <clipper/clipper.hpp>

using namespace cb;
using namespace ClipperLib;
using namespace tplang;


void ClipperModule::define(js::Sink &exports) {
  exports.insert("offset(polys, delta, join, limit=1000, autoFix=true, "
                 "scale=1000000)", this, &ClipperModule::offsetCB);

  exports.insert("JOIN_SQUARE", jtSquare);
  exports.insert("JOIN_ROUND", jtRound);
  exports.insert("JOIN_MITER", jtMiter);
}


void ClipperModule::offsetCB(const JSON::Value &args, js::Sink &sink) {
  uint32_t scale = args.getU32("scale");

  // Convert JavaScript polys to Clipper polys
  Polygons polys;
  const JSON::Value &jsPolys = args.getList("polys");

  for (unsigned i = 0; i < jsPolys.size(); i++) {
    polys.push_back(Polygon());
    Polygon &poly = polys.back();
    const JSON::Value &jsPoly = jsPolys.getList(i);

    for (unsigned j = 0; j < jsPoly.size(); j++) {
      const JSON::Value &jsPoint = jsPoly.getList(j);

      if (jsPoint.size() != 2) THROWS("Expected 2D point");
      poly.push_back(IntPoint(jsPoint.getNumber(0) * scale,
                              jsPoint.getNumber(1) * scale));
    }
  }

  double delta = args.getNumber("delta") * scale;
  JoinType join = args.has("join") ? jtRound : (JoinType)args.getU32("join");
  double limit = args.getNumber("limit") * scale;
  bool autoFix = args.getBoolean("autoFix");

  OffsetPolygons(polys, polys, delta, join, limit, autoFix);

  // Convert Clipper result back to JavaScript
  sink.beginList();
  for (unsigned i = 0; i < polys.size(); i++) {
    Polygon &poly = polys[i];
    sink.appendList();

    for (unsigned j = 0; j < poly.size(); j++) {
      IntPoint &point = poly[j];

      sink.appendList();
      sink.append((double)point.X / scale);
      sink.append((double)point.Y / scale);
      sink.endList();
    }

    sink.endList();
  }

  sink.endList();
}
