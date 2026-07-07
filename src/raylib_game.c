/*******************************************************************************************
*
*   raylib gamejam template
*
*   Code licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2026 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

/*
Tasks:
- Manually record all necessary info about each sprite in an array.
- Write function to draw sprite based on the info in the array.
- Store and load levels in the dumbest way possible. Just make a text file storing shit like A B or C or whatever, where each letter basically represents a different tile or entity or whatever. The fact is, you don't really need the flexibility of LDTK for this, so why bother? You could use it for a future game jam.
- Implement basic player movement and attacking (you will play as the knight).
- Implement the witch. If the witch sees you, then:
    - If the witch is far away from you, then:
        - It should get on its broom stick and fly towards you.
        - If it slams into you, then you both take damage. If it slams into a wall, then only it takes damage.
    - If the witch is close to you, then:
        - It should walk away from you so that way it can get onto its broom stick.
- Implement the goblin. If the goblin sees you, then:
    - It should start shooting at you.
    - If you go out of range for long enough, then it should just reenter its idle state.
- Implement the skeleton and the slime. They will both work the same way, except maybe the skeleton will be immune to fire? Basically they will work like the goblin except they use melee attacks instead of ranged attacks.
- Implement spikes. When you walk on spikes, you die instantly.
- And so on. You can add more to this as you go, assuming you have time. I will start implementing all of this tomorrow.
- Change README.
- 
*/

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>      // Emscripten library
#endif

// #include "cJson.h"

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for:

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

typedef struct Vector2i
{
    int x;
    int y;
} Vector2i;

