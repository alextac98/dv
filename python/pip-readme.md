# DV - Dimensional Variables

DV (DimensionalVariable) is a Python library for handling physical quantities with automatic unit checking and conversion. It is a wrapper library to the core Rust library, which contains all the logic. Read more about the project (including extra docs) at https://dv.alextac.com.

## Features

- **Unit-safe math**: Add, subtract, multiply, divide with automatic unit checking
- **Easy conversions**: Convert between any compatible units
- **Comprehensive unit support**: SI units, imperial units, and more
- **Type safety**: Catch unit errors at runtime before they cause problems

## Quick Example

```python
from dv_py import DimensionalVariable as dv

# Create dimensioned variables
velocity = dv(10.0, "m/s")
time = dv(2.0, "s")

# Automatic dimensional analysis
distance = velocity * time
print(distance.value_in("m"))   # 20.0
print(distance.value_in("km"))  # 0.02

# Unit checking prevents errors
mass = dv(5.0, "kg")
distance + mass  # Raises DVError: incompatible units!
```

## Installation

```bash
pip install dv_py
```

## Documentation

Full documentation, examples, and guides at: **https://dv.alextac.com**

## License

This project is licensed under the Apache License, Version 2.0 (the "License"). You may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Copyright © 2025 Alex Tacescu
