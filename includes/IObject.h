#ifndef IOBJECT_H
#define IOBJECT_H

#include <glm/glm.hpp>

#include <shader.hpp>

class IObject
{
    public:
        virtual                 ~IObject() = default;

        virtual int             Draw(Shader&) = 0;
        virtual glm::vec3       GetPos() = 0;
        virtual void            SetPos(glm::vec3) = 0;

    protected:

    private:
};

#endif // IOBJECT_H
