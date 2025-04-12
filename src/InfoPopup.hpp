#include "Includes.hpp"

class InfoPopup : public geode::Popup<> {

public:

    static InfoPopup* create();

private:

    bool setup();


};