//#pragma pack(push, 1)
//
//typedef struct ASE_Fixed
//{
//	unsigned short val[2];
//} ASE_Fixed;
//
//typedef struct ASE_String
//{
//	unsigned short len;
//#if 0
//	unsigned char buf[];
//#endif
//} ASE_String;
//
//
//typedef Vector2i ASE_Point;
//typedef Vector2i ASE_Size;
//
//typedef struct ASE_Rect
//{
//	ASE_Point origin;
//	ASE_Size size;
//} ASE_Rect;
//
//typedef union ASE_Pixel
//{
//	unsigned char rgba[4];
//	unsigned char grayscale[2];
//	unsigned char index;
//} ASE_Pixel;
//
//typedef unsigned int ASE_Flags;
//enum
//{
//	ASE_Flags_LayerOpacityValid = 1u,
//	ASE_Flags_LayerOpacityValidForGroups = 2u,
//	ASE_Flags_LayersHaveUUID = 4u,
//};
//
//typedef struct ASE_Header
//{
//	unsigned int file_size;
//	unsigned short magic_number;
//	unsigned short num_frames;
//	unsigned short w;
//	unsigned short h;
//	unsigned short color_depth;
//	ASE_Flags flags;
//	unsigned short reserved0;
//	unsigned int reserved1;
//	unsigned int reserved2;
//	unsigned char transparent_color_entry;
//	unsigned char reserved3[3];
//	unsigned short num_colors;
//	unsigned char pixel_w;
//	unsigned char pixel_h;
//	signed short grid_x;
//	signed short grid_y;
//	unsigned short grid_w;
//	unsigned short grid_h;
//	unsigned char reserved4[84];
//} ASE_Header;
//static_assert(sizeof(ASE_Header) == 128, "ASE_Header incorrectly sized");
//
//typedef struct ASE_Frame
//{
//	unsigned int num_bytes;
//	unsigned short magic_number;
//	unsigned short reserved0;
//	unsigned short frame_dur;
//	unsigned char reserved1[2];
//	unsigned int num_chunks;
//} ASE_Frame;
//static_assert(sizeof(ASE_Frame) == 16, "ASE_Frame incorrectly sized");
//
//typedef unsigned short ASE_ChunkType;
//enum
//{
//	ASE_ChunkType_OldPalette = 0x0004u,
//	ASE_ChunkType_OldPalette2 = 0x0011u,
//	ASE_ChunkType_Layer = 0x2004u,
//	ASE_ChunkType_Cell = 0x2005u,
//	ASE_ChunkType_CellExtra = 0x2006u,
//	ASE_ChunkType_ColorProfile = 0x2007u,
//	ASE_ChunkType_ExternalFiles = 0x2008u,
//	ASE_ChunkType_DeprecatedMask = 0x2016u,
//	ASE_ChunkType_Path = 0x2017u,
//	ASE_ChunkType_Tags = 0x2018u,
//	ASE_ChunkType_Palette = 0x2019u,
//	ASE_ChunkType_UserData = 0x2020u,
//	ASE_ChunkType_Slice = 0x2022u,
//	ASE_ChunkType_Tileset = 0x2023u,
//};
//
//typedef struct ASE_ChunkHeader
//{
//	unsigned int size;
//	ASE_ChunkType type;
//} ASE_ChunkHeader;
//
//typedef unsigned short ASE_LayerChunkFlags;
//enum
//{
//	ASE_LayerChunkFlags_Visible = 1u,
//	ASE_LayerChunkFlags_Editable = 2u,
//	ASE_LayerChunkFlags_LockMovement = 4u,
//	ASE_LayerChunkFlags_Background = 8u,
//	ASE_LayerChunkFlags_PreferLinkedCells = 16u,
//	ASE_LayerChunkFlags_DisplayCollapsed = 32u,
//	ASE_LayerChunkFlags_ReferenceLayer = 64u,
//};
//
//typedef unsigned short ASE_LayerType;
//enum
//{
//	ASE_LayerType_Normal = 0u,
//	ASE_LayerType_Group = 1u,
//	ASE_LayerType_Tilemap = 2u,
//};
//
//typedef unsigned short ASE_BlendMode;
//enum
//{
//	ASE_BlendMode_Normal = 0u,
//	ASE_BlendMode_Multiply = 1u,
//	ASE_BlendMode_Screen = 2u,
//	ASE_BlendMode_Overlay = 3u,
//	ASE_BlendMode_Darken = 4u,
//	ASE_BlendMode_Lighten = 5u,
//	ASE_BlendMode_ColorDodge = 6u,
//	ASE_BlendMode_ColorBurn = 7u,
//	ASE_BlendMode_HardLight = 8u,
//	ASE_BlendMode_SoftLight = 9u,
//	ASE_BlendMode_Difference = 10u,
//	ASE_BlendMode_Exclusion = 11u,
//	ASE_BlendMode_Hue = 12u,
//	ASE_BlendMode_Saturation = 13u,
//	ASE_BlendMode_Color = 14u,
//	ASE_BlendMode_Luminosity = 15u,
//	ASE_BlendMode_Addition = 16u,
//	ASE_BlendMode_Subtract = 17u,
//	ASE_BlendMode_Divide = 18u,
//};
//
//typedef struct ASE_LayerChunk
//{
//	ASE_LayerChunkFlags flags;
//	ASE_LayerType layer_type;
//	unsigned short layer_child_level;
//	unsigned short reserved0;
//	unsigned short reserved1;
//	ASE_BlendMode blend_mode;
//	unsigned char opacity;
//	unsigned char reserved2[3];
//	ASE_String layer_name;
//#if 0
//	uint tileset_idx;
//	unsigned char uuid[16];
//#endif
//} ASE_LayerChunk;
//
//typedef unsigned short ASE_CellType;
//enum
//{
//	ASE_CellType_Raw = 0u,
//	ASE_CellType_Linked = 1u,
//	ASE_CellType_CompressedImage = 2u,
//	ASE_CellType_CompressedTilemap = 3u,
//};
//
//typedef struct ASE_CellChunk
//{
//	unsigned short layer_idx;
//	short x;
//	short y;
//	unsigned char opacity;
//	ASE_CellType type;
//	short z_idx;
//	unsigned char reserved0[5];
//	unsigned short w;
//	unsigned short h;
//#if 0
//	ASE_Pixel pixels[];
//#endif
//} ASE_CellChunk;
//
//typedef unsigned int ASE_CellExtraChunkFlags;
//enum
//{
//	ASE_CellExtraChunkFlags_PreciseBounds = 1u,
//};
//
//typedef struct ASE_CellExtraChunk
//{
//	ASE_CellExtraChunkFlags flags;
//	ASE_Fixed x;
//	ASE_Fixed y;
//	ASE_Fixed w;
//	ASE_Fixed h;
//	unsigned char reserved0[16];
//} ASE_CellExtraChunk;
//
//typedef unsigned short ASE_ColorProfileType;
//enum
//{
//	ASE_ColorProfileType_None = 0u,
//	ASE_ColorProfileType_SRGB = 1u,
//	ASE_ColorProfileType_EmbeddedICC = 2u,
//};
//
//typedef unsigned short ASE_ColorProfileFlags;
//enum
//{
//	ASE_ColorProfileFlags_SpecialFixedGamma = 1u,
//};
//
//typedef struct ASE_ColorProfileChunk
//{
//	ASE_ColorProfileType type;
//	ASE_ColorProfileFlags flags;
//	ASE_Fixed fixed_gamma;
//	unsigned char reserved0[8];
//	unsigned int icc_profile_data_len;
//#if 0
//	unsigned char icc_profile_data[];
//#endif
//} ASE_ColorProfileChunk;
//
//typedef unsigned char ASE_ExternalFilesEntryType;
//enum
//{
//	ASE_ExternalFilesEntryType_ExternalPalette = 0u,
//	ASE_ExternalFilesEntryType_ExternalTileset = 1u,
//	ASE_ExternalFilesEntryType_ExtensionNameForProperties = 2u,
//	ASE_ExternalFilesEntryType_ExtensionNameForTileManagement = 3u,
//};
//
//typedef struct ASE_ExternalFilesEntry
//{
//	unsigned int id;
//	ASE_ExternalFilesEntryType type;
//	unsigned char reserved0[7];
//	union
//	{
//		ASE_String external_file_name;
//		ASE_String extension_id;
//	};
//} ASE_ExternalFilesEntry;
//
//typedef struct ASE_ExternalFilesChunk
//{
//	unsigned int num_entries;
//	unsigned char reserved0[8];
//#if 0
//	ASE_ExternalFilesEntry entries[];
//#endif
//} ASE_ExternalFilesChunk;
//
//typedef unsigned char ASE_LoopAnimDir;
//enum
//{
//	ASE_LoopAnimDir_Forward = 0u,
//	ASE_LoopAnimDir_Reverse = 1u,
//	ASE_LoopAnimDir_PingPong = 2u,
//	ASE_LoopAnimDir_PingPongReverse = 3u,
//};
//
//typedef struct ASE_Tag
//{
//	unsigned short from_frame;
//	unsigned short to_frame;
//	ASE_LoopAnimDir loop_anim_dir;
//	unsigned short repeat;
//	unsigned char reserved0[6];
//	unsigned char reserved1[3];
//	unsigned char reserved2;
//	ASE_String name;
//} ASE_Tag;
//
//typedef struct ASE_TagsChunk
//{
//	unsigned short num_tags;
//#if 0
//	ASE_Tag tags[];
//#endif
//} ASE_TagsChunk;
//
//typedef unsigned short ASE_PaletteEntryFlags;
//enum
//{
//	ASE_PaletteEntryFlags_HasName = 1u,
//};
//
//typedef struct ASE_PaletteEntry
//{
//	ASE_PaletteEntryFlags flags;
//	unsigned char r;
//	unsigned char g;
//	unsigned char b;
//	unsigned char a;
//#if 0
//	ASE_String color_name;
//#endif
//} ASE_PaletteEntry;
//
//typedef struct ASE_PaletteChunk
//{
//	unsigned int num_entries;
//	unsigned int first_color_idx_to_change;
//	unsigned int last_color_idx_to_change;
//	unsigned char reserved0[8];
//#if 0
//	ASE_PaletteEntry entries[];
//#endif
//} ASE_PaletteChunk;
//
//#pragma pack(pop)

