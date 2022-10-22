#include "../Util/la.hpp"
#include "../UIElement.hpp"
#include "Component.hpp"

#include "string"

class UIComponent : public Component
{
public:
	DEFINE_COMPONENT(UIComponent);
	virtual ~UIComponent() = default;

	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;

	std::string text;
	std::string type;

private:
	std::shared_ptr<UIElement> element;
};