#include "sani/graphics/renderables/circle.hpp"
#include "sani/core/math/trigonometric.hpp"
#include "sani/graphics/vertex_helper.hpp"
#include "sani/graphics/render_helper.hpp"
#include "sani/graphics/renderer.hpp"

namespace sani {

	namespace graphics {

		void recomputeGeometryData(Circle& circle) {
			// TODO: optimize.

			recomputeBounds(circle);
			recomputeVertices(circle);
		}
		void recomputeVertices(Circle& circle) {
			const sani::math::Vec3f& position = circle.transform.position;
			const sani::math::Vec3f& origin = circle.transform.origin;
			const sani::math::Vec3f& scale = circle.transform.scale;
			const float32 rotation = circle.transform.rotation;

			/*
				TODO: borders, rotation.
			*/

			const float32 sin = sani::math::sin(rotation);
			const float32 cos = sani::math::cos(rotation);

			const float32 dx = -origin.x * scale.x;
			const float32 dy = -origin.y * scale.y;

			for (uint32 i = 0; i < circle.vertices; i++) {
				const float32 percent = (i / float(circle.vertices));
				const float32 radius = percent * 2.0f * PI;

				const float32 ox = origin.x + circle.radius * scale.x * sani::math::cos(radius);
				const float32 oy = origin.y + circle.radius * scale.y * sani::math::sin(radius);

				sani::math::Vec3f globalPosition = position;
				sani::math::Vec3f vertexPosition(ox, oy, position.z);

				applyRotationToBottomRightVertex(&globalPosition, &vertexPosition, dx, dy, sin, cos);

				VertexPositionColorTexture& vertex = circle.renderData.vertices[i];
				vertex.vertexPositionColor.position = globalPosition;
				vertex.vertexPositionColor.color = circle.fill;
			}

			if (circle.borderThickness > 0.0f) {
				const float32 borderRadius = circle.radius + circle.borderThickness;

				for (uint32 i = circle.vertices; i < circle.vertices * 2 - 1; i++) {
					const float32 percent = (i / float(circle.vertices));
					const float32 radius = percent * 2.0f * PI;

					const float32 ox = origin.x + borderRadius * scale.x * sani::math::cos(radius);
					const float32 oy = origin.y + borderRadius * scale.y * sani::math::sin(radius);

					sani::math::Vec3f globalPosition = position;
					sani::math::Vec3f vertexPosition(ox, oy, position.z);

					applyRotationToBottomRightVertex(&globalPosition, &vertexPosition, dx, dy, sin, cos);

					VertexPositionColorTexture& vertex = circle.renderData.vertices[i];
					vertex.vertexPositionColor.position = globalPosition;
					vertex.vertexPositionColor.color = circle.borderFill;
				}
			}
 		}
		void recomputeBounds(Circle& circle) {
			circle.localBounds.x = 0.0f;
			circle.localBounds.y = 0.0f;
			circle.localBounds.w = circle.radius * 2.0f;
			circle.localBounds.h = circle.radius * 2.0f;

			const sani::math::Vec3f& position = circle.transform.position;
			const sani::math::Vec3f& scale = circle.transform.scale;
			
			circle.globalBounds.x = position.x;
			circle.globalBounds.y = position.y;
			circle.globalBounds.w = circle.localBounds.w * scale.x;
			circle.globalBounds.h = circle.localBounds.h * scale.y;
		}

		void updateRenderData(Circle& circle) {
			setupShapeForRendering(&circle, circle.borderThickness);
		}
	}
}