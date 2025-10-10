/*
 Project: OOP Assignment 1 - Part 2
 Section: S30  (all of us)

 Team Members:
 - Hazem Hamdy Hamed, ID: 20240152 → solve: (grayscale, merge, edge detection, adjust brightness, diagram, menu)
 - Mohamed Khaled Anwar, ID: 20240493 → solve: (black&white, flip, crop, resizeImage, looking purple at night )
 - Mahmoud Khaled Anwar, ID: 20240550 → solve: (invert, rotate, adding frame, blur, Infrared Filter)
*/

// link of document: https://docs.google.com/document/d/1uGtShtMJUDm_XX7-V8u84DxUwOUcwK8tlMircFK79K0/edit?tab=t.0
// link video demo of program: https://drive.google.com/drive/folders/1QAhONA7PZFEYs6khNosLfCgnPLGURe5z
//github repo link: https://github.com/HazemHamdy-10/CS213_A1
#include <bits/stdc++.h>
#include "Image_Class.h"

using namespace std;

Image img;
bool loaded = false;
bool modified = false;

// check valid extension
bool validExtension(const string &name) {
    string exts[] = {".jpg", ".png", ".bmp", ".jpeg"};
    for (auto &e: exts) {
        if (name.size() >= e.size() && equal(e.rbegin(), e.rend(), name.rbegin()))
            return true;
    }
    return false;
}

bool askToSave() {
    if (loaded && modified) {
        char c;
        cout << "Do you want to save the current image before proceeding? (y/n): ";
        cin >> c;
        if (c == 'y' || c == 'Y') {
            string name;
            cout << "Enter filename to save (with extension): ";
            cin >> name;
            if (!validExtension(name)) {
                cout << "Invalid extension!\n";
                return false;
            }
            if (img.saveImage(name)) {
                cout << "Image saved as " << name << endl;
                modified = false;
            } else {
                cout << "Failed to save!\n";
                return false;
            }
        }
    }
    return true;
}

void loadImage() {
    if (!askToSave())
        return;
    string name;
    cout << "Enter image name (with extension): ";
    cin >> name;
    if (!validExtension(name)) {
        cout << "Invalid extension!\n";
        return;
    }
    if (img.loadNewImage(name)) {
        cout << "Image loaded successfully!\n";
        loaded = true;
        modified = false;
    } else {
        cout << "Failed to load image!\n";
    }
}

