#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace glpp
{
    class Camera
    {
    public:
        enum Direction
        {
            FORWARD,
            RIGHT,
            BACKWARD,
            LEFT
        };
        bool custom_lookat;
        bool fps;
        float ypos;

    private:
        glm::vec3 position;
        glm::vec3 up;
        glm::vec3 face;
        glm::vec3 target;
        float yaw;
        float pitch;
        float roll;
        float maxspeed;

    public:
        Camera() : position(glm::vec3(0.0f, 3.0f, 0.0f)),
                   up(glm::vec3(0.0f, 1.0f, 0.0f)),
                   face(glm::vec3(0.0f, 0.0f, -1.0f)),
                   target(face + position),
                   maxspeed(3.3f),
                   yaw(-90.0f),
                   pitch(0.0f),
                   roll(0.0f),
                   fps(false),
                   ypos(0.0f),
                   custom_lookat(true)
        {
            calculate_target_from_face();
            calculate_face();
        }
        glm::vec3 &vpos()
        {
            return position;
        }
        glm::vec3 &vup()
        {
            return up;
        }
        glm::vec3 &vtarget()
        {
            return target;
        }
        glm::vec3 &vface()
        {
            return face;
        }
        float &fyaw()
        {
            return yaw;
        }
        float &fpitch()
        {
            return pitch;
        }
        float &froll()
        {
            return roll;
        }
        void calculate_target_from_face()
        {
            target = face + position;
        }
        void calculate_face()
        {
            face.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            face.y = sin(glm::radians(pitch));
            face.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            face = glm::normalize(face);
        }
        glm::mat4 view()
        {
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::rotate(transform, glm::radians(roll), face);
            glm::vec3 realup = glm::vec3(transform * glm::vec4(up, 1.0f));

            if (!custom_lookat)
                return glm::lookAt(position, target, realup);

            // To describe the camera space, we need 3 unit vectors
            // Direction, right and up vector

            glm::vec3 direction = glm::normalize(position - target);
            glm::vec3 right = glm::normalize(glm::cross(realup, direction));
            glm::vec3 upv = glm::cross(direction, right);

            glm::mat4 rotation_matrix(1.0f);
            glm::mat4 translation_matrix(1.0f);

            // First column
            rotation_matrix[0][0] = right.x;
            rotation_matrix[0][1] = upv.x;
            rotation_matrix[0][2] = direction.x;
            rotation_matrix[0][3] = 0;

            // Second column
            rotation_matrix[1][0] = right.y;
            rotation_matrix[1][1] = upv.y;
            rotation_matrix[1][2] = direction.y;
            rotation_matrix[1][3] = 0;

            // Third column
            rotation_matrix[2][0] = right.z;
            rotation_matrix[2][1] = upv.z;
            rotation_matrix[2][2] = direction.z;
            rotation_matrix[2][3] = 0;

            // Fourth column
            rotation_matrix[3][0] = 0;
            rotation_matrix[3][1] = 0;
            rotation_matrix[3][2] = 0;
            rotation_matrix[3][3] = 1;

            translation_matrix = glm::translate(translation_matrix, glm::vec3(-position.x, -position.y, -position.z));

            // First we have to translate the camera then rotate it
            // So specify in reverse order, since we read right to left
            return rotation_matrix * translation_matrix;


        }
        void move(Direction direction, float dt)
        {
            float displ = maxspeed * dt;
            if (direction == FORWARD)
            {
                position += displ * face;
            }
            if (direction == BACKWARD)
            {
                position -= displ * face;
            }
            if (direction == RIGHT)
            {
                position += displ * glm::normalize(glm::cross(face, up));
            }
            if (direction == LEFT)
            {
                position -= displ * glm::normalize(glm::cross(face, up));
            }
            if (fps)
            {
                position.y = ypos;
            }
        }
    };
} // namespace glpp
