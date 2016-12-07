

#ifndef MODULE_COMPONENT_H
#define MODULE_COMPONENT_H

namespace dfm {

class ModuleComponent {
public:
    virtual bool install() = 0;
    virtual bool uninstall() = 0;
};
}

#endif /* MODULE_COMPONENT_H */
