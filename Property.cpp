#include "Property.hpp"
void Property::setName(const QString name) {
    this->name = name;
}
void Property::setValue(const QString value) {
    this->value = value;
}
QString Property::getName() const {
    return name;
}
QString Property::getValue() const {
    return value;
}
