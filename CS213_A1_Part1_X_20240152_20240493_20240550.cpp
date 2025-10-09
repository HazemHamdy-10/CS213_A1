/*
 Name : Hazem Hamdy Hamed
 ID : 20240152
 OOP Assignment 1 - Part 2
 Section: S30
 Filters : (grayscale, merge, edge detection, adjust brightness, diagram, menu)
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

void blackWhite() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    for (int i = 0; i < img.width; i++)
        for (int j = 0; j < img.height; j++) {
            unsigned avg = 0;
            for (int k = 0; k < img.channels; k++) avg += img(i, j, k);
            avg /= img.channels;
            unsigned bw = (avg < 128) ? 0 : 255;
            for (int k = 0; k < img.channels; k++) img(i, j, k) = bw;
        }
    cout << "Black & White applied.\n";
    modified = true;
}

void flip() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    char c;
    cout << "Flip (H/V): ";
    cin >> c;
    if (c == 'H' || c == 'h') {
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

void rotate() {
    if (!loaded) {
        cout << "⚠ Load an image first!\n";
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

int main() {
    cout << "=== Welcome to Image Processing Program ===\n";
    loadImage();  // must load at start

    int choice;
    do {
        cout << "\n==== Menu ====\n";
        cout << "1. Load New Image\n";
        cout << "2. Gray Scale\n";
        cout << "3. Invert\n";
        cout << "4. Black & White\n";
        cout << "5. Flip\n";
        cout << "6. Rotate\n";
        cout << "7. Merge Two Images\n";
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
                invert();
                break;
            case 4:
                blackWhite();
                break;
            case 5:
                flip();
                break;
            case 6:
                rotate();
                break;
            case 7:
                merge();
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