//typedef struct SpriteCell
//{
//	Vector2i origin;
//	Vector2i size;
//	int z_idx;
//	unsigned int layer_idx;
//} SpriteCell;
//
//typedef struct SpriteFrame
//{
//	/**
//	 * Why have multiple cells in each frame? The reason is because that's how Aseprite does
//	 * things. In Aseprite, a sprite is made up of an array of frames, and each frame is made up
//	 * of an array of cells.
//	 */
//	SpriteCell* cells; size_t num_cells;
//
//	/**
//	 * Not every frame necessarily has a hitbox. If it exists, it is literally drawn inside of
//	 * Aseprite in a layer called "Hitbox". See GetEntityHitbox to find out how a hitbox is
//	 * selected for an entity.
//	 */
//	Rectangle hitbox;
//
//	/**
//	 * The duration of the frame. See UpdateAnim to find out how this is used.
//	 */
//	float dur;
//} SpriteFrame;
//
//typedef struct ASE_Result
//{
//	bool ok;
//	int width;
//	int height;
//	SpriteFrame* frames;
//	unsigned short num_frames;
//} ASE_Result;

// static ASE_Result ASE_LoadSprite(Context* ctx, char* path, Arena* arena);

typedef enum { 
    GameScreen_Logo, 
    GameScreen_Title, 
    GameScreen_Gameplay, 
    GameScreen_Ending,
} GameScreen;

// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
//typedef struct Arena
//{
//	unsigned char* buf;
//	size_t buf_len;
//	size_t prev_offset;
//	size_t curr_offset;
//} Arena;
//
//static bool IsPowerOf2(uintptr_t x)
//{
//	return (x & (x - 1)) == 0;
//}
//
//static uintptr_t AlignForward(uintptr_t ptr, uintptr_t align)
//{
//	assert(IsPowerOf2(align));
//	size_t p = ptr;
//	size_t a = align;
//	size_t modulo = p & (a - 1);
//	if (modulo != 0)
//	{
//		p += a - modulo;
//	}
//	return p;
//}
//
//static void* ArenaAllocRaw(Arena* arena, size_t size, size_t align)
//{
//	if (size == 0) return NULL;
//
//	// Align 'curr_offset' forward to the specified alignment
//	uintptr_t curr_ptr = (uintptr_t)arena->buf + (uintptr_t)arena->curr_offset;
//	uintptr_t offset = AlignForward(curr_ptr, align);
//	offset -= (uintptr_t)arena->buf; // Change to relative offset
//
//	assert(offset + size <= arena->buf_len);
//
//	void* ptr = &arena->buf[offset];
//	arena->prev_offset = offset;
//	arena->curr_offset = offset + size;
//
//	// Zero new memory by default
//	RL_MEMSET(ptr, 0, size);
//	return ptr;
//}
//
//#define ArenaAlloc(ARENA, COUNT, TYPE) (TYPE*)ArenaAllocRaw((ARENA), (COUNT)*sizeof(TYPE), alignof(TYPE))
//
//// https://www.gingerbill.org/article/2019/02/15/memory-allocation-strategies-003/
//typedef struct Stack
//{
//	unsigned char* buf;
//	size_t buf_len;
//	size_t prev_offset;
//	size_t curr_offset;
//} Stack;
//
//typedef struct StackAllocHeader
//{
//	size_t prev_offset;
//	size_t padding;
//} StackAllocHeader;
//
//static size_t CalcPaddingWithHeader(uintptr_t ptr, uintptr_t alignment, size_t header_size)
//{
//	assert(IsPowerOf2(alignment));
//	uintptr_t modulo = ptr & (alignment - 1); // (ptr % alignment) as it assumes alignment is a power of two
//	uintptr_t padding = 0;
//
//	if (modulo != 0)
//	{ // Same logic as 'align_forward'
//		padding = alignment - modulo;
//	}
//
//	uintptr_t needed_space = (uintptr_t)header_size;
//
//	if (padding < needed_space)
//	{
//		needed_space -= padding;
//
//		if ((needed_space & (alignment - 1)) != 0)
//		{
//			padding += alignment * (1 + (needed_space / alignment));
//		}
//		else
//		{
//			padding += alignment * (needed_space / alignment);
//		}
//	}
//
//	return (size_t)padding;
//}
//
//static void* StackAllocRaw(Stack* stack, size_t size, size_t align)
//{
//	void* res = NULL;
//
//	assert(IsPowerOf2(align));
//
//	uintptr_t curr_addr = (uintptr_t)stack->buf + (uintptr_t)stack->curr_offset;
//	size_t padding = CalcPaddingWithHeader(curr_addr, (uintptr_t)align, sizeof(StackAllocHeader));
//	assert(stack->curr_offset + padding + size <= stack->buf_len);
//
//	if (size != 0)
//	{
//		stack->prev_offset = stack->curr_offset; // Store the previous offset
//		stack->curr_offset += padding;
//
//		uintptr_t next_addr = curr_addr + (uintptr_t)padding;
//		StackAllocHeader* header = (StackAllocHeader*)(next_addr - sizeof(StackAllocHeader));
//		header->padding = padding;
//		header->prev_offset = stack->prev_offset;
//
//		stack->curr_offset += size;
//
//		res = SDL_memset((void*)next_addr, 0, size);
//	}
//
//	return res;
//}
//
//#define StackAlloc(STACK, COUNT, TYPE) (TYPE*)StackAllocRaw(STACK, COUNT*sizeof(TYPE), alignof(TYPE))
//
//static void StackFree(Stack* stack, void* ptr)
//{
//	if (ptr)
//	{
//		uintptr_t start = (uintptr_t)stack->buf;
//		uintptr_t end = start + (uintptr_t)stack->buf_len;
//		uintptr_t curr_addr = (uintptr_t)ptr;
//
//		if (!(start <= curr_addr && curr_addr < end))
//		{
//			assert(0 && "Out of bounds memory address passed to stack allocator (free)");
//			return;
//		}
//
//		if (curr_addr >= start + (uintptr_t)stack->curr_offset)
//		{
//			// Allow double frees
//			return;
//		}
//
//		StackAllocHeader* header = (StackAllocHeader*)(curr_addr - sizeof(StackAllocHeader));
//		size_t prev_offset = (size_t)(curr_addr - (uintptr_t)header->padding - start);
//
//		assert(prev_offset == header->prev_offset);
//
//		stack->curr_offset = stack->prev_offset;
//		stack->prev_offset = header->prev_offset;
//	}
//}
//
//static void StackFreeAll(Stack* stack)
//{
//	stack->curr_offset = 0;
//}

typedef struct {
    Rectangle src;
    int frame_count;
} Sprite;

typedef struct Sprites {
    Sprite witch_idle;
} Sprites;

static void DrawSprite(Texture2D atlas, Sprite* sprite, Vector2 position, int frame_idx, Color tint) {
	// assert(sprite->frame_count > 0);
	// assert(frame_idx >= 0 && frame_idx < sprite->frame_count);
	float frame_width = sprite->src.width / (float)sprite->frame_count;
	Rectangle src = {
		.x = sprite->src.x + frame_width * (float)frame_idx,
		.y = sprite->src.y,
		.width = frame_width,
		.height = sprite->src.height,
	};
	DrawTexturePro(atlas, src, (Rectangle) { position.x, position.y, src.width, src.height }, (Vector2) { 0.0f, 0.0f }, 0.0f, tint);
}

typedef struct Context {
    GameScreen screen;
    int frame_counter;
    Sprites sprites;
    Texture2D atlas;
} Context;

#define WIDTH 720
#define HEIGHT 720

