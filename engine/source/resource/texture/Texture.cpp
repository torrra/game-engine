#define STB_IMAGE_IMPLEMENTATION 
#include "resource/texture/Texture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

#define TEXTURE_RELATIVE_PATH "..\\assets\\"

engine::Texture::Texture(void)
	: m_texture(0), m_size(0, 0)
{
}

ENGINE_API void engine::Texture::LoadResource(const char* fileName)
{
	// Concatenate file path
	char relativePath[256] = TEXTURE_RELATIVE_PATH;
	strcat_s(relativePath, fileName);

	glGenTextures(1, &m_texture);

	int32 channelCount = 0;
	unsigned char* data = stbi_load(relativePath, &m_size[0], &m_size[1], &channelCount, 0);

	// Invalid file type or file not found
	if (!data)
	{
		std::printf("Failed to load texture %s\n", fileName);
		return;
	}

	GLenum format = SetFormat(channelCount);

	if (format == -1)
	{
		std::printf("Failed to load texture '%s'. Error: unsupported channel count.\n", fileName);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_size[0], m_size[1], 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set wrapping parameter
	SetWrapping(ETextureWrapping::REPEAT);

	// Set filtering parameter
	SetFiltering(ETextureFiltering::BILINEAR);

	stbi_image_free(data);
}

ENGINE_API void engine::Texture::UseTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

ENGINE_API void engine::Texture::RemoveTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void engine::Texture::SetWrapping(ETextureWrapping const& wrapMode)
{
	// Set wrapping parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

void engine::Texture::SetFiltering(ETextureFiltering const& filterMode)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
}

int32 engine::Texture::SetFormat(int32 channelCount)
{
	switch (channelCount)
	{
		case 1:
			return GL_RED;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
	}

	return -1;
}
