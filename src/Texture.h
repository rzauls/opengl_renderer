#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // texture image loader

class Texture
{
public:
	// texture id
	unsigned int ID;

	// read file and set up texture object
	Texture(const char *imagePath, GLenum format)
	{
		stbi_set_flip_vertically_on_load(true); // flip images on load

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		// wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int img_w, img_h, nrChannels;
		unsigned char *tex_data = stbi_load(imagePath, &img_w, &img_h, &nrChannels, 0);
		if (tex_data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, tex_data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			spdlog::error("Failed to load texture file");
		}
		stbi_image_free(tex_data);
	}
};
#endif // !TEXTURE_H