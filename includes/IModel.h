#ifndef IMODEL_H
#define IMODEL_H

#include <shader.hpp>

class IModel
{
    public:
        virtual                 ~IModel() = default;

        virtual void            Draw(Shader&) = 0;
};

#endif // IMODEL_H
