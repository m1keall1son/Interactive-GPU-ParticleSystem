#pragma once
#include "cinder/CinderResources.h"

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )

#define RES_DRAW_VERT       CINDER_RESOURCE( ../resources/, drawShader_vert.glsl, 128, GLSL )
#define RES_DRAW_FRAG		CINDER_RESOURCE( ../resources/, drawShader_frag.glsl, 129, GLSL )
#define RES_UPDATE_FRAG		CINDER_RESOURCE( ../resources/, updateShader_frag.glsl, 130, GLSL )
#define RES_UPDATE_PASS		CINDER_RESOURCE( ../resources/, update_passVert.glsl, 131, GLSL )