void saveImage() {
    if (!loaded) {
        cout << "No image loaded!\n";
        return;
    }
    string name;
    cout << "Enter filename to save (with extension): ";
    cin >> name;
    if (!validExtension(name)) {
        cout << "Invalid extension!\n";
        return;
    }
    if (img.saveImage(name)) {
        cout << "Saved as " << name << endl;
        modified = false;
    } else {
        cout << "Failed to save!\n";
    }
}
// Filter 1 : Grayscale Conversion
void grayScale() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    for (int i = 0; i < img.width; i++)
        for (int j = 0; j < img.height; j++) {
            unsigned avg = 0;
            for (int k = 0; k < img.channels; k++)
                avg += img(i, j, k);
            avg /= img.channels;
            for (int k = 0; k < img.channels; k++)
                img(i, j, k) = avg;
        }
    cout << "Gray Scale applied.\n";
    modified = true;
}
// Filter 2 : Black and White
void blackWhite() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {

            unsigned int avg = 0;
            for (int k = 0; k < img.channels; ++k) {
                avg += img(i, j, k);
            }
            avg = avg / img.channels;

            for (int k = 0; k < img.channels; ++k) {
                if (avg < 128)
                    img(i, j, k) = 0;
                else
                    img(i, j, k) = 255;
            }
        }
    }
    cout << "Black & White applied.\n";
    modified = true;
}
// Filter 3 : Invert Image
void invert() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    for (int i = 0; i < img.width; i++)
        for (int j = 0; j < img.height; j++)
            for (int k = 0; k < img.channels; k++)
                img(i, j, k) = 255 - img(i, j, k);
    cout << "Invert applied.\n";
    modified = true;
}
// Filter 4 : Merge Images
void merge() {
    string n1, n2;
    cout << "Enter first image: ";
    cin >> n1;
    cout << "Enter second image: ";
    cin >> n2;
    Image img1, img2;
    if (!img1.loadNewImage(n1) || !img2.loadNewImage(n2)) {
        cout << "Failed to load images!\n";
        return;
    }
    int newW = max(img1.width, img2.width);
    int newH = max(img1.height, img2.height);
    Image r1(newW, newH), r2(newW, newH);
    for (int i = 0; i < newW; i++)
        for (int j = 0; j < newH; j++) {
            int oldx1 = i * img1.width / newW;
            int oldy1 = j * img1.height / newH;
            int oldx2 = i * img2.width / newW;
            int oldy2 = j * img2.height / newH;
            for (int k = 0; k < img1.channels; k++) {
                r1(i, j, k) = img1(oldx1, oldy1, k);
                r2(i, j, k) = img2(oldx2, oldy2, k);
            }
        }
    img = Image(newW, newH);
    for (int i = 0; i < newW; i++)
        for (int j = 0; j < newH; j++)
            for (int k = 0; k < r1.channels; k++)
                img(i, j, k) = (r1(i, j, k) + r2(i, j, k)) / 2;
    loaded = true;
    modified = true;
    cout << "Images merged.\n";
}
// Filter 5 : Flip Image
void flip() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    char choice;
    cout << "Flip (H/V): ";
    cin >> choice;
    if (choice == 'H' || choice == 'h') {
        for (int i = 0; i < img.width / 2; i++) {
            int i2 = img.width - 1 - i;
            for (int j = 0; j < img.height; j++)
                for (int k = 0; k < img.channels; k++)
                    swap(img(i, j, k), img(i2, j, k));
        }
        cout << "Flipped Horizontally.\n";
    } else {
        for (int j = 0; j < img.height / 2; j++) {
            int j2 = img.height - 1 - j;
            for (int i = 0; i < img.width; i++)
                for (int k = 0; k < img.channels; k++)
                    swap(img(i, j, k), img(i, j2, k));
        }
        cout << "Flipped Vertically.\n";
    }
    modified = true;
}
// Filter 6 : Rotate Image
void rotate() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    int angle;
    cout << "Enter rotation angle (90/180/270): ";
    cin >> angle;
    if (angle != 90 && angle != 180 && angle != 270) {
        cout << "Invalid angle!\n";
        return;
    }
    Image rotated(angle == 180 ? img.width : img.height, angle == 180 ? img.height : img.width);
    for (int i = 0; i < img.width; i++)
        for (int j = 0; j < img.height; j++)
            for (int k = 0; k < img.channels; k++) {
                if (angle == 90)
                    rotated(img.height - 1 - j, i, k) = img(i, j, k);
                else if (angle == 180)
                    rotated(img.width - 1 - i, img.height - 1 - j, k) = img(i, j, k);
                else
                    rotated(j, img.width - 1 - i, k) = img(i, j, k);
            }
    img = rotated;
    cout << "Rotated " << angle << " degrees.\n";
    modified = true;
}
// Filter 7 : Darken and Lighten Image
void adjustBrightness() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    int percent;
    cout << "Enter brightness level (-100 to 100): ";
    cin >> percent;

    if (percent < -100 || percent > 100) {
        cout << "Invalid value! Must be between -100 and 100.\n";
        return;
    }

    float factor = percent / 100.0f;

    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int k = 0; k < img.channels; k++) {
                int newVal = img(i, j, k) + img(i, j, k) * factor;
                if (newVal > 255) newVal = 255;
                if (newVal < 0) newVal = 0;
                img(i, j, k) = newVal;
            }
        }
    }

    if (percent > 0)
        cout << "Image lightened by " << percent << "%.\n";
    else if (percent < 0)
        cout << "Image darkened by " << -percent << "%.\n";
    else
        cout << "No change applied.\n";

    modified = true;
}

