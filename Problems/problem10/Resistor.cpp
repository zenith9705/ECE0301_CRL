#include "Resistor.hpp"
#include <stdexcept>

void Resistor::set_value(double r)
{
  // TODO
  if(r < 0) {
    throw std::invalid_argument("Resistance cannot be negative");
  }
  else {
  Component::set_value(r);
  }
}

double Resistor::get_current() const
{
  // TODO
  double res = this->get_value();
  if(res == 0) {
    throw std::logic_error("Cannot divide by zero");
  }
  double voltage = this->get_voltage();
  return voltage / res;
}