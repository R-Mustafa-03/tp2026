#ifndefCOMPOSITESHAPE
#defineCOMPOSITESHAPE
#include"shape.h"
#include<vector>
#include<memory>

classCompositeShape:publicShape
{
private:
std::vector<std::unique_ptr<Shape>>shapes;

public:
voidaddShape(std::unique_ptr<Shape>shape);

doublegetArea()constoverride;
PointgetCenter()constoverride;

voidmove(doubledx,doubledy)override;
voidscale(doublefactor)override;

std::stringgetName()constoverride;
};
#endif
