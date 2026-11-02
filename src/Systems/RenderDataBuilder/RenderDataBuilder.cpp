#include "Systems/RenderDataBuilder/RenderDataBuilder.h"

#include "Model/Map/Map.h"
#include "Model/Map/MapConstants.h"
#include "Model/Player/Player.h"
#include "World/World.h"

namespace
{
	MapRenderData BuildMapRenderData(const Map& map)
	{
		MapRenderData data;
		data.width = map.GetWidth();
		data.height = map.GetHeight();
		data.tileSize = static_cast<float>(MapConstants::TileSize);

		data.tiles.reserve(static_cast<std::size_t>(data.width) * static_cast<std::size_t>(data.height));
		for (int row = 0; row < data.height; ++row)
		{
			for (int col = 0; col < data.width; ++col)
			{
				data.tiles.push_back(map.GetTileAtCell(col, row));
			}
		}

		return data;
	}

	EntityRenderData BuildPlayerRenderData(const Player& player)
	{
		EntityRenderData data;
		data.position = player.GetPosition();
		data.size = player.GetBounds().size;
		data.facingDirection = player.GetFacingDirection();
		data.visual = EntityVisual::Hero;
		return data;
	}
}

RenderSnapshot RenderDataBuilder::Build(const World& world) const
{
	RenderSnapshot snapshot;
	snapshot.map = BuildMapRenderData(world.GetMap());
	snapshot.entities.push_back(BuildPlayerRenderData(world.GetPlayer()));
	snapshot.cameraTargetPosition = world.GetPlayer().GetPosition();
	return snapshot;
}
