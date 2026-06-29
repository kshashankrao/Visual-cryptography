# Visual Cryptography: Successive Reading of Dual-Layer Images

This repository provides a C++ implementation of the visual cryptography schemes discussed in the paper:

> **"Visual Communication with Successive Reading of Public and Secret Information by Generating Dual-Layer Images"**  
> *Karim Hammoudi, Halim Benhabiles, Mahmoud Melkemi, and Shashank Rao Kadapanatham*  
> Published in **Expressive 2019 (Posters, Demos, and Artworks)**.  
> [Paper Link / Digital Library](http://diglib.eg.org/handle/10.2312/exp20191091)

---

## 📖 Theory and Mathematical Formulation

Visual Cryptography (VC) is a cryptographic technique that allows visual information (e.g., pictures or text) to be encrypted in such a way that decryption can be performed by the human visual system (specifically, by superimposing shares printed on transparent sheets) without requiring computers.

This repository implements two schemes:
1. **Classic (2, 2) Visual Cryptography (Naor-Shamir Scheme)**
2. **Dual-Layer Visual Cryptography (The Paper's Scheme)**

---

### 1. Classic (2, 2) Visual Cryptography

In a classic $2$-out-of-$2$ visual cryptography scheme, a secret binary image is split into two shares (Share 1 and Share 2). Each pixel $s$ of the secret image is represented by a block of $2$ subpixels in each share.

Let the black pixel be denoted by $0$ and the white pixel by $255$.

#### Case 1: Secret Pixel is White ($s = 255$)
We choose the same subpixel configuration for both shares. A random bit $A \in \{0, 1\}$ is generated:

$$
\text{Share 1} = \begin{cases} [0, 255] & \text{if } A = 1 \\ [255, 0] & \text{if } A = 0 \end{cases}, \quad \text{Share 2} = \begin{cases} [0, 255] & \text{if } A = 1 \\ [255, 0] & \text{if } A = 0 \end{cases}
$$

*Superposition (using point-wise minimum):*

$$
\text{Share 1} \oplus \text{Share 2} = \min(\text{Share 1}, \text{Share 2}) = [0, 255] \text{ or } [255, 0]
$$

The superimposed pixel contains one black and one white subpixel (perceived as 50% gray by the human eye).

#### Case 2: Secret Pixel is Black ($s = 0$)
We choose complementary subpixel configurations for the two shares:

$$
\text{Share 1} = \begin{cases} [0, 255] & \text{if } A = 1 \\ [255, 0] & \text{if } A = 0 \end{cases}, \quad \text{Share 2} = \begin{cases} [255, 0] & \text{if } A = 1 \\ [0, 255] & \text{if } A = 0 \end{cases}
$$

*Superposition (using point-wise minimum):*

$$
\text{Share 1} \oplus \text{Share 2} = \min(\text{Share 1}, \text{Share 2}) = [0, 0]
$$

The superimposed pixel is 100% black.

Since the selection of subpixels depends on the random bit $A$, looking at any single share reveals nothing but random noise (entropy is maximized, giving $0$ information leakage).

---

### 2. Dual-Layer Visual Cryptography (Successive Reading)

Traditional visual cryptography shares look like meaningless random noise. This can be suspicious or visually unappealing. 

The paper introduces **Dual-Layer Images**, where:
1. **Layer 1 (Public Layer):** Individual shares are legible and contain meaningful public information (e.g. a sequence of text characters).
2. **Layer 2 (Secret Layer):** Superimposing the two shares obliterates the public characters in specific regions, revealing a secret binary image silhouette.

#### Algorithm Mechanics

Let $T$ be a public text sequence (e.g. `"ABCD"`). For each character in the text, we pre-calculate two binary templates of size $H \times W$ (typically $20 \times 20$):
* **Positive Template ($P$):** The character is drawn in white ($255$) on a black background ($0$).
* **Negative Template ($N$):** The character is drawn in black ($0$) on a white background ($255$).
* *Property:* Since they are exact opposites, $\min(P(x, y), N(x, y)) = 0$ for all pixel coordinates $(x, y)$.

The secret binary image is divided into a grid of blocks matching the size $H \times W$ of the character templates. For each grid block $(r, c)$ in the secret image:

1. Retrieve the corresponding character template index from the public text. Let its templates be $P$ and $N$.
2. Generate a random bit $A \in \{0, 1\}$.
3. Inspect the intensity of the secret image pixel corresponding to the block:

#### Case A: The secret pixel is White (Background)
Assign the same template to both shares:

$$
\text{Share 1}^{(r,c)} = \begin{cases} N & \text{if } A = 0 \\ P & \text{if } A = 1 \end{cases}, \quad \text{Share 2}^{(r,c)} = \begin{cases} N & \text{if } A = 0 \\ P & \text{if } A = 1 \end{cases}
$$

*Superposition:* $\min(\text{Share 1}, \text{Share 2}) = N \text{ or } P$ (the character remains visible).

#### Case B: The secret pixel is Black (Secret Foreground)
Assign opposite templates to the shares:

$$
\text{Share 1}^{(r,c)} = \begin{cases} N & \text{if } A = 0 \\ P & \text{if } A = 1 \end{cases}, \quad \text{Share 2}^{(r,c)} = \begin{cases} P & \text{if } A = 0 \\ N & \text{if } A = 1 \end{cases}
$$

*Superposition:* $\min(\text{Share 1}, \text{Share 2}) = \min(P, N) = \mathbf{0}$ (completely black block, destroying the character).

#### Visual Security
On their own, both Share 1 and Share 2 are grids of public letters randomly alternating between positive and negative styles. Because of this random distribution, they both look like standard patterned text grids and leak no information about the secret shape. However, when overlaid, the blocks corresponding to the secret image's black silhouette turn solid black, while the background blocks continue to display the public characters, revealing the secret image.

---

## 📂 Project Structure

The project has been restructured to use a single unified entry point `visual_crypto` along with modular Strategy/Command classes and separated header files:

```
Visual-cryptography/
├── CMakeLists.txt              # Main CMake compilation script
├── README.md                   # Project documentation and theory
├── assets/                     # Input/test images
│   ├── 0791_orig.png           # Sample secret image
│   └── map.jpg                 # Sample image for ASCII art
├── include/                    # Header files (.hpp)
│   ├── Task.hpp                # CryptographyTask interface and TaskFactory
│   ├── ClassicVCTask.hpp       # Classic Visual Cryptography task header
│   ├── DualLayerVCTask.hpp     # Dual-Layer Visual Cryptography task header
│   ├── ASCIIArtTask.hpp        # ASCII Art Generator task header
│   └── Utils.hpp               # Shared visual cryptography helper headers
└── src/                        # Source files (.cpp)
    ├── main.cpp                # The single unified entrypoint
    ├── TaskFactory.cpp         # Factory implementation
    ├── ClassicVCTask.cpp       # Classic Visual Cryptography task implementation
    ├── DualLayerVCTask.cpp     # Dual-Layer Visual Cryptography task implementation
    ├── ASCIIArtTask.cpp        # ASCII Art Generator task implementation
    └── Utils.cpp               # Shared visual cryptography helper source
```

---

## 🛠️ Compilation

This project requires **CMake** and **OpenCV** installed on your system.

### Prerequisites

* **C++ Compiler:** Supporting C++17.
* **OpenCV:** OpenCV 3 or 4.
* **CMake:** Version 3.12 or higher.

### Build Steps

1. Open your terminal (e.g., PowerShell on Windows or Bash on Linux/WSL).
2. Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build
   ```
3. Generate the build configuration and compile the binaries:
   ```bash
   cmake ..
   cmake --build . --config Release
   ```

This will generate two executable files in your build directory:
* `visual_crypto` (Unified master binary)
* `visual_crypto_tests` (Unit test runner)

---

## 🚀 Running the Executable

Run the executable from the project root directory using the following syntax:

```bash
./build/visual_crypto <mode> [arguments]
```

### 1. Dual-Layer Visual Cryptography
Encrypts a secret image using public text characters.
```bash
./build/visual_crypto dual_layer assets/0791_orig.png
```
* You will be prompted to enter 4 alphabets (e.g., `ABCD`).
* Generates:
  * `imgShare1.png` - Share 1 showing the public characters.
  * `imgShare2.png` - Share 2 showing the public characters.
  * `reconstructed.png` - Overlaid result revealing the secret image shape.

### 2. Classic Visual Cryptography
Standard Naor-Shamir (2, 2) Visual Cryptography.
```bash
./build/visual_crypto classic assets/0791_orig.png
```
* Generates:
  * `classic_share1.png` - Share 1 (looks like random noise).
  * `classic_share2.png` - Share 2 (looks like random noise).
  * `classic_reconstructed.png` - Overlaid result revealing the secret image.

### 3. ASCII Art Generator
Converts any image into ASCII characters printed in your terminal.
```bash
./build/visual_crypto ascii_art assets/map.jpg
```
* Displays the ASCII representation of the image directly in the terminal output.

---

## 🧪 Running the Unit Tests

Execute ctest within the build directory to run the test suite:
```bash
cd build
ctest -V
```

Or execute the test binary directly from the project root:
```bash
./build/visual_crypto_tests
```

