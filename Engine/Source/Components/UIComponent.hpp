#include "../Util/la.hpp"
#include "../UI/UIElement.hpp"
#include "Component.hpp"

#include <vector>

// The UILayer's purpose is to store UIElement(s) and render them (by calling Draw()).
// Input handling is done separately in the UIElement itself.
class UILayer : public Component
{
public:
	DEFINE_COMPONENT(UILayer);
	virtual ~UILayer() = default;

	void OnUpdate() override;

	void PushElement(std::shared_ptr<UIElement> element);
	void ClearElements();

private:
	std::vector<std::shared_ptr<UIElement>> m_Elements;
};