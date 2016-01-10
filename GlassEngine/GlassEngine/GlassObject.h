#if !defined GLASS_OBJECT
#define GLASS_OBJECT

#include "GlassMaths.h"
#include "HAPI/HAPI_InputCodes.h"
#include <HAPI_lib.h>

class GlassObject
{
public:
	GlassObject(){};
	virtual ~GlassObject(){};
	
	Vector3 positon = Vector3(0,0,0);
};


#endif
