#include "GMPch.h"
#include "BoxBounds.h"

#include "BoundingBox.h"

namespace GM
{
	BoxBounds::BoxBounds(const Vector3& InOrigin, const Vector3& InExtents)
		: Origin(InOrigin), Extent(InExtents)
	{
	}

	BoxBounds::BoxBounds(const BoundingBox& Box)
	{
		Box.GetCenterAndExtent(Origin, Extent);
	}

	BoxBounds::BoxBounds(const std::vector<Vector3>& Points)
	{
		BoundingBox box(Points);
		box.GetCenterAndExtent(Origin, Extent);
	}
}