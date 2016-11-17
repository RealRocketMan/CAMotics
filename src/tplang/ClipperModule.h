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

#ifndef TPLANG_CLIPPER_MODULE_H
#define TPLANG_CLIPPER_MODULE_H

#include <cbang/js/Module.h>


namespace tplang {
  class ClipperModule : public cb::js::Module {
  public:
    ClipperModule() {}

    // From cb::js::Module
    const char *getName() const {return "clipper";}
    void define(cb::js::Sink &exports);

    // Javascript call backs
    void offsetCB(const cb::JSON::Value &args, cb::js::Sink &sink);
  };
}

#endif // TPLANG_CLIPPER_MODULE_H
