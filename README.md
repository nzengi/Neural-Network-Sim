# Neural Network Simulation

A sophisticated neural network simulation that models the interaction between pyramidal (excitatory) and inhibitory neurons, featuring homeostatic plasticity and neuromodulation.

## Features

- Biologically-inspired neuron models
- Homeostatic plasticity mechanisms
- Neuromodulation (dopamine, serotonin, noradrenaline, acetylcholine)
- OpenMP parallelization for performance
- Web-based visualization interface

## Installation

### Prerequisites
- C compiler (gcc recommended)
- CMake (>= 3.10)
- Python 3.8+
- GSL (GNU Scientific Library)
- OpenMP

### Building

```bash
# Clone the repository
git clone https://github.com/yourusername/neural-network-sim.git
cd neural-network-sim

# Create build directory
mkdir build && cd build

# Build the project
cmake ..
make
```

### Setting up the Web Interface

```bash
# Create and activate virtual environment
python3 -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install requirements
pip install -r requirements.txt

# Run the web interface
cd web
python app.py
```

## Usage

1. Start the web interface (http://localhost:5000)
2. Configure simulation parameters:
   - Number of pyramidal neurons
   - Number of inhibitory neurons
   - Connection rate
   - Simulation time
3. Run the simulation and view results in real-time

## Configuration

The simulation can be configured through:
- Web interface
- Configuration files (see `config/default_config.ini`)
- Command line arguments

## Documentation

For detailed documentation, see the [Wiki](https://github.com/yourusername/neural-network-sim/wiki).

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Based on research in computational neuroscience
- Uses modern C11 features and OpenMP parallelization
- Web interface built with Flask and Plotly.js