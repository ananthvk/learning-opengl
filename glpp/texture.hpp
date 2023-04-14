#include <string>
#include<string.h>
#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_img.h"

namespace glpp
{
    class Texture
    {
    private:
        unsigned int id;
        std::string name;

    public:
        Texture() : id(0), name("undefined") {}
        bool load_from_file(const std::string &texture_path, GLenum internal_format = GL_RGB, GLenum format = GL_RGB)
        {
            int width = 0, height = 0, nchannels = 0;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nchannels, 0);
            if (!data)
            {
                std::cerr << "ERROR: Unable to load texture " << name << " from " << texture_path << std::endl;
                return false;
            }

            // Generate the textures
            unsigned int tid;
            glGenTextures(1, &tid);
            glBindTexture(GL_TEXTURE_2D, tid);

            // Set various parameters such as mipmaps, interpolation and repeat
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // Free the pixel data loaded by stb
            stbi_image_free(data);
            id = tid;
            return true;
        }
        unsigned int get_id()
        {
            return id;
        }
        std::string get_name()
        {
            return name;
        }
        void set_name(const std::string &s)
        {
            name = s;
        }
        // Activates the given texture at texture_unit
        // texture_unit can take values from (0 to 15)
        // as OpenGL has a minimum of 16 texture units
        void activate(int loc)
        {
            if (id == 0)
            {
                std::cerr << "ERROR: Texture " << name << " cannot be activated before texture is loaded" << std::endl;
                return;
            }
            if (loc < 0 || loc >= 16)
            {
                std::cerr << "ERROR: Texture " << name << " activation index invalid (should be between 0(incl) and 15(incl))" << std::endl;
                return;
            }
            glActiveTexture(GL_TEXTURE0 + loc);
            glBindTexture(GL_TEXTURE_2D, id);
        }
    };
};