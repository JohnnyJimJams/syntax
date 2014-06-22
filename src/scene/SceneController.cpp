#include <scene/SceneController.h>
#include <scene/SceneObject.h>
#include <glm/ext.hpp>

namespace syn
{

void RotationController::update(SceneObject* a_object, float a_deltaTime)
{
	a_object->setLocalTransform(a_object->getLocalTransform() * glm::rotate<float>(m_angle * a_deltaTime, m_axis));
}

} // namespace syn