static void UpdateDrawFrame(Context*);      // Update and Draw one frame

int main(void) {
    Context ctx = { 0 };
    ctx.sprites.witch_idle = (Sprite){
        .src = (Rectangle){33.0f, 641.0f, 96.0f-33.0f, 671.0f-641.0f},
        .frame_count = 4,
    };

	

    ctx.screen = GameScreen_Gameplay;

#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(WIDTH, HEIGHT, "raylib gamejam template");

    ctx.atlas = LoadTexture("C:/raylib-gamejam-template/assets/atlas.png");
    
    // TODO: Load resources / Initialize variables at this point


#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1); // This is needed for web because in web, there is no window to check for.
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame(&ctx);
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    // TODO: Unload all loaded resources at this point


    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

static void UpdateDrawFrame(Context* ctx)
{
    // Update
    //----------------------------------------------------------------------------------
	float fixed_dt = 1.0f / 60.0f;
    float dt = GetFrameTime();
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        switch (ctx->screen) {
        case GameScreen_Logo:
            // TODO: Draw your game screen here

            DrawRectangle(70, 90, 200, 200, BLACK);
            DrawRectangle(70 + 16, 90 + 16, 200 - 32, 200 - 32, RAYWHITE);
            DrawText("raylib", 70 + 200 - MeasureText("raylib", 40) - 32, 90 + 200 - 40 - 24, 40, BLACK);

            DrawText("6.x", 290, 90 - 26, 280, BLACK);
            DrawText("GAMEJAM", 70, 90 + 210, 120, MAROON);

            if ((ctx->frame_counter / 20) % 2) DrawText("are you ready?", 160, 500, 50, BLACK);

            DrawRectangleLinesEx((Rectangle) { 0, 0, WIDTH, HEIGHT }, 16, BLACK);

            // TODO: Draw everything that requires to be drawn at this point, maybe UI?
            break;

        case GameScreen_Gameplay:
            static int tick = 0;
            static int frame_idx = 0;
            ++tick;
			if (tick > 10) {
				tick = 0;
				frame_idx += 1;
				if (frame_idx >= ctx->sprites.witch_idle.frame_count) {
					frame_idx = 0;
				}
			}
			DrawSprite(ctx->atlas, &ctx->sprites.witch_idle, (Vector2) { 100.0f, 100.0f }, frame_idx, WHITE);
        }

        
    }
    EndDrawing();
    //----------------------------------------------------------------------------------
}

