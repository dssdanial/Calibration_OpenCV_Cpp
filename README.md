# 📸 Camera Calibration with OpenCV

## 🔍 Overview

This project demonstrates how to calibrate a camera using OpenCV in C++. The process involves capturing a set of images of a checkerboard pattern, detecting corner points, estimating intrinsic and extrinsic parameters, and undistorting images.

---

## 📂 Project Structure

```plaintext
📁 Project Root
├── 📂 src/                  # Source code directory
│   ├── 📄 main.cpp          # Main C++ file for calibration
├── 📂 checkerboard/         # Folder containing calibration images
├── 📂 results/              # Folder to store results (optional)
├── 📄 README.md             # Documentation
└── 📄 CMakeLists.txt        # CMake build script (if applicable)
```

---

## 🔢 Step-by-Step Process

### 1️⃣ Load Images 🖼️

- Use **`cv::glob()`** to load images with a specific pattern (e.g., `frame*.jpg`).
- Capture images from different angles for better calibration accuracy.

📌 **Key Point:** Ensure good lighting and sharp focus.

---

### 2️⃣ Define Checkerboard Parameters 📏

- Set **checkerboard size** (e.g., **8x5 internal corners** for a 9x6 grid).
- Initialize a 3D object point vector.

🛠️ **Formula:**

```
patternSize = cv::Size(width_corners, height_corners);
```

---

### 3️⃣ Detect Corners 🔍

✅ **Steps:**
1. Convert image to grayscale: `cv::cvtColor()`
2. Detect corners: `cv::findChessboardCorners()`
3. Refine positions: `cv::cornerSubPix()`

🔎 **Corner Detection Visualization:**

```
◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼
◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼
◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼ ◼
``` 
(Detected corners marked within the grid)

---

### 4️⃣ Store Object & Image Points 📊

📍 **Real-world 3D points:** Stored in `Q`
📍 **Detected 2D points:** Stored in `q`

📝 **Formula:**
```cpp
std::vector<std::vector<cv::Point3f>> Q;
std::vector<std::vector<cv::Point2f>> q;
```

---

### 5️⃣ Visualize Corner Detection 🎨

- Use **`cv::drawChessboardCorners()`** to draw corners.
- Display images with **`cv::imshow()`** for verification.

📌 **Example:**
```cpp
cv::drawChessboardCorners(image, patternSize, corners, found);
```

---

### 6️⃣ Camera Calibration 📷

- Call **`cv::calibrateCamera()`** to estimate:
  - 🔲 **Intrinsic matrix (K)** (Focal length & principal point)
  - 🔄 **Distortion coefficients (k)** (Lens distortions)
  - 🛑 **Rotation & translation vectors (rvecs, tvecs)** (Camera orientation & position)

📌 **Re-projection Error** should be **below 0.5 pixels** for good accuracy.

📊 **Mathematical Representation:**
```math
q = K [R|t] Q
```

---

### 7️⃣ Compute Undistortion Maps 🔄

- Use **`cv::initUndistortRectifyMap()`** to precompute pixel mappings for undistortion.

📌 **Key Function:**
```cpp
cv::initUndistortRectifyMap(K, dist, cv::Mat(), K, imageSize, CV_16SC2, map1, map2);
```

---

### 8️⃣ Undistort Images ✨

- Apply **`cv::remap()`** to remove distortion.
- Display and save undistorted images.

📌 **Before vs. After Comparison:**

```
Original:            Undistorted:
██████████          ░░░░░░░░░░
██████████    →    ░░░░░░░░░░
██████████          ░░░░░░░░░░
```

---

### 9️⃣ Evaluate Calibration Quality ✅

📌 **Steps to Improve Accuracy:**
- Capture **more images** 📸
- Ensure **good lighting** 💡
- Use **higher resolution** images 🔍

📊 **Ideal Re-projection Error:**
```
< 0.5 pixels ✅
> 1.0 pixels ❌ (Recalibrate!)
```

---

### 🔟 Adjust Parameters & Repeat ♻️

- If calibration error is **too high**, modify parameters and re-run the process.

📌 **Consider:**
✅ More **images**
✅ Better **checkerboard pattern**
✅ Higher **resolution**
✅ Better **lens focus**

---


## 📊 Results

### 📷 **Comparison of Original and Undistorted Image**
| Original Image | Undistorted Image |
|---------------|------------------|
| ![Original](results/original.jpg) | ![Undistorted](results/undistorted.jpg) |

### 🔢 **Camera Matrices**

```
Re-projection Error= 0.270713  

Intrinsic Matrix (K):
⎡ fx   0   cx ⎤    ⎡ 1090.8203     0        959.5 ⎤
⎢ 0    fy  cy ⎥ =  ⎢     0    1090.8203     539.5 ⎥
⎣ 0    0    1 ⎦    ⎣     0          0        1    ⎦

Distortion Coefficients (k):
[k1, k2, p1, p2, k3]=[-0.402892, 0.161711, 0, 0, 0]
```






📌 **Explanation:**  
The **intrinsic matrix (K)** defines the camera’s focal length and principal point.  
The **distortion coefficients (k)** describe the lens distortions.  


---
## 🛠️ Usage

### **Compile and Run** 🚀
```bash
mkdir build && cd build
cmake ..
make
./camera_calibration
```

### **Required Dependencies** ⚙️
- ✅ OpenCV 4.x
- ✅ C++14 or later
- ✅ CMake (if applicable)

---

## 📜 License

This project is open-source under the **MIT License**.

📌 **Contributions are welcome!** Feel free to modify and improve the calibration process. 🚀