// Filter 8 : Crop Images
void crop() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    cout << "Original image size: " << img.width << " x " << img.height << endl;

    int x, y, w, h;
    cout << "Enter the starting point (x y) of the upper-left corner:\n";
    cin >> x >> y;
    cout << "Enter the width and height (W H) of the area to crop:\n";
    cin >> w >> h;

    if (x  < 0 || y < 0 || w <= 0 || h <= 0 || x + w > img.width || y + h > img.height) {
        cout << "Invalid crop dimensions. They must fit within the original image.\n";
        return;
    }

    Image cropped(w, h);

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            for (int c = 0; c < img.channels; ++c) {
                cropped(i, j, c) = img(x + i, y + j, c);
            }
        }
    }

    img = cropped;
    loaded = true;
    cout << "Image cropped successfully.\n";
    cout << "New image size: " << w << " x " << h << endl;
    modified = true;

}

// Filter 9 : Adding a Frame to the Picture
void addFrame() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    int thickness, r, g, b;
    cout << "Enter frame thickness (pixels): ";
    cin >> thickness;
    cout << "Enter frame color (R G B): ";
    cin >> r >> g >> b;

    for (int y = 0; y < img.height; ++y)
        for (int x = 0; x < img.width; ++x)
            if (x < thickness || x >= img.width - thickness ||
                y < thickness || y >= img.height - thickness) {
                img(x, y, 0) = r;
                img(x, y, 1) = g;
                img(x, y, 2) = b;
            }

    cout << "Frame added successfully.\n";
    modified = true;
}

// Filter 10 : Detect Image Edges
void edgeDetection() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            unsigned avg = 0;
            for (int k = 0; k < img.channels; k++)
                avg += img(i, j, k);

            avg /= 3;

            unsigned bw = (avg < 128) ? 0 : 255;
            for (int k = 0; k < img.channels; k++)
                img(i, j, k) = bw;
        }
    }

    Image result = img;
    for (int i = 1; i < img.width; i++) {
        for (int j = 1; j < img.height; j++) {
            int current = img(i, j, 0);
            int prev = img(i - 1, j, 0);
            int up = img(i, j - 1, 0);

            if (current == prev && current == up)
                result(i, j, 0) = result(i, j, 1) = result(i, j, 2) = 255; // أبيض
            else
                result(i, j, 0) = result(i, j, 1) = result(i, j, 2) = 0;   // أسود
        }
    }

    img = result;
    cout << "Edge detection applied\n";
    modified = true;
}
// Filter 11 : Resizing Images
void resizeImage() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    int newWidth, newHeight;
    cout << "Enter the new width and height of the image:\n";
    cin >> newWidth >> newHeight;

    if (newWidth <= 0 || newHeight <= 0) {
        cout << "Invalid dimensions!\n";
        return;
    }

    // Create a new image with the desired size
    Image resizedImage(newWidth, newHeight);

    // Compute scale factors
    float x_ratio = static_cast<float>(img.width) / newWidth;
    float y_ratio = static_cast<float>(img.height) / newHeight;

    // Nearest Neighbor Interpolation
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int srcX = static_cast<int>(x * x_ratio);
            int srcY = static_cast<int>(y * y_ratio);

            for (int c = 0; c < img.channels; ++c) {
                resizedImage(x, y, c) = img(srcX, srcY, c);
            }
        }
    }


    img = resizedImage;
    loaded = true;
    cout << "Image resized successfully!\n";
    cout << "Original size: " << img.width << "x" << img.height << endl;
    cout << "New size: " << newWidth << "x" << newHeight << endl;
    modified = true;

}

