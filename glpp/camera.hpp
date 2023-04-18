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
        Camera(const glm::vec3 &position, const glm::vec3 &up, const glm::vec3 &target, float maxspeed) : position(position), up(up), target(target), maxspeed(maxspeed) {}
        Camera() : position(glm::vec3(0.0f, 0.0f, 3.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), face(glm::vec3(0.0f, 0.0f, -1.0f)), target(face + position), maxspeed(3.3f), yaw(-90.0f), pitch(0.0f), roll(0.0f)
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
            return glm::lookAt(position, target, up);
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
        }
    };
} // namespace glpp
