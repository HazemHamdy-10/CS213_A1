/*
 Name : Hazem Hamdy Hamed
 ID : 20240152
 OOP Assignment 1 - Part 2
 Section: S30
 Filters : (grayscale, merge, edge detection, adjust brightness, diagram, menu, TV images, natural sunlight)
*/

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
                result(i, j, 0) = result(i, j, 1) = result(i, j, 2) = 255;
            else
                result(i, j, 0) = result(i, j, 1) = result(i, j, 2) = 0;
        }
    }

    img = result;
    cout << "Edge detection applied\n";
    modified = true;
}

void fixSunlight() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            int r = img(i, j, 0);
            int g = img(i, j, 1);
            int b = img(i, j, 2);


            r = min(255, (int)(r * 1.2));
            g = min(255, (int)(g * 1.1));
            b = min(255, (int)(b * 0.9));

            img(i, j, 0) = r;
            img(i, j, 1) = g;
            img(i, j, 2) = b;
        }
    }

    cout << "Sunlight fixed successfully!\n";
}

void oldTVEffect() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {


            if (j % 3 == 0) {
                for (int k = 0; k < img.channels; k++) {
                    int val = img(i, j, k) * 0.3;
                    if (val < 0) val = 0;
                    img(i, j, k) = val;
                }
            }
        }
    }

    cout << "Old TV effect applied.\n";
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
        cout << "3. Merge Images\n";
        cout << "4. Darken/Lighten Image\n";
        cout << "5. Detect Image Edges\n";
        cout << "6. TV images\n";
        cout << "7. natural sunlight\n";
        cout << "8. Save Image\n";
        cout << "9. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                loadImage();
                break;
            case 2:
                grayScale();
                break;
            case 3:
                merge();
                break;
            case 4:
                adjustBrightness();
                break;
            case 5:
                edgeDetection();
                break;
            case 6:
                oldTVEffect();
                break;
            case 7:
                fixSunlight();
                break;
            case 8:
                saveImage();
                break;
            case 9:
                if (!askToSave())
                    break;
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 9);

    return 0;
}


