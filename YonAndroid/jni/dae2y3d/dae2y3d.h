#ifndef _DAE2Y3D_H_
#define _DAE2Y3D_H_

#include "vector3d.h"
#include "matrix4.h"
#include "quaternion.h"
#include "fastatof.h"

#include "CXMLReaderImpl.h"
#include "CReadFileStream.h"
#include "CWriteFileStream.h"

using namespace yon;
using namespace yon::core;
using namespace yon::io;

void dae2y3d(XMLReader* reader,IWriteStream* stream);

#endif