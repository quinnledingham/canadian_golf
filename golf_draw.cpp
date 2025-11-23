internal void
draw_rect(Vector2 coords, Vector2 size, Color_RGBA color) {
	const SDL_FRect rect = { coords.x, coords.y, coords.x + size.x, coords.y + size.y };
	SDL_SetRenderDrawColor(sdl_ctx.renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(sdl_ctx.renderer, &rect);
}

//
// From https://github.com/nicbarker/clay/blob/main/renderers/SDL3/clay_renderer_SDL3.c
//

/* Global for convenience. Even in 4K this is enough for smooth curves (low radius or rect size coupled with
* no AA or low resolution might make it appear as jagged curves) */
static int NUM_CIRCLE_SEGMENTS = 16;

//all rendering is performed by a single SDL call, avoiding multiple RenderRect + plumbing choice for circles.
internal void
draw_rounded_rect(const Vector2 coords, const Vector2 size, const float cornerRadius, const Color_RGBA _color) {
	const SDL_FRect rect = { coords.x, coords.y, coords.x + size.x, coords.y + size.y };
	Vector4 colorv4 = cv4(_color);
	const SDL_FColor color = { colorv4.r / 255, colorv4.g / 255, colorv4.b / 255, colorv4.a / 255 };

	int indexCount = 0, vertexCount = 0;

	const float minRadius = SDL_min(rect.w, rect.h) / 2.0f;
	const float clampedRadius = SDL_min(cornerRadius, minRadius);

	const int numCircleSegments = SDL_max(NUM_CIRCLE_SEGMENTS, (int)(clampedRadius * 0.5f));

	int totalVertices = 4 + (4 * (numCircleSegments * 2)) + 2 * 4;
	int totalIndices = 6 + (4 * (numCircleSegments * 3)) + 6 * 4;

	//SDL_Vertex vertices[totalVertices];
	SDL_Vertex *vertices = (SDL_Vertex *)SDL_malloc(totalVertices * sizeof(SDL_Vertex));
	//int indices[totalIndices];
	int *indices = (int *)SDL_malloc(totalIndices * sizeof(int));

	//define center rectangle
	vertices[vertexCount++] = SDL_Vertex { { rect.x + clampedRadius, rect.y + clampedRadius }, color, { 0, 0 } }; //0 center TL
	vertices[vertexCount++] = SDL_Vertex { { rect.x + rect.w - clampedRadius, rect.y + clampedRadius }, color, { 1, 0 } }; //1 center TR
	vertices[vertexCount++] = SDL_Vertex { { rect.x + rect.w - clampedRadius, rect.y + rect.h - clampedRadius }, color, { 1, 1 } }; //2 center BR
	vertices[vertexCount++] = SDL_Vertex { { rect.x + clampedRadius, rect.y + rect.h - clampedRadius }, color, { 0, 1 } }; //3 center BL

	indices[indexCount++] = 0;
	indices[indexCount++] = 1;
	indices[indexCount++] = 3;
	indices[indexCount++] = 1;
	indices[indexCount++] = 2;
	indices[indexCount++] = 3;

	//define rounded corners as triangle fans
	const float step = (SDL_PI_F / 2) / numCircleSegments;
	for (int i = 0; i < numCircleSegments; i++) {
		const float angle1 = (float)i * step;
		const float angle2 = ((float)i + 1.0f) * step;

		for (int j = 0; j < 4; j++) {  // Iterate over four corners
			float cx, cy, signX, signY;

			switch (j) {
				case 0: cx = rect.x + clampedRadius; cy = rect.y + clampedRadius; signX = -1; signY = -1; break; // Top-left
				case 1: cx = rect.x + rect.w - clampedRadius; cy = rect.y + clampedRadius; signX = 1; signY = -1; break; // Top-right
				case 2: cx = rect.x + rect.w - clampedRadius; cy = rect.y + rect.h - clampedRadius; signX = 1; signY = 1; break; // Bottom-right
				case 3: cx = rect.x + clampedRadius; cy = rect.y + rect.h - clampedRadius; signX = -1; signY = 1; break; // Bottom-left
				default: return;
			}

			vertices[vertexCount++] = SDL_Vertex { { cx + SDL_cosf(angle1) * clampedRadius * signX, cy + SDL_sinf(angle1) * clampedRadius * signY }, color, { 0, 0 } };
			vertices[vertexCount++] = SDL_Vertex { { cx + SDL_cosf(angle2) * clampedRadius * signX, cy + SDL_sinf(angle2) * clampedRadius * signY }, color, { 0, 0 } };

			indices[indexCount++] = j;  // Connect to corresponding central rectangle vertex
			indices[indexCount++] = vertexCount - 2;
			indices[indexCount++] = vertexCount - 1;
		}
	}

	//Define edge rectangles
	// Top edge
	vertices[vertexCount++] = SDL_Vertex { { rect.x + clampedRadius, rect.y }, color, { 0, 0 } }; //TL
	vertices[vertexCount++] = SDL_Vertex { { rect.x + rect.w - clampedRadius, rect.y }, color, { 1, 0 } }; //TR

	indices[indexCount++] = 0;
	indices[indexCount++] = vertexCount - 2; //TL
	indices[indexCount++] = vertexCount - 1; //TR
	indices[indexCount++] = 1;
	indices[indexCount++] = 0;
	indices[indexCount++] = vertexCount - 1; //TR
	// Right edge
	vertices[vertexCount++] = SDL_Vertex { { rect.x + rect.w, rect.y + clampedRadius }, color, { 1, 0 } }; //RT
	vertices[vertexCount++] = SDL_Vertex { { rect.x + rect.w, rect.y + rect.h - clampedRadius }, color, { 1, 1 } }; //RB

	indices[indexCount++] = 1;
	indices[indexCount++] = vertexCount - 2; //RT
	indices[indexCount++] = vertexCount - 1; //RB
	indices[indexCount++] = 2;
	indices[indexCount++] = 1;
	indices[indexCount++] = vertexCount - 1; //RB
	// Bottom edge
	vertices[vertexCount++] = SDL_Vertex { { rect.x + rect.w - clampedRadius, rect.y + rect.h }, color, { 1, 1 } }; //BR
	vertices[vertexCount++] = SDL_Vertex { { rect.x + clampedRadius, rect.y + rect.h }, color, { 0, 1 } }; //BL

	indices[indexCount++] = 2;
	indices[indexCount++] = vertexCount - 2; //BR
	indices[indexCount++] = vertexCount - 1; //BL
	indices[indexCount++] = 3;
	indices[indexCount++] = 2;
	indices[indexCount++] = vertexCount - 1; //BL
	// Left edge
	vertices[vertexCount++] = SDL_Vertex { { rect.x, rect.y + rect.h - clampedRadius }, color, { 0, 1 } }; //LB
	vertices[vertexCount++] = SDL_Vertex { { rect.x, rect.y + clampedRadius }, color, { 0, 0 } }; //LT

	indices[indexCount++] = 3;
	indices[indexCount++] = vertexCount - 2; //LB
	indices[indexCount++] = vertexCount - 1; //LT
	indices[indexCount++] = 0;
	indices[indexCount++] = 3;
	indices[indexCount++] = vertexCount - 1; //LT

	// Render everything
	SDL_RenderGeometry(sdl_ctx.renderer, NULL, vertices, vertexCount, indices, indexCount);

	SDL_free(vertices);
	SDL_free(indices);
}

/*
internal String_Draw_Info
get_string_draw_info(u32 id, const char *string, s32 length, float32 pixel_height) {
  Font *font = find_font(id);
  return get_string_draw_info(font, string, length, pixel_height);
}
*/

internal void
draw_text(const char *text, Vector2 coords, float32 pixel_height, Color_RGBA color) {
	Font *font = find_font(draw_ctx.font_id);

	//SDL_SetRenderDrawColor(sdl_ctx.renderer, color.r, color.g, color.b, color.a);

	TTF_SetFontSize(font->ttf_font, pixel_height);
	TTF_Text *ttf_text = TTF_CreateText(sdl_ctx.text_engine, font->ttf_font, text, 0);

	#if DEBUG

	if (!ttf_text) {
		app_log("draw_text(): failed to create text (for %s)\n", text);
		return;
	}

	#endif // DEBUG

	TTF_SetTextColor(ttf_text, color.r, color.g, color.b, color.a);
	bool result = TTF_DrawRendererText(ttf_text, coords.x, coords.y);
	if (!result) {
		sdl_log("draw_text(), TTF_DrawRendererText failed (%s)\n", SDL_GetError());
	}

	if (ttf_text) {
		TTF_DestroyText(ttf_text);
	}
}
