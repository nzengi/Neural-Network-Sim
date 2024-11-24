# Advanced Neural Network Simulation

A state-of-the-art neural network simulation incorporating modern neuroscience discoveries, including dendritic computation, synaptic plasticity, neuromodulation, and homeostatic mechanisms.

## Table of Contents
- [Overview](#overview)
- [New Features](#new-features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Technical Details](#technical-details)
- [Output Files](#output-files)
- [Data Analysis](#data-analysis)
- [Contributing](#contributing)
- [License](#license)

## Overview

This advanced simulation implements a biologically-detailed neural network model that incorporates recent discoveries in neuroscience. The model features:

- Realistic dendritic computation
- Multiple forms of synaptic plasticity
- Neuromodulatory systems
- Homeostatic scaling
- Burst detection and processing
- Correlated neural noise
- Advanced calcium dynamics

## New Features

### 1. Dendritic Computation
- Multiple dendritic segments per neuron
- Local NMDA spikes
- Nonlinear dendritic integration
- Compartmentalized synaptic plasticity
- Branch-specific calcium dynamics

### 2. Advanced Synaptic Plasticity
- Spike-Timing-Dependent Plasticity (STDP)
- Metaplasticity mechanisms
- Homeostatic scaling
- Activity-dependent modification
- Synaptic trace mechanisms

### 3. Neuromodulation
```c
typedef struct {
    double dopamine;
    double serotonin;
    double noradrenaline;
    double acetylcholine;
} Neuromodulators;
```
- Implementation of major neuromodulatory systems
- Burst-dependent modulation
- Plasticity modification
- Behavioral state simulation

### 4. Homeostatic Mechanisms
```c
typedef struct {
    double threshold_baseline;
    double adaptation_rate;
    double target_activity;
    double current_activity;
} HomeostaticScaling;
```
- Target activity maintenance
- Adaptive threshold adjustment
- Synaptic scaling
- Metaplasticity regulation

### 5. Burst Detection
- Real-time burst analysis
- Burst-triggered plasticity
- Neuromodulator release
- Pattern detection capabilities

## Requirements

### System Requirements
- GCC compiler (version 8.0 or higher)
- Make build system
- Minimum 16GB RAM (32GB recommended)
- Multi-core processor recommended
- POSIX-compliant operating system

### Dependencies
- C Standard Library
- Math Library (libm)
- Standard C Time Library
- OpenMP (optional, for parallelization)
- GSL (GNU Scientific Library) for advanced mathematical functions

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/advanced-neural-simulation.git
cd advanced-neural-simulation
```

2. Configure build options:
```bash
./configure --enable-openmp --with-gsl
```

3. Compile the source code:
```bash
make
```

4. Run tests:
```bash
make test
```

## Usage

### Basic Usage
```bash
./neural_sim [options]
```

### Command Line Arguments
```bash
Options:
  -c, --config <file>      Specify configuration file
  -o, --output <dir>       Specify output directory
  -t, --time <seconds>     Simulation duration
  -s, --seed <number>      Random seed
  -n, --neurons <number>   Number of neurons
  -d, --dendrites <number> Dendrites per neuron
  -p, --plasticity <type>  Plasticity rule (stdp/hebbian/homeostatic)
  -m, --modulation <bool>  Enable neuromodulation
  -b, --burst <bool>       Enable burst detection
  -v, --verbose           Enable verbose output
  --gpu                    Use GPU acceleration (if available)
```

### Example Configurations

1. Basic Simulation:
```bash
./neural_sim --config basic.conf --time 10.0
```

2. Advanced Features:
```bash
./neural_sim --config advanced.conf --dendrites 20 --plasticity stdp --modulation true
```

3. Large-Scale Simulation:
```bash
./neural_sim --neurons 1000 --dendrites 50 --gpu --output large_sim/
```

## Configuration

### Advanced Network Parameters

```ini
[Network]
num_pyramidal = 1000
num_inhibitory = 200
dt = 0.00001
connection_rate = 0.1

[Dendrites]
num_dendrites = 20
num_synapses_per_dendrite = 100
dendrite_coupling = 0.5
nmda_threshold = 0.8

[Plasticity]
stdp_window = 0.020
stdp_rate = 0.01
meta_plasticity_rate = 0.001
homeostatic_tau = 1000.0

[Neuromodulation]
dopamine_baseline = 1.0
serotonin_baseline = 1.0
modulator_decay = 0.100
burst_threshold = -50.0

[Homeostasis]
target_rate = 10.0
adaptation_rate = 0.01
scaling_tau = 1000.0
```

## Technical Details

### Dendritic Computation Model

The simulation implements a multi-compartmental dendritic model:

1. Local Potential Evolution:
```
dV_dend/dt = (-g_leak*(V_dend-E_leak) + I_syn + I_NMDA + I_coupling)/C_dend
```

2. NMDA Spike Generation:
```
I_NMDA = g_NMDA * s_NMDA * (V_dend-E_NMDA) * Mg_block(V_dend)
```

3. Dendritic Integration:
```
V_soma = sum(w_i * V_dend_i) + V_baseline
```

### Advanced Plasticity Rules

1. STDP with Metaplasticity:
```
dw/dt = A+ * exp(-Δt/τ+) * M(w)  for Δt > 0
dw/dt = -A- * exp(Δt/τ-) * M(w)  for Δt < 0
```

2. Homeostatic Scaling:
```
dθ/dt = (r_current - r_target)/τ_homeo
```

### Neuromodulation Effects

The impact of neuromodulators on synaptic plasticity:
```
Δw_effective = Δw_base * (1 + α_DA * [DA] + α_ACh * [ACh])
```

## Output Files

The simulation generates several detailed output files:

1. `neuron_activity.txt`:
   ```
   time neuron_id voltage calcium burst_state modulator_levels
   ```

2. `synaptic_weights.txt`:
   ```
   time pre_id post_id dendrite_id weight meta_plasticity
   ```

3. `population_statistics.txt`:
   ```
   time mean_rate burst_frequency average_weight modulator_levels
   ```

### Data Analysis

Example Python script for analyzing burst patterns:

```python
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

def analyze_bursts(filename):
    # Load data
    data = np.loadtxt(filename)
    time = data[:, 0]
    voltage = data[:, 1]
    
    # Detect bursts
    burst_threshold = -50
    burst_indices = np.where(voltage > burst_threshold)[0]
    
    # Analyze burst patterns
    burst_intervals = np.diff(time[burst_indices])
    
    # Plot results
    plt.figure(figsize=(12, 6))
    plt.hist(burst_intervals, bins=50, density=True)
    plt.xlabel('Inter-burst Interval (ms)')
    plt.ylabel('Probability Density')
    plt.title('Burst Interval Distribution')
    plt.show()
    
    return burst_intervals

# Usage
intervals = analyze_bursts('neuron_activity.txt')
print(f"Mean burst interval: {np.mean(intervals):.2f} ms")
```

## Contributing

We welcome contributions! Areas of particular interest:

1. New Features
   - Additional neuron types
   - More complex dendrite morphologies
   - Glial cell interactions
   - Extracellular field effects
   - Detailed ion channel dynamics

2. Performance Improvements
   - GPU acceleration
   - Parallel processing
   - Memory optimization
   - Numerical method improvements

3. Analysis Tools
   - Real-time visualization
   - Advanced analytics
   - Machine learning integration
   - Pattern detection algorithms

### Development Process
1. Fork the repository
2. Create a feature branch
3. Implement your changes
4. Add tests and documentation
5. Submit a pull request

### Coding Standards
- Follow C99 standard
- Use consistent indentation (4 spaces)
- Document all functions and complex code sections
- Include unit tests for new features
- Follow neuroscientific naming conventions

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Citation

If you use this software in your research, please cite:

```bibtex
@software{advanced_neural_sim_2024,
  title = {Advanced Neural Network Simulation},
  author = {Author, A.},
  year = {2024},
  url = {https://github.com/yourusername/advanced-neural-simulation},
  version = {2.0.0}
}
```

---

For more information, bug reports, or feature requests, please:
- Open an issue in the GitHub repository
- Contact the maintainers
- Check our documentation