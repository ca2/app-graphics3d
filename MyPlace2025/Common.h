#pragma once




#include <iostream>

struct Transform {
    floating_sequence3 position = floating_sequence3(0.0f);
    floating_sequence3 rotation = floating_sequence3(0.0f);
    floating_sequence3 scale = floating_sequence3(1.0f);

    floating_matrix4 GetModelMatrix() const {
        floating_matrix4 m = glm::translate(floating_matrix4(1.0f), position);
        m *= glm::mat4_cast(glm::quat(rotation));
        m = glm::scale(m, scale);
        return m;
    }
};

 