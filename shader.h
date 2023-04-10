#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <fstream>

namespace gl
{
    class Shader
    {
    private:
        // The id of the created shader
        // -1 indicates that the shader has not yet been created
        unsigned int id;
        // Name of this shader
        std::string name;

    public:
        void use()
        {
            glUseProgram(id);
        }
        unsigned int getid()
        {
            return id;
        }
        void set_boo(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
        }
        void set_int(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(id, name.c_str()), value);
        }
        void set_float(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(id, name.c_str()), value);
        }

        Shader(const std::string &name, const std::string &vertex_shader_path, const std::string &fragment_shader_path) : id(-1)
        {
            int success;
            std::ifstream v, f;
            std::string vsrc, fsrc;
            v.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                // Read the shader programs
                v.open(vertex_shader_path);
                f.open(fragment_shader_path);
                std::stringstream vs, fs;
                vs << v.rdbuf();
                fs << f.rdbuf();
                vsrc = vs.str();
                fsrc = fs.str();

                // Create the shaders
                unsigned int vertex_shader, fragment_shader;
                vertex_shader = glCreateShader(GL_VERTEX_SHADER);
                fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

                if (!vertex_shader || !fragment_shader)
                {
                    std::cerr << "ERROR: Error while creating shaders for " << name << std::endl;
                }

                const char *vsrc_ptr = vsrc.c_str();
                const char *fsrc_ptr = fsrc.c_str();

                // Compile the shaders
                glShaderSource(vertex_shader, 1, &vsrc_ptr, NULL);
                glCompileShader(vertex_shader);
                glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

                if (!success)
                {
                    char buffer[1024];
                    std::cerr << "ERROR: Error while compiling vertex shader for " << name << std::endl;
                    glGetShaderInfoLog(vertex_shader, 1024, NULL, buffer);
                    std::cerr << buffer << std::endl;
                }

                glShaderSource(fragment_shader, 1, &fsrc_ptr, NULL);
                glCompileShader(fragment_shader);
                glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    char buffer[1024];
                    std::cerr << "ERROR: Error while compiling fragment shader for " << name << std::endl;
                    glGetShaderInfoLog(fragment_shader, 1024, NULL, buffer);
                    std::cerr << buffer << std::endl;
                }

                // Link the shader program
                unsigned int tmpid;
                tmpid = glCreateProgram();
                glAttachShader(tmpid, vertex_shader);
                glAttachShader(tmpid, fragment_shader);
                glLinkProgram(tmpid);

                glGetProgramiv(id, GL_LINK_STATUS, &success);
                if (!success)
                {
                    std::cerr << "ERROR: Error while linking shaders" << std::endl;
                    char buffer[1024];
                    glGetProgramInfoLog(tmpid, 1024, NULL, buffer);
                    std::cerr << buffer << std::endl;
                }
                id = tmpid;
                glDeleteShader(vertex_shader);
                glDeleteShader(fragment_shader);
            }
            catch (const std::ifstream::failure &e)
            {
                std::cerr << "ERROR: Error while loading shaders for " << name << " : " << e.what() << std::endl;
            }
        }
    };
};