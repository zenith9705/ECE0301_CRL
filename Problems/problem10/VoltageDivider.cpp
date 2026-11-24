#include "VoltageDivider.hpp"
#include <stdexcept>

VoltageDivider::VoltageDivider(double v, double res[], int num_res) : Network(3, num_res+1) {
    if (num_res<1) {
        throw std::invalid_argument("Number of resistors must be greater than or equal to 1");
    }
    VoltageSource* vs = new VoltageSource(v);
    Network::set_component(0, vs);
    Network::set_component_value(0, v);
    Network::connect_component_terminal_a(0, 0);
    Network::connect_component_terminal_b(0, 1);
    Resistor* res_array[num_res];
    for(int i = 0; i < num_res; i++) {
        res_array[i] = new Resistor(res[i]);
        Network::set_component(i + 1, res_array[i]); //set the component to reference a resistor pointer
        Network::set_component_value(i + 1, res[i]); //set component value to the resistor value
        Network::connect_component_terminal_a(i+1, i+1);
        Network::connect_component_terminal_b(i+1, i+2%(num_res+1));
    }
}
int VoltageDivider::get_num_resistors() const {
    return Network::get_num_components() - 1; //subtract 1 for the voltage source
}
double VoltageDivider::get_source_voltage() const {
    return Network::get_component_value(0); //get the voltage source value
}
double VoltageDivider::get_resistance(int idx) const {
    if (idx<0 || idx >= get_num_resistors()) {
        throw std::out_of_range("Index out of range");
    }
    return Network::get_component_value(idx + 1); //get the resistor value at the specified index
}
double VoltageDivider::get_resistor_voltage(int idx) const {
    if (idx<0 || idx >= get_num_resistors()) {
        throw std::out_of_range("Index out of range");
    }
    return Network::get_component_voltage(idx+1); //first value in the component network is the source
}

double VoltageDivider::get_loop_current() const {
    return Network::get_component_current(1);
}

void VoltageDivider::set_source_voltage(double v){
    Network::set_component_value(0, v); //set the voltage source to the new value
}
void VoltageDivider::set_resistance(int idx, double r){
    if (idx<0 || idx >= get_num_resistors()) {
        throw std::out_of_range("Index out of range");
    }
    if(r < 0) {
        throw std::invalid_argument("Resistance cannot be negative");
    }
    Network::set_component_value(idx + 1, r); //set the resistor value at the specified index
}

void VoltageDivider::update_node_voltages() {
    int n = get_num_resistors();
    double total_resistance = 0;
    for(int i = 0; i<n; i++) {
        total_resistance += get_component_value(i+1);
    }
    double voltage = get_source_voltage();
    double current;
    if(total_resistance > 0) {
        current = voltage / total_resistance;
    }
    else {
        current = 0;
    }

    set_node_voltage(0, 0);
    set_node_voltage(1, voltage);
    for(int i=2; i<=n; i++) {
        set_node_voltage(i, get_node_voltage(i-1)-current*get_component_value(i-1));
    }
}