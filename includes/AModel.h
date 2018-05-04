#ifndef AMODEL_H
#define AMODEL_H

#include <glad\glad.h>
#include <string>
#include <glm/glm.hpp>

class AModel
{
    public:
        virtual                 ~AModel() = default;

        virtual unsigned int    GetShaderId() = 0;
        virtual void            UseShaderProgram() { glUseProgram(_shaderProgram); }
        virtual void            Draw() = 0;

        unsigned int            makeShader(GLenum, const char **);
        unsigned int            linkShaders(unsigned int *, unsigned int);

        void                    SetBool(const std::string &, bool) const;
        void                    SetInt(const std::string &, int) const;
        void                    SetFloat(const std::string &, float) const;
        void                    SetVec2(const std::string &, const glm::vec2&) const;
        void                    SetVec3(const std::string &, const glm::vec3&) const;
        void                    SetVec4(const std::string &, const glm::vec4&) const;
        void                    SetMat2(const std::string &, const glm::mat2&) const;
        void                    SetMat3(const std::string &, const glm::mat3&) const;
        void                    SetMat4(const std::string &, const glm::mat4&) const;

    protected:
        unsigned int            _shaderProgram;
};

#endif // IMODEL_H
