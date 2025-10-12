#include "Setting.hpp"

void Setting::setGroup(const QString group) {
    this->group = group;
}
void Setting::setName(const QString name) {
    this->name = name;
}
void Setting::setValue(const QString value) {
    this->value = value;
}
QString Setting::getGroup() const {
    return group;
}
QString Setting::getName() const {
    return name;
}
QString Setting::getValue() const {
    return value;
}