//static unsigned char* _AdvancePtr(unsigned char* ptr, int size, int cap)
//{
//	if (ptr + size > cap)
//	{
//		LOG("%s(%d): Bounds check failed: ptr=%p, size=%d, cap=%d\n", __FUNCTION__, __LINE__, ptr, size, cap);
//		return ptr;
//	}
//	
//	unsigned char* new_ptr = ptr + size;
//	return new_ptr;
//}
//#define AdvancePtr(P, T, C) (T*)_AdvancePtr(P, sizeof(T), C)
//
//static ASE_Result ASE_LoadSprite(Context* ctx, char* path, Arena* arena, Stack* stack)
//{
//	ASE_Result res = { 0 };
//
//	int file_size;
//	unsigned char* file_data = LoadFileData(path, &file_size);
//	if (!file_data || file_size < sizeof(ASE_Header))
//	{
//		LOG("%s(%d): Failed to load file: %s\n", __FUNCTION__, __LINE__, path);
//		return res;
//	}
//	unsigned char* file_cur = file_data;
//
//	ASE_Header* header = AdvancePtr(file_cur, ASE_Header, file_size);
//
//	assert(header->color_depth == 32);
//	assert((header->pixel_w == 0 || header->pixel_w == 1) && (header->pixel_h == 0 || header->pixel_h == 1));
//	assert(header->grid_x == 0);
//	assert(header->grid_y == 0);
//	assert(header->grid_w == 0 || header->grid_w == 16);
//	assert(header->grid_h == 0 || header->grid_h == 16);
//
//	res.num_frames = header->num_frames;
//	res.frames = (SpriteFrame*)ArenaAllocRaw(arena, res.num_frames*sizeof(SpriteFrame), 1);
//
//	unsigned short hitbox_layer_idx = USHRT_MAX;
//	unsigned short origin_layer_idx = USHRT_MAX;
//
//	for (size_t frame_idx = 0; frame_idx < res.num_frames; frame_idx += 1)
//	{
//		ASE_Frame* frame = AdvancePtr(file_cur, ASE_Frame, file_size);
//		assert(frame->magic_number == 0xF1FA);
//
//		// Would mean this aseprite file is very old.
//		assert(frame->num_chunks != 0);
//
//		res.frames[frame_idx].dur = ((float)frame->frame_dur) / (1000.0f / 60.0f);
//
//		unsigned char* prev_file_cur = file_cur;
//
//		// NOTE: According to the Aseprite spec, all layer chunks are found in the first frame, but not necessarily before everything else in that frame->
//		// https://github.com/aseprite/aseprite/blob/main/docs/ase-file-specs.md#layer-chunk-0x2004
//		if (frame_idx == 0)
//		{
//			for (size_t chunk_idx = 0, layer_idx = 0; chunk_idx < frame->num_chunks; chunk_idx += 1)
//			{
//				void* raw_chunk;
//				size_t raw_chunk_size;
//				ASE_ChunkType* chunk_type = AdvancePtr(file_cur, ASE_ChunkType, file_size);
//				if (chunk_type == ASE_ChunkType_Layer)
//				{
//					ASE_LayerChunk* chunk = raw_chunk;
//					assert(chunk->layer_name.len > 0);
//					
//					char* layer_name = (char*)StackAllocRaw(stack, chunk->layer_name.len + 1 * sizeof(char), 1);
//
//					SDL_strlcpy(layer_name, (const char*)(chunk + 1), chunk->layer_name.len + 1);
//
//					if (strcmp(layer_name, "Hitbox") == 0)
//					{
//						assert(hitbox_layer_idx == USHRT_MAX);
//						hitbox_layer_idx = (unsigned short)layer_idx;
//					}
//					else if (strcmp(layer_name, "Origin") == 0)
//					{
//						assert(origin_layer_idx == USHRT_MAX);
//						origin_layer_idx = (unsigned short)layer_idx;
//					}
//					layer_idx += 1;
//
//					StackFree(stack, layer_name);
//				}
//
//				StackFree(stack, raw_chunk);
//			}
//
//			file_cur = prev_file_cur;
//		}
//
//		for (size_t chunk_idx = 0; chunk_idx < frame->num_chunks; chunk_idx += 1)
//		{
//			void* raw_chunk;
//			size_t raw_chunk_size;
//			ASE_ChunkType* chunk_type = AdvancePtr(file_cur, ASE_ChunkType, file_size);
//
//			if (chunk_type == ASE_ChunkType_Cell)
//			{
//				ASE_CellChunk* chunk = raw_chunk;
//				if (chunk->layer_idx == hitbox_layer_idx)
//				{
//					res.frames[frame_idx].hitbox = (Rectangle)
//					{
//						.x = (int)chunk->x,
//						.y = (int)chunk->y,
//						.width = (int)chunk->w,
//						.height = (int)chunk->h,
//					};
//				}
//				else if (chunk->layer_idx == origin_layer_idx)
//				{
//					if (frame_idx == 0)
//					{
//						res.origin = (Vector2){ (int)chunk->x, (int)chunk->y };
//					}
//				}
//				else
//				{
//					res.frames[frame_idx].num_cells += 1;
//					assert(chunk->type == ASE_CellType_CompressedImage);
//				}
//			}
//
//			StackFree(stack, raw_chunk);
//		}
//		
//		LOG("sprites[%s].frames[%llu].num_cells = %llu", path, frame_idx, res.frames[frame_idx].num_cells);
//
//		if (res.frames[frame_idx].num_cells > 0)
//		{
//			res.frames[frame_idx].cells = ArenaAlloc(&ctx->arena, res.frames[frame_idx].num_cells, SpriteCell);
//			size_t cell_idx = 0;
//
//			SDL_SeekIO(fs, fs_pos, SDL_IO_SEEK_SET);
//
//			for (size_t chunk_idx = 0; chunk_idx < frame->num_chunks; chunk_idx += 1)
//			{
//				void* raw_chunk;
//				size_t raw_chunk_size;
//				ASE_ChunkType chunk_type = ASE_ReadChunk(fs, stack, &raw_chunk, &raw_chunk_size);
//
//				ASE_CellChunk* chunk = raw_chunk;
//				if (chunk_type == ASE_ChunkType_Cell &&
//					chunk->layer_idx != hitbox_layer_idx &&
//					chunk->layer_idx != origin_layer_idx)
//				{
//
//					SpriteCell cell =
//					{
//							.origin.x = (int)chunk->x,
//							.origin.y = (int)chunk->y,
//							.z_idx = chunk->z_idx,
//							.layer_idx = (unsigned int)chunk->layer_idx,
//							.size.x = (int)chunk->w,
//							.size.y = (int)chunk->h,
//					};
//
//					assert(cell.size.x != 0 && cell.size.y != 0);
//					size_t dst_buf_size = cell.size.x * cell.size.y * sizeof(unsigned int);
//					cell.dst_buf = RL_MALLOC(dst_buf_size);
//
//					// It's the zero-sized array at the end of ASE_CellChunk.
//					size_t src_buf_size = raw_chunk_size - sizeof(ASE_CellChunk) - 2;
//					void* src_buf = (void*)((&chunk->h) + 1);
//
//					size_t res = INFL_ZInflate(cell.dst_buf, dst_buf_size, src_buf, src_buf_size);
//					assert(res > 0);
//
//					assert(cell_idx < res.frames[frame_idx].num_cells);
//					res.frames[frame_idx].cells[cell_idx++] = cell;
//				}
//
//				StackFree(stack, raw_chunk);
//			}
//
//			// Makes the cells draw in the correct order.
//			assert(frame_idx < res.num_frames);
//			SDL_qsort(
//				res.frames[frame_idx].cells,
//				res.frames[frame_idx].num_cells,
//				sizeof(SpriteCell),
//				(SDL_CompareCallback)CompareSpriteCells);
//		}
//	}
//
//	UnloadFileData(file_data);
//
//	res.ok = true;
//	return res;
// }

