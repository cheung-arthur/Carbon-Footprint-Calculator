#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>
#include <stdexcept>

using namespace std;

struct Co2Source {
    string source_name;
    string units;
    double co2_emission_factor;
    double quantity;

    Co2Source(string name, string unit, double factor, double qty = 0)
        : source_name(std::move(name)), units(std::move(unit)),
          co2_emission_factor(factor), quantity(qty) {}

    double calculateEmissions() const {
        return co2_emission_factor * quantity;
    }
};

class CarbonFootprint {
public:
    virtual ~CarbonFootprint() = default;
    virtual double getCarbonFootprint() const = 0;
    virtual string getInfo() const = 0;
};

class Building : public CarbonFootprint {
public:
    explicit Building(string building_name) : name(std::move(building_name)) {}

    double getCarbonFootprint() const override {
        double total_emissions = 0;
        for (const auto& source : co2_sources) {
            total_emissions += source.calculateEmissions();
        }
        if (total_emissions <= 0) {
            throw runtime_error("Emission data is incomplete for building: " + name);
        }
        return total_emissions;
    }

    string getInfo() const override {
        ostringstream s;
        s << "Building Name: " << name << "\n"
          << "Total CO2 Emissions: " << getCarbonFootprint() << " mt CO2\n";
        return s.str();
    }

    Building& addSource(const Co2Source& source) {
        if (source.quantity <= 0) {
            throw invalid_argument("Quantity must be positive for source: " + source.source_name);
        }
        co2_sources.push_back(source);
        return *this;
    }

private:
    vector<Co2Source> co2_sources;
    string name;
};

class Car : public CarbonFootprint {
public:
    Car(string model, double fuelEfficiency, Co2Source fuelType, double milesDriven)
        : model(std::move(model)), fuel_efficiency(fuelEfficiency), fuel_type(fuelType), mileage(milesDriven) {
        if (fuelEfficiency <= 0) {
            throw invalid_argument("Fuel efficiency must be positive for car: " + model);
        }
    }

    double getCarbonFootprint() const override {
        return fuel_type.calculateEmissions() * mileage / fuel_efficiency;
    }

    string getInfo() const override {
        ostringstream s;
        s << "Car Model: " << model << "\n"
          << "Fuel Efficiency: " << fuel_efficiency << " mpg\n"
          << "Total CO2 Emissions: " << getCarbonFootprint() << " mt CO2\n";
        return s.str();
    }

    Car& addSecondaryCo2Source(const Co2Source& source) {
        if (source.quantity <= 0) {
            throw invalid_argument("Quantity must be positive for source: " + source.source_name);
        }
        secondary_co2_sources.push_back(source);
        return *this;
    }

private:
    string model;
    Co2Source fuel_type;
    vector<Co2Source> secondary_co2_sources;
    double fuel_efficiency, mileage;
};

class Bicycle : public CarbonFootprint {
public:
    Bicycle(Co2Source frameMaterial, double hoursRidden, double riderWeight)
        : frame_material(frameMaterial), hours_ridden(hoursRidden), rider_weight(riderWeight) {
        if (riderWeight <= 0) {
            throw invalid_argument("Rider weight must be positive.");
        }
    }

    double getCarbonFootprint() const override {
        double sum = frame_material.calculateEmissions();
        for (const auto& source : co2_sources) {
            sum += source.calculateEmissions();
        }
        return sum;
    }

    string getInfo() const override {
        ostringstream s;
        s << "Bicycle Type: " << frame_material.source_name << "\n"
          << "Rider Weight: " << rider_weight << " kg\n"
          << "Riding Time: " << hours_ridden << " hours\n"
          << "Total CO2 Emissions: " << getCarbonFootprint() << " mt CO2\n";
        return s.str();
    }

    Bicycle& addSource(const Co2Source& source) {
        if (source.quantity <= 0) {
            throw invalid_argument("Quantity must be positive for source: " + source.source_name);
        }
        co2_sources.push_back(source);
        return *this;
    }

private:
    Co2Source frame_material;
    vector<Co2Source> co2_sources;
    double hours_ridden, rider_weight;
};

int main() {
    try {
        vector<unique_ptr<CarbonFootprint>> footprints;

        auto empire_state_building = make_unique<Building>("Empire State");
        empire_state_building->addSource(Co2Source("Natural Gas", "therms", 0.005307, 12345))
                             .addSource(Co2Source("Electricity", "kWh", 0.000707, 5000));
        footprints.push_back(move(empire_state_building));

        auto lamborghini = make_unique<Car>("Lamborghini Huracan Performante", 12, Co2Source("Gasoline", "gallons", 0.00889, 8000), 8000);
        lamborghini->addSecondaryCo2Source(Co2Source("Motor Oil", "quarts", 0.22933, 7.93));
        footprints.push_back(move(lamborghini));

        auto aluminum_bike = make_unique<Bicycle>(Co2Source("Aluminum", "", 0.25, 1), 320, 75);
        aluminum_bike->addSource(Co2Source("Tire Rubber", "kg", 0.0044, 3.8));
        footprints.push_back(move(aluminum_bike));

        for (const auto& footprint : footprints) {
            cout << footprint->getInfo() << endl;
        }
    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
