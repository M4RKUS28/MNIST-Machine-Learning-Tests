<p align="center">
  <img src="docs/logo.png" alt="MNIST-Net Logo" width="128">
</p>

<h1 align="center">MNIST-Net</h1>

<p align="center">
  A Qt-based desktop application for training and evaluating neural networks on the <a href="http://yann.lecun.com/exdb/mnist/">MNIST</a> handwritten digit dataset.
</p>

---

## Features

- **Live Training Visualisation** — real-time accuracy chart (test & train) powered by Qt Charts
- **Interactive Controls** — adjustable learning rate, momentum, and batch size during training
- **Architecture Editor** — modal dialog to design custom network topologies with configurable aggregation and activation functions
- **Save / Load Weights** — persist trained networks to CSV and resume later
- **Multi-Threaded Evaluation** — background accuracy evaluation with cancellable worker threads
- **Sample Preview** — displays the current training image with predicted and correct labels

## Screenshots

> _Start training and watch the accuracy converge in real time._

## Tech Stack

| Component      | Technology                               |
| -------------- | ---------------------------------------- |
| Language       | C++17                                    |
| UI Framework   | Qt 6 (Widgets, Charts)                   |
| Neural Network | [GenNet](../GenNet/) (custom NN library) |
| Build System   | qmake                                    |
| Platform       | Windows (MinGW / MSVC)                   |

## Project Structure

```
MNIST-Net/
├── src/
│   ├── main.cpp                  # Application entry point
│   ├── mainwindow.h/cpp/ui       # Main window UI and training loop
│   ├── architecturedialog.h/cpp  # Network architecture editor dialog
│   ├── backproptrainer.h/cpp     # Multi-threaded evaluation workers
│   └── datasetloader.h/cpp       # MNIST dataset loader (IDX format)
├── dataset/                      # MNIST data files (not tracked)
├── icons/                        # Application icons
├── docs/                         # Logo and documentation assets
├── installer/                    # Qt Installer Framework config
└── MNIST-Net.pro                 # qmake project file
```

## Prerequisites

- **Qt 6** with `core`, `gui`, `widgets`, and `charts` modules
- **MNIST dataset** — place the four IDX files in `./dataset/`:
  - `train-images-idx3-ubyte`
  - `train-labels-idx1-ubyte`
  - `t10k-images-idx3-ubyte`
  - `t10k-labels-idx1-ubyte`

## Building

```bash
# 1. Clone with submodules
git clone --recurse-submodules https://github.com/M4RKUS28/MNIST-Net.git
cd MNIST-Net

# 2. Build GenNet library first
cd libs/GenNet
qmake && make
cd ../..

# 3. Build MNIST-Net
qmake
make            # or: mingw32-make / nmake
```

## Default Architecture

```
784  INPUT LAYER     (28×28 pixel input)
256  SUM   TANH      (hidden layer)
 10  SUM   SMAX      (output — one per digit)
```

The architecture editor allows choosing from:

| Aggregation        | Activation                                               |
| ------------------ | -------------------------------------------------------- |
| SUM, AVG, MAX, MIN | TANH, RELU, SIGMOID, LEAKYRELU, IDENTITY, SOFTPLUS, SMAX |

## License

This project is provided as-is for educational and research purposes.