// Filter 12 : Blur Images
void blurImage() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    int radius;
    cout << "Enter blur strength : ";
    cin >> radius;

    Image temp(img.width, img.height);        // Create a temporary image with the same dimensions as the original

    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            for (int c = 0; c < img.channels; ++c) {

                int sum = 0, count = 0;           // Initialize variables for calculating average

                // Iterate through vertical neighbors within the blur radius
                for (int dy = -radius; dy <= radius; ++dy) {

                    // Iterate through horizontal neighbors within the blur radius
                    for (int dx = -radius; dx <= radius; ++dx) {

                        // Calculate neighbor pixel coordinates
                        int nx = x + dx, ny = y + dy;

                        // Check if neighbor coordinates are within image boundaries
                        if (nx >= 0 && nx < img.width && ny >= 0 && ny < img.height) {
                            // Add the neighbor's pixel value to sum
                            sum += img(nx, ny, c);
                            // Increment count of valid neighbors
                            count++;
                        }
                    }
                }
                // Calculate and store the average value for current pixel and channel
                // This creates the blur effect by averaging neighboring pixels
                temp(x, y, c) = sum / count;
            }
        }
    }


    img = temp;
    cout << "Blur applied successfully.\n";
    modified = true;
}

// Filter 16 : wano purple (looking purple at night)
void wanoPurple() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    if (img.width <= 0 || img.height <= 0 || img.channels <= 0) {
        cout << "Invalid image properties!\n";
        return;
    }

    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            int red = img(i, j, 0);
            int green = img(i, j, 1);
            int blue = img(i, j, 2);

            int newRed = min(255, red + 60);
            int newGreen = max(0, green - 20);
            int newBlue = min(255, blue + 40);

            img(i, j, 0) = (newRed + red) / 2;
            img(i, j, 1) = newGreen;
            img(i, j, 2) = (newBlue + blue) / 2;
        }
    }

    cout << "Wano purple night filter applied successfully!\n";
    modified = true;

}

// Filter 17 : infrared
void infraredFilter() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    for (int i = 0; i < img.width; ++i)
        for (int j = 0; j < img.height; ++j)
            for (int k = 0; k < img.channels; ++k) {
                if (k == 0) // Red channel → max intensity
                    img(i, j, k) = 255;
                else // Invert other channels
                    img(i, j, k) = 255 - img(i, j, k);
            }

    cout << "Infrared effect applied.\n";
    modified = true;
}


int main() {
    cout << "=== Welcome to Image Processing Program ===\n";
    loadImage();  // must load at start

    int choice;
    do {
        cout << "\n==== Menu ====\n";
        cout << "1. Load New Image\n";
        cout << "2. Gray Scale\n";
        cout << "3. Black and White\n";
        cout << "4. Invert Image\n";
        cout << "5. Merge Images\n";
        cout << "6. Flip Image\n";
        cout << "7. Rotate Image\n";
        cout << "8. Darken/Lighten Image\n";
        cout << "9. Crop Images\n";
        cout << "10. Adding a Frame to the Picture\n";
        cout << "11. Detect Image Edges\n";
        cout << "12. Resizing Images\n";
        cout << "13. Blur Images\n";
        cout << "14. Wano purple\n";
        cout << "15. infrared\n";
        cout << "16. Save Image\n";
        cout << "17. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1:
                loadImage();
                break;
            case 2:
                grayScale();
                break;
            case 3:
                blackWhite();
                break;
            case 4:
                invert();
                break;
            case 5:
                merge();
                break;
            case 6:
                flip();
                break;
            case 7:
                rotate();
                break;
            case 8:
                adjustBrightness();
                break;
            case 9 :
                crop();
                break;
            case 10:
                addFrame();
                break;
            case 11:
                edgeDetection();
                break;
            case 12:
                resizeImage();
                break;
            case 13:
                blurImage();
                break;
            case 14:
                wanoPurple();
                break;
            case 15:
                infraredFilter();
                break;
            case 16:
                saveImage();
                break;
            case 17:
                if (!askToSave())
                    break;
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 17);

    return 0;
}
