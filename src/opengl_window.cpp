// opengl_window_windows_setup.cpp : This file contains the 'main' function.
// Program execution begins and ends there.
//

/* Standard Libaries */
#include <windows.h>

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/* Third Party Libraries */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* local header files */
#include "audio.hpp"

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

std::string GetShaderCode(std::string &shader_file) {
    /* Get shader source code from a file as a string */
    std::ifstream read_file(shader_file);

    std::string shader_code;
    std::string line;

    while (getline(read_file, line)) {
        // add line to the string
        shader_code += line + "\n";
    }

    read_file.close();

    return shader_code;
}

int main(int argc, char *argv[]) {
    /* Shader File Path */
    std::string vertex_shader_path = "shader/shader.vert";
    std::string fragment_shader_path = "shader/shader.frag";

    /* Shader Source Code (GLSL code) */
    std::string vertex_shader_string = GetShaderCode(vertex_shader_path);
    std::string fragment_shader_string = GetShaderCode(fragment_shader_path);
    const char *vertex_shader_source = vertex_shader_string.c_str();
    const char *fragment_shader_source = fragment_shader_string.c_str();

    /* SDL_mixer */
    const int music_volume = 64;
    const int channels = 2;
    const int chunksize = 1024;
    const char *music_path = "audio/chill.mp3";

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create GLFW Window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                          "OpenGL Window", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW which loads all OpenGL functions
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        const char *glew_error = (const char *)glewGetErrorString(err);
        std::string glew_error_str(glew_error);
        std::string debug_msg = "Error: " + glew_error_str;
        OutputDebugStringA(debug_msg.c_str());
    } else {
        const char *glew_version = (const char *)glewGetString(GLEW_VERSION);
        std::string glew_version_str(glew_version);
        std::string debug_msg = "Status: Using GLEW: " + glew_version_str;
        OutputDebugStringA(debug_msg.c_str());
    }

    /* Build and compile the Shader */
    // vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int success = 0;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);

        const char *info_log_char_p = (const char *)info_log;
        std::string info_log_str(info_log_char_p);
        std::string debug_msg =
            "Error: Vertex Shader Compilation Failed: " + info_log_str;
        std::cout << debug_msg << std::endl;
    }

    // frament shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);

        const char *info_log_char_p = (const char *)info_log;
        std::string info_log_str(info_log_char_p);
        std::string debug_msg =
            "Error: Fragment Shader Compilation Failed: " + info_log_str;
        std::cout << debug_msg << std::endl;
    }

    // link shaders
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);

        const char *info_log_char_p = (const char *)info_log;
        std::string info_log_str(info_log_char_p);
        std::string debug_msg =
            "Error: Program Shader Compilation Failed: " + info_log_str;
        std::cout << debug_msg << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /*
     * setup vertex data and buffers
     * configure vertex attributes
     */

    float vertices[] = {
        // positions        // colors
        -0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F,  // left
        0.5F,  -0.5F, 0.0F, 0.0F, 1.0F, 0.0F,  // right
        0.0F,  0.5F,  0.0F, 0.0F, 0.0F, 1.0F,  // top
    };

    unsigned int vbo = 0;
    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader_program);

    /* Audio */
    Audio audio(channels, chunksize);

    audio.LoadMusic(music_path);
    audio.PlayMusic();

    Audio::ChangeVolume(music_volume);

    while (!glfwWindowShouldClose(window)) {
        /* Game loop */

        /* Render here */
        glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Dealocate Music and Audio
    audio.FreeResources();

    // Dealocate all resources once they have outlived their purpose
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    // Terminates the GLFW library
    glfwTerminate();

    return 0;
}
