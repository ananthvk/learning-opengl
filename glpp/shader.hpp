#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <fstream>
#include "common.hpp"

namespace glpp
{
    class Shader
    {
    private:
        unsigned int id;
        std::string name;
        unsigned int load_shader_from_file(const std::string &shader_path, GLenum type)
        {
            std::ifstream shader_stream;
            std::stringstream ssrc;
            std::string src;

            shader_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            // Load the shader
            try
            {
                shader_stream.open(shader_path);
                ssrc << shader_stream.rdbuf();
                src = ssrc.str();
            }
            catch (const std::ifstream::failure &f)
            {
                std::cerr << "ERROR: Failed to load shader for (" << name << ") : " << f.what() << std::endl;
                return 0;
            }

            // Create and compile the shader
            const char *src_ptr = src.c_str();
            unsigned int shader = glCreateShader(type);
            glShaderSource(shader, 1, &src_ptr, NULL);
            glCompileShader(shader);

            // Check if compilation succeeded
            int status;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (!status)
            {
                char buffer[ERROR_BUFFER_SIZE];
                glGetShaderInfoLog(shader, ERROR_BUFFER_SIZE, NULL, buffer);
                std::cerr << "ERROR: Error while loading/compiling shader for " << name << " : " << buffer << std::endl;
                return 0;
            }
            return shader;
        }

    public:
        Shader() : id(0), name("undefined")
        {
        }
        // Loads the vertex and fragment shader from a file on the disk
        bool load_from_file(const std::string &vertex_path, const std::string &fragment_path)
        {
            unsigned int vshader = load_shader_from_file(vertex_path, GL_VERTEX_SHADER);
            unsigned int fshader = load_shader_from_file(fragment_path, GL_FRAGMENT_SHADER);
            if (!vshader || !fshader)
            {
                std::cerr << "ERROR: Failed to load shaders" << std::endl;
                return false;
            }
            // Create a new shader program
            unsigned int program = glCreateProgram();
            glAttachShader(program, vshader);
            glAttachShader(program, fshader);
            glLinkProgram(program);

            // Check link status
            int status = 1 ;
            glGetProgramiv(id, GL_LINK_STATUS, &status);
            if (!status)
            {
                std::cerr << "ERROR: Error while linking shaders" << std::endl;
                char buffer[ERROR_BUFFER_SIZE];
                glGetProgramInfoLog(program, ERROR_BUFFER_SIZE, NULL, buffer);
                std::cerr << buffer << std::endl;
            }
            // Delete the shaders
            glDeleteShader(vshader);
            glDeleteShader(fshader);
            id = program;
            return true;
        }
        // Applies the shader program.
        void use()
        {
            if (id)
                glUseProgram(id);
            else
                std::cerr << "ERROR: Invalid shader " << name << " : Not loaded completely" << std::endl;
        }
        // Sets the name of the shader for easier debugging
        void set_name(const std::string &s)
        {
            name = s;
        }
        // Creates a uniform with the given keyname and sets it to value.
        void set(const std::string &keyname, float value)
        {
            unsigned int loc = glGetUniformLocation(id, keyname.c_str());
            glUniform1f(loc, value);
        }
        // Creates a uniform with the given keyname and sets it to value.
        void set(const std::string &keyname, int value)
        {
            unsigned int loc = glGetUniformLocation(id, keyname.c_str());
            glUniform1i(loc, value);
        }
        // Creates a uniform with the given keyname and sets it to value.
        void set(const std::string &keyname, glm::mat4 value)
        {
            unsigned int loc = glGetUniformLocation(id, keyname.c_str());
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        }
        // Returns the id of the current shader program
        unsigned int get_id()
        {
            return id;
        }
    };
};