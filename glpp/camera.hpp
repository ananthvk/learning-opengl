#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace glpp
{
    class Camera
    {
    private:
        glm::vec3 position;
        glm::vec3 up;
        glm::vec3 target;

    public:
        Camera(const glm::vec3 &position, const glm::vec3 &up, const glm::vec3 &target) : position(position), up(up), target(target) {}
        Camera() : position(glm::vec3(0.0f, 0.0f, 3.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), target(glm::vec3(0.0f, 0.0f, 0.0f)) {}
        glm::vec3& vpos()
        {
            return position;
        }
        glm::vec3& vup()
        {
            return up;
        }
        glm::vec3& vtarget()
        {
            return target;
        }
        glm::mat4 view()
        {
            return glm::lookAt(position, target, up);
        }
    };
} // namespace glpp
