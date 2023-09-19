#include "Texture.h"

#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define stringify( name ) #name
Texture::Texture(const std::string& filepath, TextureType textureType, glm::vec2 uvScale)
{
    this->textureType = textureType;
    this->uvScale = uvScale;
    stbi_set_flip_vertically_on_load(true);  
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, channelsInFile;
    
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &channelsInFile, 0);
    if (data)
    {
        GLenum imageFormat;
        if (channelsInFile == 1)
            imageFormat = GL_RED;
        else if (channelsInFile == 3)
            imageFormat = GL_RGB;
        else if (channelsInFile == 4)
            imageFormat = GL_RGBA;
        
        glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

std::string Texture::GetSamplerName(int index)
{
    return "material" + std::to_string(index) + "." + stringify(textureType);
}

void Texture::Activate(int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}