//static void LoadLevel(void)
//{
//	cJSON* head;
//	{
//		size_t file_len;
//
//		void* file_data = SDL_LoadFile("assets\\levels\\test.ldtk", &file_len);
//		SDL_CHECK(file_data);
//
//		SPALL_BUFFER_BEGIN_NAME("cJSON_ParseWithLength");
//		head = cJSON_ParseWithLength((const char*)file_data, file_len);
//		SPALL_BUFFER_END();
//
//		SDL_free(file_data);
//	}
//	SDL_assert(HAS_FLAG(head->type, cJSON_Object));
//
//	cJSON* level_nodes = cJSON_GetObjectItem(head, "levels");
//	cJSON* level_node = level_nodes->child;
//
//	cJSON* w = cJSON_GetObjectItem(level_node, "pxWid");
//	ctx->level.size.x = ((int32_t)cJSON_GetNumberValue(w)) / TILE_SIZE;
//
//	cJSON* h = cJSON_GetObjectItem(level_node, "pxHei");
//	ctx->level.size.y = ((int32_t)cJSON_GetNumberValue(h)) / TILE_SIZE;
//
//	size_t num_tiles = (size_t)(ctx->level.size.x * ctx->level.size.y);
//	ctx->level.tiles = ArenaAlloc(&ctx->arena, num_tiles, bool);
//
//	ctx->level.num_tile_layers = 3;
//	ctx->level.tile_layers = ArenaAlloc(&ctx->arena, ctx->level.num_tile_layers, TileLayer);
//
//	const char* layer_tiles = "Tiles";
//	const char* layer_props = "Props";
//	const char* layer_grass = "Grass";
//
//	ctx->level.num_entities = 1; // the player
//
//	const char* layer_player = "Player";
//	const char* layer_enemies = "Enemies";
//
//	cJSON* layer_instances = cJSON_GetObjectItem(level_node, "layerInstances");
//	cJSON* layer_instance;
//	cJSON_ArrayForEach(layer_instance, layer_instances)
//	{
//		cJSON* node_type = cJSON_GetObjectItem(layer_instance, "__type");
//		char* type = cJSON_GetStringValue(node_type); SDL_assert(type);
//		cJSON* node_ident = cJSON_GetObjectItem(layer_instance, "__identifier");
//		char* ident = cJSON_GetStringValue(node_ident); SDL_assert(ident);
//
//		if (SDL_strcmp(type, "Tiles") == 0)
//		{
//			TileLayer* tile_layer = NULL;
//			// TODO
//			if (SDL_strcmp(ident, layer_tiles) == 0)
//			{
//				tile_layer = &ctx->level.tile_layers[0];
//			}
//			else if (SDL_strcmp(ident, layer_props) == 0)
//			{
//				tile_layer = &ctx->level.tile_layers[1];
//			}
//			else if (SDL_strcmp(ident, layer_grass) == 0)
//			{
//				tile_layer = &ctx->level.tile_layers[2];
//			}
//			else
//			{
//				SDL_assert(!"Invalid layer!");
//			}
//
//			cJSON* grid_tiles = cJSON_GetObjectItem(layer_instance, "gridTiles");
//			cJSON* grid_tile;
//			cJSON_ArrayForEach(grid_tile, grid_tiles)
//			{
//				tile_layer->num_tiles += 1;
//			}
//			tile_layer->tiles = ArenaAlloc(&ctx->arena, tile_layer->num_tiles, Tile);
//			SDL_memset(tile_layer->tiles, -1, tile_layer->num_tiles * sizeof(Tile));
//		}
//		else if (SDL_strcmp(ident, layer_enemies) == 0)
//		{
//			cJSON* entity_instances = cJSON_GetObjectItem(layer_instance, "entityInstances");
//			cJSON* entity_instance;
//			cJSON_ArrayForEach(entity_instance, entity_instances)
//			{
//				ctx->level.num_entities += 1;
//			}
//		}
//		else if (SDL_strcmp(ident, "IntGrid") == 0)
//		{
//			cJSON* tile_collisions = cJSON_GetObjectItem(layer_instance, "intGridCsv"); SDL_assert(tile_collisions);
//			cJSON* tile_collision;
//			size_t tile_collision_idx = 0;
//			cJSON_ArrayForEach(tile_collision, tile_collisions)
//			{
//				bool val = (bool)cJSON_GetNumberValue(tile_collision);
//				ctx->level.tiles[tile_collision_idx++] = val;
//			}
//		}
//	}
//
//	ctx->level.entities = ArenaAlloc(&ctx->arena, ctx->level.num_entities, Entity);
//	Entity* enemy = &ctx->level.entities[1];
//
//	cJSON_ArrayForEach(layer_instance, layer_instances)
//	{
//		cJSON* node_type = cJSON_GetObjectItem(layer_instance, "__type");
//		char* type = cJSON_GetStringValue(node_type); SDL_assert(type);
//		cJSON* node_ident = cJSON_GetObjectItem(layer_instance, "__identifier");
//		char* ident = cJSON_GetStringValue(node_ident); SDL_assert(ident);
//		if (SDL_strcmp(type, "Tiles") == 0)
//		{
//			cJSON* grid_tiles = cJSON_GetObjectItem(layer_instance, "gridTiles");
//
//			// TODO
//			TileLayer* tile_layer = NULL;
//			if (SDL_strcmp(ident, layer_tiles) == 0)
//			{
//				tile_layer = &ctx->level.tile_layers[0];
//			}
//			else if (SDL_strcmp(ident, layer_props) == 0)
//			{
//				tile_layer = &ctx->level.tile_layers[1];
//			}
//			else if (SDL_strcmp(ident, layer_grass) == 0)
//			{
//				tile_layer = &ctx->level.tile_layers[2];
//			}
//			else
//			{
//				SDL_assert(!"Invalid layer!");
//			}
//
//			size_t i = 0;
//			cJSON* grid_tile;
//			cJSON_ArrayForEach(grid_tile, grid_tiles)
//			{
//				cJSON* src_node = cJSON_GetObjectItem(grid_tile, "src");
//				ivec2s src =
//				{
//					(int32_t)cJSON_GetNumberValue(src_node->child),
//					(int32_t)cJSON_GetNumberValue(src_node->child->next),
//				};
//
//				cJSON* dst_node = cJSON_GetObjectItem(grid_tile, "px");
//				ivec2s dst =
//				{
//					(int32_t)cJSON_GetNumberValue(dst_node->child),
//					(int32_t)cJSON_GetNumberValue(dst_node->child->next),
//				};
//
//				SDL_assert(i < tile_layer->num_tiles);
//				tile_layer->tiles[i++] = (Tile){ src, dst };
//			}
//		}
//		else if (SDL_strcmp(type, "Entities") == 0)
//		{
//			cJSON* entity_instances = cJSON_GetObjectItem(layer_instance, "entityInstances");
//
//			if (SDL_strcmp(ident, layer_player) == 0)
//			{
//				cJSON* entity_instance = entity_instances->child;
//				cJSON* world_x = cJSON_GetObjectItem(entity_instance, "__worldX");
//				cJSON* world_y = cJSON_GetObjectItem(entity_instance, "__worldY");
//				ctx->level.entities[0].start_pos = (ivec2s){ (int32_t)cJSON_GetNumberValue(world_x), (int32_t)cJSON_GetNumberValue(world_y) };
//			}
//			else if (SDL_strcmp(ident, layer_enemies) == 0)
//			{
//				cJSON* entity_instance; cJSON_ArrayForEach(entity_instance, entity_instances)
//				{
//					cJSON* identifier_node = cJSON_GetObjectItem(entity_instance, "__identifier");
//					char* identifier = cJSON_GetStringValue(identifier_node);
//					cJSON* world_x = cJSON_GetObjectItem(entity_instance, "__worldX");
//					cJSON* world_y = cJSON_GetObjectItem(entity_instance, "__worldY");
//					enemy->start_pos = (ivec2s){ (int32_t)cJSON_GetNumberValue(world_x), (int32_t)cJSON_GetNumberValue(world_y) };
//					if (SDL_strcmp(identifier, "Boar") == 0)
//					{
//						enemy->type = EntityType_Boar;
//					} // else if (SDL_strcmp(identifier, "") == 0) {}
//					enemy += 1;
//				}
//			}
//		}
//	}
//
//	cJSON_Delete(head);
//
//}
//
//#if TOGGLE_TESTS
//// PrintLevel
//{
//	uint8_t* buf = StackAllocRaw(&ctx->stack, ctx->level.size.val.x + 1, 1);
//	SDL_Log("level start");
//	for (size_t y = 0; y < (size_t)(ctx->level.size.val.y); y += 1)
//	{
//		for (size_t x = 0; x < (size_t)(ctx->level.size.val.x); x += 1)
//		{
//			buf[x] = ctx->level.tiles[x + y * (size_t)ctx->level.size.val.x];
//			if (buf[x] == 0) buf[x] = '0';
//			else buf[x] = '1';
//		}
//		buf[ctx->level.size.val.x] = 0;
//		SDL_Log((const char*)buf);
//	}
//	SDL_Log("level end");
//	StackFree(&ctx->stack, buf);
//}
//#endif