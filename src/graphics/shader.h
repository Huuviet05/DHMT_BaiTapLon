#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Shader {
public:
    GLuint ID;

    Shader();
    ~Shader();

    void compile(const char* vertexPath, const char* fragmentPath);
    void use() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat4(const std::string& name, float* mat) const;

private:
    std::string readFile(const char* filePath) const;
    void checkCompileErrors(GLuint shader, const std::string& type) const;
};

#endif // SHADER_H
