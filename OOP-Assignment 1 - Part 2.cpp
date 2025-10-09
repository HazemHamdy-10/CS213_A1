/// Name : Mahmoud Khaled Anwar Lethy
/// ID : 20240550
/// Assignment 1 - Part 2
/// Filters: (3)Invert Image, (6)Rotate Image, (9)Adding a Frame to the Picture, (12)Blur Image,
/// (17)Infrared photography.


/*
 Project: OOP Assignment - Image Filters Menu
 Filters included:
 3. Invert Image
 6. Rotate Image
 9. Add Frame to Image
 12. Blur Image
 17. Infrared (Samurai) Filter
*/

#include <iostream>
#include "Image_Class.h"
using namespace std;

// Global variables
Image img;
bool loaded = false;
bool modified = false;

// Validate image extension
bool validExtension(const string &name) {
    string exts[] = {".jpg", ".png", ".bmp", ".jpeg"};
    for (auto &e : exts)
        if (name.size() >= e.size() && equal(e.rbegin(), e.rend(), name.rbegin()))
            return true;
    return false;
}

// Ask user to save before proceeding
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
                cout << "Image saved successfully.\n";
                modified = false;
            } else {
                cout << "Failed to save image!\n";
                return false;
            }
        }
    }
    return true;
}

// Load image
void loadImage() {
    if (!askToSave()) return;

    string name;
    cout << "Enter image name (with extension): ";
    cin >> name;

    if (!validExtension(name)) {
        cout << "Invalid extension!\n";
        return;
    }
    if (img.loadNewImage(name)) {
        cout << "Image loaded successfully.\n";
        loaded = true;
        modified = false;
    } else {
        cout << "Failed to load image!\n";
    }
}

// Save image
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

// ========== FILTERS ==========

/// Filter 3: Invert Image
void invertImage() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    for (int i = 0; i < img.width; ++i)
        for (int j = 0; j < img.height; ++j)
            for (int k = 0; k < img.channels; ++k)
                img(i, j, k) = 255 - img(i, j, k);

    cout << "Invert filter applied.\n";
    modified = true;
}

//  Filter 6: Rotate Image
void rotateImage() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    int angle;
    cout << "Enter rotation angle (90 / 180 / 270): ";
    cin >> angle;

    if (angle != 90 && angle != 180 && angle != 270) {
        cout << "Invalid angle!\n";
        return;
    }

    Image rotated(angle == 180 ? img.width : img.height,
                  angle == 180 ? img.height : img.width);

    for (int i = 0; i < img.width; ++i)
        for (int j = 0; j < img.height; ++j)
            for (int k = 0; k < img.channels; ++k) {
                if (angle == 90)
                    rotated(img.height - 1 - j, i, k) = img(i, j, k);
                else if (angle == 180)
                    rotated(img.width - 1 - i, img.height - 1 - j, k) = img(i, j, k);
                else
                    rotated(j, img.width - 1 - i, k) = img(i, j, k);
            }

    img = rotated;
    cout << "Rotation applied successfully.\n";
    modified = true;
}

//  Filter 9: Adding a Frame to the Picture
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

// Filter 12: Blur Images
void blurImage() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    int blurRange;
    cout << "Enter blur strength : ";
    cin >> blurRange;

    Image temp(img.width, img.height);

    for (int y = 0; y < img.height; ++y)
        for (int x = 0; x < img.width; ++x)
            for (int c = 0; c < img.channels; ++c) {
                int sum = 0, count = 0;
                for (int dy = -blurRange; dy <= blurRange; ++dy)
                    for (int dx = -blurRange; dx <= blurRange; ++dx) {
                        int nx = x + dx, ny = y + dy;
                        if (nx >= 0 && nx < img.width && ny >= 0 && ny < img.height) {
                            sum += img(nx, ny, c);
                            count++;
                        }
                    }
                temp(x, y, c) = sum / count;
            }

    img = temp;
    cout << "Blur applied successfully.\n";
    modified = true;
}

// Filter 17: Infrared photography
void infraredFilter() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    if (img.width <= 0 || img.height <= 0 || img.channels <= 0) {
        cout << "Invalid image properties!\n";
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

    cout << "Infrared (Samurai) effect applied.\n";
    modified = true;
}

// ===== MAIN MENU =====
int main() {
    cout << "=== Welcome to Image Processing Menu ===\n";
    loadImage();

    int choice;
    do {
        cout << "\n==== Menu ====\n";
        cout << "1. Load New Image\n";
        cout << "2. Invert Image\n";
        cout << "3. Rotate Image\n";
        cout << "4. Add Frame\n";
        cout << "5. Blur Image\n";
        cout << "6. Infrared (Samurai) Filter\n";
        cout << "7. Save Image\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: loadImage(); break;
            case 2: invertImage(); break;
            case 3: rotateImage(); break;
            case 4: addFrame(); break;
            case 5: blurImage(); break;
            case 6: infraredFilter(); break;
            case 7: saveImage(); break;
            case 8:
                if (!askToSave()) break;
                cout << "Goodbye!\n";
                break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 8);

    return 0;
}

