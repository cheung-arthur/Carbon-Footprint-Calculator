# Carbon Footprint Calculator

This C++ application calculates and reports the carbon footprint of various entities such as buildings, vehicles, and bicycles. Designed with an educational goal, it aims to highlight the environmental impact of everyday objects and activities.

## Features

- **Modular Design**: Utilizing an object-oriented approach, the application's structure is based on the `CarbonFootprint` abstract base class, ensuring clarity and extensibility.
- **Smart Memory Management**: Implements modern C++ smart pointers for dynamic memory management, enhancing stability and reducing memory leak risks.
- **Exception Handling**: Equipped with robust exception handling, the application improves reliability and user experience through informative error messaging.
- **Customizable CO2 Sources**: Allows for detailed and accurate carbon footprint calculations by adding multiple CO2 emission sources to each entity.
- **Readable Output**: Generates comprehensive reports on the carbon footprint of each entity in a user-friendly format.

## Classes

- `Co2Source`: Defines sources of CO2 emissions.
- `CarbonFootprint`: An abstract base class for calculating carbon footprints.
- `Building`: Calculates footprint based on various energy sources.
- `Car`: Accounts for fuel consumption and other emissions related to vehicles.
- `Bicycle`: Considers the materials and human energy expenditure.

## Getting Started

### Prerequisites

Ensure you have a C++ compiler that supports the C++17 standard.

### Compilation

To compile the program, navigate to the program's directory and run:

```bash
g++ -std=c++17 -o CarbonFootprintCalculator Carbon_Footprint_Calculator.cpp
